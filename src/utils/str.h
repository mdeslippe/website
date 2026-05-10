#include <stddef.h>

/**
 * @brief Total initial buffer size including null terminator.
 */
#define DEFAULT_STRING_CAPACITY 16

/**
 * @brief Result codes returned by String operations.
 *
 * Defines the possible outcomes of functions operating on the String type.
 * Functions that can fail should return one of these values to indicate
 * success or the type of failure.
 *
 * @note Usage - A return value of STRING_SUCCESS indicates success. Any other
 *               value indicates an error.
 */
typedef enum {
    /**
     * Operation completed successfully.
     */
    STRING_SUCCESS = 0,

    /**
     * Invalid argument was provided.
     */
    STRING_ERROR_ARGUMENT = 1,

    /**
     * Memory allocation failed.
     */
    STRING_ERROR_ALLOCATION = 2
} StringResult;

/**
 * @brief Dynamically resizable, heap-allocated string.
 *
 * Represents a mutable, null-terminated character buffer stored on the heap.
 * The buffer grows automatically as needed to accommodate additional
 * characters.
 *
 * @note Ownership - Each String instance owns its internal buffer (`data`) and
 *                   is responsible for releasing it when no longer needed.
 *
 * @invariant After initialization:
 * - `1 <= capacity <= SIZE_MAX`
 * - `0 <= length <= capacity - 1`
 * - `data != NULL`
 * - `data` points to a writable buffer of size `capacity`
 * - `data[length] == '\0'`
 */
typedef struct {
    /**
     * Total allocated buffer size in characters (including space for the
     * null-terminator).
     */
    size_t capacity;

    /**
     * Number of characters in the string (excluding the null-terminator).
     */
    size_t length;

    /**
     * Pointer to the character buffer (null-terminated).
     */
    char* data;
} String;
