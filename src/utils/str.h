#ifndef STR_H
#define STR_H

#include <stddef.h>

/**
 * @brief Total initial buffer size including null terminator.
 */
#define DEFAULT_STRING_CAPACITY ((size_t)16)

/**
 * @brief Result codes returned by String operations.
 *
 * Defines the possible outcomes of functions operating on the String type.
 * Functions return one of these values to indicate success, a non-success
 * condition such as a value not being found, or the type of failure.
 *
 * @note Usage - A return value of STRING_SUCCESS indicates success. Any other
 *               value indicates a non-success condition, which may be a normal
 *               outcome (such as STRING_NOT_FOUND) or an error.
 */
typedef enum {
    /**
     * Operation completed successfully.
     */
    STRING_SUCCESS = 0,

    /**
     * Target value was not found.
     */
    STRING_NOT_FOUND = 1,

    /**
     * Invalid argument was provided.
     */
    STRING_ERROR_ARGUMENT = 2,

    /**
     * Memory allocation failed.
     */
    STRING_ERROR_ALLOCATION = 3
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

/**
 * @brief Inserts the first `length` bytes of `value` into the string starting
 *        at `index`.
 *
 * All existing content at and after `index` will be shifted to the right to
 * accommodate the inserted bytes.
 *
 * The source bytes may overlap the destination buffer, including regions within
 * the string itself.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - The bytes are inserted into the existing contents.
 *
 * @note Failure - The string remains unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param index Index at which to begin inserting bytes. Must be less than or
 *              equal to the current string length.
 * @param value Pointer to the source bytes to insert. May be NULL if
 *              `length == 0`.
 * @param length Number of bytes to insert.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `index > string->length`,
 *         or if `value` is NULL and `length != 0`,
 *         or if the required capacity cannot be represented.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_insert(
    String* string,
    size_t index,
    const char* value,
    size_t length
);

/**
 * @brief Removes `length` bytes from the string starting at `index`.
 *
 * All existing content after the removed segment will be shifted to the left.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - The bytes are removed from the existing contents.
 *
 * @note Failure - The string remains unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param index Index at which to begin removing bytes.
 * @param length Number of bytes to remove.
 *
 * @return STRING_SUCCESS on success.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `index + length > string->length`.
 */
StringResult string_remove(String* string, size_t index, size_t length);

/**
 * @brief Searches for the first occurrence of a substring within a string.
 *
 * Searches the string starting at `start_index` for the first occurrence of
 * the substring specified by `value` and `length`. If found, the index of the
 * match is written to `match_index`.
 *
 * If `length` is zero, the search succeeds immediately and `match_index` is set
 * to `start_index`.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - The index of the first match is written to `match_index`.
 *
 * @note Failure - The value of `match_index` remains unmodified.
 *
 * @param string Pointer to the initialized string to search.
 * @param start_index Index at which to begin searching. Must be less than or
 *                    equal to the current string length.
 * @param value Pointer to the substring to search for. May be NULL if
 *              `length == 0`.
 * @param length Length of the substring to search for.
 * @param match_index Pointer to receive the index of the first match.
 *
 * @return STRING_SUCCESS if the substring is found.
 * @return STRING_NOT_FOUND if the substring is not found.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `match_index` is NULL,
 *         or if `start_index > string->length`,
 *         or if `value` is NULL and `length != 0`.
 */
StringResult string_find(
    const String* string,
    size_t start_index,
    const char* value,
    size_t length,
    size_t* match_index
);

/**
 * @brief Replaces the first occurrence of a substring within a string.
 *
 * Searches the string starting at `start_index` for the first occurrence of the
 * substring specified by `target` and `target_length`. If found, the matched
 * substring is replaced with the contents specified by `replacement` and
 * `replacement_length`.
 *
 * If `target_length` is zero, the `replacement` will be inserted at
 * `start_index` and `match_index` will be set to `start_index`.
 *
 * The target and replacement content may overlap the destination buffer,
 * including regions within the string itself.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - The first matching substring is replaced and the index of
 *                 the match is written to `match_index` (if not NULL).
 *
 * @note Failure - The string and the value of `match_index` remain unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param start_index Index at which to begin searching. Must be less than or
 *                    equal to the current string length.
 * @param target Pointer to the substring to search for. May be NULL if
 *               `target_length == 0`.
 * @param target_length Length of the substring to search for.
 * @param replacement Pointer to the replacement content. May be NULL if
 *                    `replacement_length == 0`.
 * @param replacement_length Length of the replacement content.
 * @param match_index Pointer to receive the index of the match. May be NULL if
 *                    the match index is not needed.
 *
 * @return STRING_SUCCESS if the substring is found and replaced.
 * @return STRING_NOT_FOUND if the substring is not found.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `start_index > string->length`,
 *         or if `target` is NULL and `target_length != 0`,
 *         or if `replacement` is NULL and `replacement_length != 0`,
 *         or if the required capacity cannot be represented.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_replace_first(
    String* string,
    size_t start_index,
    const char* target,
    size_t target_length,
    const char* replacement,
    size_t replacement_length,
    size_t* match_index
);

/**
 * @brief Replaces all occurrences of a substring within a string.
 *
 * Searches the string starting at `start_index` for any occurrence of the
 * substring specified by `target` and `target_length`. If found, the matched
 * substring(s) are replaced with the contents specified by `replacement` and
 * `replacement_length`.
 *
 * Unlike `string_replace_first`, `target_length` cannot be zero; this would
 * result in infinite matches.
 *
 * The target and replacement content may overlap the destination buffer,
 * including regions within the string itself.
 *
 * @pre `string` must be initialized with `string_init`.
 *
 * @note Success - The matched substring(s) are replaced and the number of
 *                 matches found is written to `matches_found` (if not NULL).
 *
 * @note Failure - The string and the value of `matches_found` remain
 *                 unmodified.
 *
 * @param string Pointer to the initialized string.
 * @param start_index Index at which to begin searching. Must be less than or
 *                    equal to the current string length.
 * @param target Pointer to the substring to search for.
 * @param target_length Length of the substring to search for.
 * @param replacement Pointer to the replacement content. May be NULL if
 *                    `replacement_length == 0`.
 * @param replacement_length Length of the replacement content.
 * @param matches_found Pointer to receive the number of matches found. May be
 *                      NULL if the number of matches found is not needed.
 *
 * @return STRING_SUCCESS if the substring is found and replaced at least once.
 * @return STRING_NOT_FOUND if the substring is not found.
 * @return STRING_ERROR_ARGUMENT if `string` is NULL,
 *         or if `start_index > string->length`,
 *         or if `target` is NULL or `target_length == 0`,
 *         or if `replacement` is NULL and `replacement_length != 0`,
 *         or if the required capacity cannot be represented.
 * @return STRING_ERROR_ALLOCATION if memory allocation fails.
 */
StringResult string_replace_all(
    String* string,
    size_t start_index,
    const char* target,
    size_t target_length,
    const char* replacement,
    size_t replacement_length,
    size_t* matches_found
);

#endif
