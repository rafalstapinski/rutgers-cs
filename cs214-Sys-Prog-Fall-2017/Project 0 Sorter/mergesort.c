#include <stdlib.h>
#include "sorter.h"

struct Row *mergesort(struct Row *head)
{
    struct Row *list1 = head;
    if ( (list1 == NULL) || (list1->next == NULL) )
    {
        return list1;
    }

    struct Row *list2 = bisectlist(list1);

    // printf("\nList1 = "); printList(list1);
    // printf("\nList2 = "); printList(list2);

    return mergelist( mergesort(list1), mergesort(list2) );
}

struct Row* mergelist(struct Row *list1, struct Row *list2)
{
    struct Row dummy_head = { 0, NULL }, *tail = &dummy_head;

    while ( (list1 != NULL) && (list2 != NULL) )
    {
        struct Row **min = (*(list1->director_facebook_likes) < *(list2->director_facebook_likes)) ? &list1 : &list2;
        struct Row *next = (*min)->next;
        tail = tail->next = *min;
        *min = next;
    }
    tail->next = list1 ? list1 : list2;
    return dummy_head.next;
}

struct Row *bisectlist(struct Row *head)
{
    /* The fast pointer moves twice as fast as the slow pointer. */
    /* The prev pointer points to the Row preceding the slow pointer. */
    struct Row *fast = head, *slow = head, *prev = NULL;

    while (fast != NULL && fast->next != NULL)
    {
        fast = fast->next->next;
        prev = slow;
        slow = slow->next;
    }

    if (prev != NULL)
    {
        prev->next = NULL;
    }
    return slow;
}
