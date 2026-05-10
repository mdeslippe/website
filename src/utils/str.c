#include <stdlib.h>
#include "str.h"

StringResult string_init(String* string) {

    if (string == NULL) {
        return STRING_ERROR_ARGUMENT;
    }

    char* data = malloc(DEFAULT_STRING_CAPACITY * sizeof(char));

    if (data == NULL) {
        return STRING_ERROR_ALLOCATION;
    }

    string->capacity = DEFAULT_STRING_CAPACITY;
    string->length = 0;
    string->data = data;
    string->data[string->length] = '\0';

    return STRING_SUCCESS;

}

/**
 * @brief Releases resources owned by a string.
 *
 * Releases the internal buffer and resets the string to a zero-initialized
 * state.
 *
 * @note Idempotence - Calling `string_free` on a previously freed or
 *                     zero-initialized string is safe.
 *
 * @note Reinitialization - After this call, the string may be safely
 *                          reinitialized.
 *
 * @param string Pointer to the string to free.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL.
 */
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
