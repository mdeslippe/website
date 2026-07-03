#ifndef LINKED_LIST_H
#define LINKED_LIST_H

#include <stddef.h>

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

/**
 * @brief A node in a doubly linked list.
 *
 * Represents a single element in a doubly linked list, containing a pointer to
 * user-provided data and links to adjacent nodes.
 *
 * @note Ownership - The node does not own the data pointed to by `value`. The
 *                   caller is responsible for managing the lifetime of the
 *                   data.
 *
 * @invariant While linked into a list:
 * - `previous == NULL` if and only if this is the first node
 * - `next == NULL` if and only if this is the last node
 * - `previous->next == this` when `previous != NULL`
 * - `next->previous == this` when `next != NULL`
 */
typedef struct LinkedNode {
    /**
     * Pointer to user-provided data.
     */
    void* value;

    /**
     * Pointer to the previous node in the list, or NULL if this is the first
     * node.
     */
    struct LinkedNode* previous;

    /**
     * Pointer to the next node in the list, or NULL if this is the last node.
     */
    struct LinkedNode* next;
} LinkedNode;

#endif
