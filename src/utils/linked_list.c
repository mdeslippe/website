#include <assert.h>
#include <stdlib.h>
#include "linked_list.h"

/**
 * @brief Asserts that a linked list is valid.
 *
 * This macro is intended for internal debug assertions to verify that a linked
 * list satisfies all required invariants.
 *
 * A valid linked list must satisfy:
 * - `list != NULL`
 * - `list->head == NULL` if and only if `list->length == 0`
 * - `list->tail == NULL` if and only if `list->length == 0`
 * - `list->head->previous == NULL` when `list->head != NULL`
 * - `list->tail->next == NULL` when `list->tail != NULL`
 * - `list->head == list->tail` if and only if `list->length <= 1`
 *
 * @remark A zero-initialized linked list is considered valid.
 */
#define LINKED_LIST_ASSERT_VALID(list)                              \
    assert(                                                         \
        (list) != NULL &&                                           \
        (((list)->head == NULL) == ((list)->length == 0)) &&        \
        (((list)->tail == NULL) == ((list)->length == 0)) &&        \
        ((list)->head == NULL || (list)->head->previous == NULL) && \
        ((list)->tail == NULL || (list)->tail->next == NULL) &&     \
        (((list)->head == (list)->tail) == ((list)->length <= 1))   \
    )

LinkedListResult linked_list_init(LinkedList* list) {

    if (list == NULL) {
        return LINKED_LIST_ERROR_ARGUMENT;
    }

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    LINKED_LIST_ASSERT_VALID(list);

    return LINKED_LIST_SUCCESS;

}

LinkedListResult linked_list_free(LinkedList* list) {

    if (list == NULL) {
        return LINKED_LIST_ERROR_ARGUMENT;
    }

    LINKED_LIST_ASSERT_VALID(list);

    LinkedNode* current = list->head;
    LinkedNode* next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    LINKED_LIST_ASSERT_VALID(list);

    return LINKED_LIST_SUCCESS;

}

LinkedListResult linked_list_clear(LinkedList* list) {

    if (list == NULL) {
        return LINKED_LIST_ERROR_ARGUMENT;
    }

    LINKED_LIST_ASSERT_VALID(list);

    LinkedNode* current = list->head;
    LinkedNode* next = NULL;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }

    list->length = 0;
    list->head = NULL;
    list->tail = NULL;

    LINKED_LIST_ASSERT_VALID(list);

    return LINKED_LIST_SUCCESS;

}

LinkedListResult linked_list_push_front(LinkedList* list, void* value) {

    if (list == NULL) {
        return LINKED_LIST_ERROR_ARGUMENT;
    }

    LINKED_LIST_ASSERT_VALID(list);

    LinkedNode* node = malloc(sizeof(LinkedNode));

    if (node == NULL) {
        return LINKED_LIST_ERROR_ALLOCATION;
    }

    node->previous = NULL;
    node->next = list->head;
    node->value = value;

    list->head = node;
    list->length += 1;

    if (node->next == NULL) {
        list->tail = node;
    } else {
        node->next->previous = node;
    }

    LINKED_LIST_ASSERT_VALID(list);

    return LINKED_LIST_SUCCESS;

}

LinkedListResult linked_list_push_back(LinkedList* list, void* value) {

    if (list == NULL) {
        return LINKED_LIST_ERROR_ARGUMENT;
    }

    LINKED_LIST_ASSERT_VALID(list);

    LinkedNode* node = malloc(sizeof(LinkedNode));

    if (node == NULL) {
        return LINKED_LIST_ERROR_ALLOCATION;
    }

    node->previous = list->tail;
    node->next = NULL;
    node->value = value;

    list->tail = node;
    list->length += 1;

    if (node->previous == NULL) {
        list->head = node;
    } else {
        node->previous->next = node;
    }

    LINKED_LIST_ASSERT_VALID(list);

    return LINKED_LIST_SUCCESS;

}
