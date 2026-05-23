#include <stddef.h>

/**
 * @brief Total initial buffer size including null terminator.
 */
#define DEFAULT_STRING_CAPACITY ((size_t)16)

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

/**
 * @brief Initializes a string.
 *
 * Allocates internal storage and initializes the string to a valid, empty,
 * null-terminated state.
 *
 * @note Reinitialization - Calling `string_init` on an already initialized
 *                          string does not release previously allocated
 *                          resources. The caller must call `string_free` before
 *                          reinitializing.
 *
 * @param string Pointer to the string to initialize.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_init(String* string);

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
StringResult string_free(String* string);

/**
 * @brief Ensures that a string has at least the specified capacity.
 *
 * If the current capacity is less than `capacity`, the internal buffer is
 * reallocated to provide at least `capacity` characters of storage. Otherwise,
 * no action is taken.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - Existing string content is preserved.
 *
 * @note Failure - The string remains unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param capacity Minimum capacity to reserve, including space for the
 *                 null-terminator.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL or `capacity == 0`.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_reserve(String* string, size_t capacity);

/**
 * @brief Replaces the contents of a string with the first `length` bytes of
 *        `value`.
 *
 * Copies `length` bytes from `value` into the string and appends a trailing
 * null-terminator. If necessary, the string is reallocated to provide
 * sufficient capacity for the assigned bytes and the null-terminator.
 *
 * The source bytes may overlap the destination buffer, including regions within
 * the string itself.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - Existing contents are replaced.
 *
 * @note Failure - The string remains unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param value Pointer to the source bytes to assign. May be NULL if
 *              `length == 0`.
 * @param length Number of bytes to assign.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `value` is NULL and `length != 0`,
 *         or if the required capacity cannot be represented.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_assign(String* string, const char* value, size_t length);

/**
 * @brief Appends the contents of the string with the first `length` bytes of
 *        `value`.
 *
 * Copies `length` bytes from `value` to the end of the string and appends a
 * trailing null-terminator. If necessary, the string is reallocated to provide
 * sufficient capacity for the appended bytes and the null-terminator.
 *
 * The source bytes may overlap the destination buffer, including regions within
 * the string itself.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - The bytes are appended to the existing contents.
 *
 * @note Failure - The string remains unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param value Pointer to the source bytes to append. May be NULL if
 *              `length == 0`.
 * @param length Number of bytes to append.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `value` is NULL and `length != 0`,
 *         or if the required capacity cannot be represented.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_append(String* string, const char* value, size_t length);
