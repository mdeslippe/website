#include <assert.h>
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
 * - `list->head == list->tail` when `list->length == 1`
 */
#define LINKED_LIST_ASSERT_VALID(list)                              \
    assert(                                                         \
        (list) != NULL &&                                           \
        (((list)->head == NULL) == ((list)->length == 0)) &&        \
        (((list)->tail == NULL) == ((list)->length == 0)) &&        \
        ((list)->head == NULL || (list)->head->previous == NULL) && \
        ((list)->tail == NULL || (list)->tail->next == NULL) &&     \
        ((list)->length != 1 || (list)->head == (list)->tail)       \
    )
