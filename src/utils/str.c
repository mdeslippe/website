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
