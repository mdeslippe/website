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

    // Aliased (substring) assignments cannot require growth, hence we do not 
    // need to handle the buffer for value being reallocated.
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

    if (string->capacity >= required_capacity) {
        // If we do not need to expand the string's capacity, use memmove to
        // move the value with buffer overlap support.
        memmove(string->data + old_length, value, length);
    } else {
        // Check if 'value' overlaps with 'string->data' using integer address
        // casting. If it does, we must copy it to a temporary buffer before 
        // calling 'string_reserve' because reallocation may turn 'value' into a
        // dangling pointer.
        uintptr_t value_addr = (uintptr_t)value;
        uintptr_t data_start = (uintptr_t)string->data;

        // Check if value resides anywhere inside the string's active allocation
        // block [data_start, data_start + capacity).
        bool is_overlapping = (value_addr >= data_start) &&
            ((value_addr - data_start) < string->capacity);

        const char* source_ptr = value;
        char* copy = NULL;

        if (is_overlapping) {
            copy = malloc(length);
            if (copy == NULL) {
                return STRING_ERROR_ALLOCATION;
            }
            memcpy(copy, value, length);
            source_ptr = copy;
        }

        StringResult result = string_reserve(string, required_capacity);

        if (result != STRING_SUCCESS) {
            if (is_overlapping) {
                free(copy);
            }
            return result;
        }

        memcpy(string->data + old_length, source_ptr, length);

        if (is_overlapping) {
            free(copy);
        }
    }

    string->length = old_length + length;
    string->data[old_length + length] = '\0';

    STRING_ASSERT_VALID(string);

    return STRING_SUCCESS;

}
