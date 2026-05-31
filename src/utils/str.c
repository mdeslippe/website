#include <assert.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "str.h"
#include "musl/memmem.h"

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

    size_t overlap_offset = 0;
    bool is_overlapping = pointer_is_in_block(
        value,
        string->data,
        string->capacity,
        &overlap_offset
    );

    StringResult result = string_reserve(string, length + 1);

    if (result != STRING_SUCCESS) {
        return result;
    }

    if (is_overlapping) {
        value = string->data + overlap_offset;
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
    size_t overlap_offset = 0;
    bool is_overlapping = pointer_is_in_block(
        value,
        string->data,
        string->capacity,
        &overlap_offset
    );

    StringResult result = string_reserve(string, required_capacity);

    if (result != STRING_SUCCESS) {
        return result;
    }

    if (is_overlapping) {
        value = string->data + overlap_offset;
    }

    memmove(string->data + old_length, value, length);

    string->length = old_length + length;
    string->data[old_length + length] = '\0';

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_insert(
    String* string,
    size_t index,
    const char* value,
    size_t length
) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (index > string->length) {
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
    size_t overlap_offset = 0;
    bool is_overlapping = pointer_is_in_block(
        value,
        string->data,
        string->capacity,
        &overlap_offset
    );

    StringResult result = string_reserve(string, required_capacity);

    if (result != STRING_SUCCESS) {
        return result;
    }

    if (is_overlapping) {
        value = string->data + overlap_offset;
    }

    // Shift existing contents, including the null terminator (+1), to the right
    // to make room for the value being inserted.
    memmove(
        string->data + index + length,
        string->data + index,
        old_length - index + 1
    );

    // Insert content depending on if and where overlap occurs.
    if (!is_overlapping) {
        // Source does not overlap.
        memmove(string->data + index, value, length);
    } else if (overlap_offset >= index) {
        // Source is entirely to the right of the insertion point.
        // We need to shift it forward.
        memmove(string->data + index, value + length, length);
    } else if (overlap_offset + length <= index) {
        // Source is entirely to the left of the insertion point.
        // It did not move.
        memmove(string->data + index, value, length);
    } else {
        // Source straddles the insertion point and has been torn in half.
        size_t left_length = index - overlap_offset;
        size_t right_length = length - left_length;

        // Copy the unmoved left half into the start of the gap.
        memmove(string->data + index, value, left_length);

        // Copy the shifted right half into the remainder of the gap.
        memmove(
            string->data + index + left_length,
            value + left_length + length,
            right_length
        );
    }

    string->length = old_length + length;
    string->data[old_length + length] = '\0';

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_remove(String* string, size_t index, size_t length) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (index > string->length) {
        return STRING_ERROR_ARGUMENT;
    }

    if (length > string->length - index) {
        return STRING_ERROR_ARGUMENT;
    }

    STRING_ASSERT_VALID(string);

    if (length == 0) {
        return STRING_SUCCESS;
    }

    memmove(
        string->data + index,
        string->data + index + length,
        string->length + 1 - index - length
    );

    string->length = string->length - length;

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_find(
    const String* string,
    size_t start_index,
    const char* value,
    size_t length,
    size_t* match_index
) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (match_index == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (start_index > string->length) {
        return STRING_ERROR_ARGUMENT;
    }

    if (value == NULL && length != 0) {
        return STRING_ERROR_ARGUMENT;
    }

    STRING_ASSERT_VALID(string);

    if (length == 0) {
        *match_index = start_index;
        return STRING_SUCCESS;
    }

    const char* haystack = string->data + start_index;
    size_t haystack_length = string->length - start_index;

    const char* match = memmem(haystack, haystack_length, value, length);

    if (match == NULL) {
        return STRING_NOT_FOUND;
    }

    *match_index = match - string->data;

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}

StringResult string_replace_first(
    String* string,
    size_t start_index,
    const char* target,
    size_t target_length,
    const char* replacement,
    size_t replacement_length,
    size_t* match_index
) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    if (start_index > string->length) {
        return STRING_ERROR_ARGUMENT;
    }

    if (target == NULL && target_length != 0) {
        return STRING_ERROR_ARGUMENT;
    }

    if (replacement == NULL && replacement_length != 0) {
        return STRING_ERROR_ARGUMENT;
    }

    STRING_ASSERT_VALID(string);

    size_t found_index = 0;

    StringResult find_result = string_find(
        string,
        start_index,
        target,
        target_length,
        &found_index
    );

    if (find_result != STRING_SUCCESS) {
        return find_result;
    }

    // We need to insert before we delete. If there is overlap, the content that
    // we are deleting may actually be the content we need to insert.
    StringResult insert_result = string_insert(
        string,
        found_index,
        replacement,
        replacement_length
    );

    if (insert_result != STRING_SUCCESS) {
        return insert_result;
    }

    // Removing the original target substring, which now starts immediately 
    // after the inserted replacement, cannot fail because the target bytes are
    // guaranteed to still exist at this position.
    StringResult remove_result = string_remove(
        string,
        found_index + replacement_length,
        target_length
    );

    // If for some reason the remove did fail, the application is in an unknown
    // state and we cannot safely continue.
    if (remove_result != STRING_SUCCESS) {
        abort();
    }

    if (match_index != NULL) {
        *match_index = found_index;
    }

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}
