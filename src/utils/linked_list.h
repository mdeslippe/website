#ifndef LINKED_LIST_H
#define LINKED_LIST_H

/**
 * @brief Result codes returned by linked list operations.
 *
 * Defines the possible outcomes of functions operating on the linked list type.
 * Functions return one of these values to indicate success, a non-success
 * condition such as a value not being found, or the type of failure.
 *
 * @note Usage - A return value of LINKED_LIST_SUCCESS indicates success. Any
 *               other value indicates a non-success condition, which may be a
 *               normal outcome (such as LINKED_LIST_NOT_FOUND) or an error.
 */
typedef enum {
    /**
     * Operation completed successfully.
     */
    LINKED_LIST_SUCCESS = 0,

    /**
     * Target value was not found.
     */
    LINKED_LIST_NOT_FOUND = 1,

    /**
     * Invalid argument was provided.
     */
    LINKED_LIST_ERROR_ARGUMENT = 2,

    /**
     * Memory allocation failed.
     */
    LINKED_LIST_ERROR_ALLOCATION = 3
} LinkedListResult;

#endif
