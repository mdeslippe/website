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
     * Pointer to the previous node in the list, or NULL if this is the first
     * node.
     */
    struct LinkedNode* previous;

    /**
     * Pointer to the next node in the list, or NULL if this is the last node.
     */
    struct LinkedNode* next;

    /**
     * Pointer to user-provided data.
     */
    void* value;
} LinkedNode;

/**
 * @brief A doubly linked list.
 *
 * Represents a mutable doubly linked list with heap-allocated nodes. Nodes are
 * allocated and released as elements are added and removed.
 *
 * @note Ownership - Each LinkedList instance owns its internal nodes and is
 *                   responsible for releasing them when no longer needed. The
 *                   list does not own the data pointed to by the `value` field
 *                   in each `LinkedNode`.
 *
 * @invariant After initialization:
 * - `head == NULL` if and only if `length == 0`
 * - `tail == NULL` if and only if `length == 0`
 * - `head->previous == NULL` when `head != NULL`
 * - `tail->next == NULL` when `tail != NULL`
 * - `head == tail` if and only if `length <= 1`
 * - `length` equals the number of nodes reachable from `head` via `next`
 * - `length` equals the number of nodes reachable from `tail` via `previous`
 */
typedef struct {
    /**
     * Number of nodes in the list.
     */
    size_t length;

    /**
     * Pointer to the first node in the list, or NULL if the list is empty.
     */
    LinkedNode* head;

    /**
     * Pointer to the last node in the list, or NULL if the list is empty.
     */
    LinkedNode* tail;
} LinkedList;

/**
 * @brief Initializes a linked list.
 *
 * Initializes the linked list to a valid, empty state.
 *
 * @note Reinitialization - Calling `linked_list_init` on an already initialized
 *                          linked list does not release previously allocated
 *                          resources. The caller must call `linked_list_free`
 *                          before reinitializing.
 *
 * @param list Pointer to the linked list to initialize.
 *
 * @return LINKED_LIST_SUCCESS on success.
 * @return LINKED_LIST_ERROR_ARGUMENT if `list` is NULL.
 */
LinkedListResult linked_list_init(LinkedList* list);

/**
 * @brief Releases resources owned by a linked list.
 *
 * Releases all nodes and resets the linked list to a zero-initialized state.
 *
 * @note Ownership - The data pointed to by the `value` field in each
 *                   `LinkedNode` is not freed.
 *
 * @note Idempotence - Calling `linked_list_free` on a previously freed or
 *                     zero-initialized linked list is safe.
 *
 * @note Reinitialization - After this call, the linked list may be safely
 *                          reinitialized.
 *
 * @param list Pointer to the linked list to free.
 *
 * @return LINKED_LIST_SUCCESS on success.
 * @return LINKED_LIST_ERROR_ARGUMENT if `list` is NULL.
 */
LinkedListResult linked_list_free(LinkedList* list);

/**
 * @brief Removes all elements from a linked list.
 *
 * Releases all nodes and resets the linked list to a valid, empty state.
 *
 * @pre `list` must be initialized with `linked_list_init`.
 *
 * @note Ownership - The data pointed to by the `value` field in each
 *                   `LinkedNode` is not freed.
 *
 * @param list Pointer to the initialized linked list.
 *
 * @return LINKED_LIST_SUCCESS on success.
 * @return LINKED_LIST_ERROR_ARGUMENT if `list` is NULL.
 */
LinkedListResult linked_list_clear(LinkedList* list);

/**
 * @brief Inserts an element at the front of a linked list.
 *
 * Allocates a new node containing `value` and inserts it at the front of the
 * linked list.
 *
 * @pre `list` must be initialized with `linked_list_init`.
 *
 * @note Ownership - The linked list does not take ownership of the data pointed
 *                   to by `value`. The caller is responsible for managing its 
 *                   lifetime.
 *
 * @note Success - The new node becomes the head of the linked list.
 *
 * @note Failure - The linked list remains unmodified.
 *
 * @param list Pointer to the initialized linked list.
 * @param value Pointer to the user-provided data to store. May be NULL.
 *
 * @return LINKED_LIST_SUCCESS on success.
 * @return LINKED_LIST_ERROR_ARGUMENT if `list` is NULL.
 * @return LINKED_LIST_ERROR_ALLOCATION if memory allocation fails.
 */
LinkedListResult linked_list_push_front(LinkedList* list, void* value);

/**
 * @brief Inserts an element at the back of a linked list.
 *
 * Allocates a new node containing `value` and inserts it at the back of the
 * linked list.
 *
 * @pre `list` must be initialized with `linked_list_init`.
 *
 * @note Ownership - The linked list does not take ownership of the data pointed
 *                   to by `value`. The caller is responsible for managing its 
 *                   lifetime.
 *
 * @note Success - The new node becomes the tail of the linked list.
 *
 * @note Failure - The linked list remains unmodified.
 *
 * @param list Pointer to the initialized linked list.
 * @param value Pointer to the user-provided data to store. May be NULL.
 *
 * @return LINKED_LIST_SUCCESS on success.
 * @return LINKED_LIST_ERROR_ARGUMENT if `list` is NULL.
 * @return LINKED_LIST_ERROR_ALLOCATION if memory allocation fails.
 */
LinkedListResult linked_list_push_back(LinkedList* list, void* value);

#endif
