#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"

/**
 * @brief Asserts that a string is valid.
 *
 * This macro is intended for internal debug assertions to verify that a string
 * satisfies all required invariants.
 *
 * A valid string must satisfy:
 * - `string != NULL`
 * - `string->data != NULL`
 * - `string->capacity > 0`
 * - `string->length < string->capacity`
 * - `string->data[string->length] == '\0'`
 */
#define STRING_ASSERT_VALID(string)              \
    assert(                                      \
        (string) != NULL &&                      \
        (string)->data != NULL &&                \
        (string)->capacity > 0 &&                \
        (string)->length < (string)->capacity && \
        (string)->data[(string)->length] == '\0' \
    )

/**
 * @brief Determines whether `pointer` lies within the memory block beginning at
 * `block_start` and spanning `block_length` bytes.
 *
 * If `pointer` lies within the block, `offset` (if non-NULL) is set to the byte
 * offset of `pointer` relative to `block_start`.
 *
 * The memory block is treated as the half-open interval:
 * [block_start, block_start + block_length)
 *
 * @param pointer Pointer to test.
 * @param block_start Start address of the memory block.
 * @param block_length Size of the memory block in bytes.
 * @param offset Optional output receiving the byte offset of `pointer` from
 *               `block_start`.
 *
 * @return `true` if `pointer` lies within the block, otherwise `false`.
 */
static inline bool pointer_is_in_block(
    const void* pointer,
    const void* block_start,
    size_t block_length,
    size_t* offset
) {

    if (pointer == NULL || block_start == NULL) {
        return false;
    }

    uintptr_t pointer_address = (uintptr_t)pointer;
    uintptr_t start_address = (uintptr_t)block_start;

    if (pointer_address < start_address) {
        return false;
    }

    size_t difference = pointer_address - start_address;

    if (difference >= block_length) {
        return false;
    }

    if (offset != NULL) {
        *offset = difference;
    }

    return true;
}

StringResult string_init(String* string) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    char* data = malloc(DEFAULT_STRING_CAPACITY);

    if (data == NULL) {
        return STRING_ERROR_ALLOCATION;
    }

    string->capacity = DEFAULT_STRING_CAPACITY;
    string->length = 0;
    string->data = data;
    string->data[string->length] = '\0';

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_free(String* string) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (string->data != NULL) {
        free(string->data);
    }

    string->capacity = 0;
    string->length = 0;
    string->data = NULL;

    return STRING_SUCCESS;

}

StringResult string_reserve(String* string, size_t capacity) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (capacity == 0) {
        return STRING_ERROR_ARGUMENT;
    }

    STRING_ASSERT_VALID(string);

    if (capacity <= string->capacity) {
        return STRING_SUCCESS;
    }

    size_t new_capacity = string->capacity;

    while (new_capacity < capacity) {
        if (new_capacity > SIZE_MAX / 2) {
            new_capacity = capacity;
            break;
        }

        new_capacity = new_capacity * 2;
    }

    char* data = realloc(string->data, new_capacity);

    if (data == NULL) {
        return STRING_ERROR_ALLOCATION;
    }

    string->data = data;
    string->capacity = new_capacity;

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_assign(String* string, const char* value, size_t length) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (value == NULL && length != 0) {
        return STRING_ERROR_ARGUMENT;
    }

    if (length == SIZE_MAX) {
        return STRING_ERROR_ARGUMENT;
    }

    STRING_ASSERT_VALID(string);

    if (length == 0) {
        string->length = 0;
        string->data[0] = '\0';

        STRING_ASSERT_VALID(string);

        return STRING_SUCCESS;
    }

    // Aliased (substring) assignments cannot require buffer growth, so we do 
    // not need to handle reallocation of a potentially overlapping value when 
    // calling string_reserve.
    StringResult result = string_reserve(string, length + 1);

    if (result != STRING_SUCCESS) {
        return result;
    }

    memmove(string->data, value, length);

    string->length = length;
    string->data[length] = '\0';

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_append(String* string, const char* value, size_t length) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (value == NULL && length != 0) {
        return STRING_ERROR_ARGUMENT;
    }

    if (length > SIZE_MAX - string->length - 1) {
        return STRING_ERROR_ARGUMENT;
    }

    STRING_ASSERT_VALID(string);

    if (length == 0) {
        return STRING_SUCCESS;
    }

    size_t old_length = string->length;
    size_t required_capacity = old_length + length + 1;

    uintptr_t value_addr = (uintptr_t)value;
    uintptr_t data_start = (uintptr_t)string->data;

    bool is_overlapping = (value_addr >= data_start) &&
        ((value_addr - data_start) < string->capacity);

    size_t overlap_offset = 0;
    if (is_overlapping) {
        overlap_offset = (size_t)(value - string->data);
    }

    StringResult result = string_reserve(string, required_capacity);
    if (result != STRING_SUCCESS) {
        return result;
    }

    const char* source_ptr = value;
    if (is_overlapping) {
        source_ptr = string->data + overlap_offset;
    }

    memmove(string->data + old_length, source_ptr, length);

    string->length = old_length + length;
    string->data[old_length + length] = '\0';

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}
