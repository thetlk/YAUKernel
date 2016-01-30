#ifndef LIST_H
#define LIST_H

/*
    struct page *p1, *p2;
    LIST_HEAD(page_list_type, page) page_list = LIST_HEAD_INITIALIZER(page_list);

    LIST_INIT(&page_list);

    p1 = malloc(sizeof(struct page));
    p1->physaddr = (void*) 0x1337babe;
    p1->virtaddr = (void*) 0xcafedead;
    LIST_INSERT_HEAD(&page_list, p1, list);

    p2 = malloc(sizeof(struct page));
    p2->physaddr = (void*) 0x0;
    p2->virtaddr = (void*) 0x1337babe;
    LIST_INSERT_AFTER(p1, p2, list);

    LIST_REMOVE_HEAD(&page_list, list);
    free(p1);
*/

#define LIST_FIRST(head) ((head)->first)
#define LIST_EMPTY(head) ((head)->first == 0)

#define LIST_HEAD(headname, type)       \
    struct headname                     \
    {                                   \
        struct type *first;             \
    }

#define LIST_HEAD_INITIALIZER(head) { 0 }

#define LIST_ENTRY(type)    \
    struct                  \
    {                       \
        struct type *next;  \
    }

#define LIST_INIT(head)     \
    LIST_FIRST(head) = 0

#define LIST_NEXT(element, field) (element->field.next)

#define LIST_INSERT_HEAD(head, element, field)          \
    LIST_NEXT((element), field) = LIST_FIRST((head));   \
    LIST_FIRST((head)) = element

#define LIST_INSERT_AFTER(helement, element, field)             \
    LIST_NEXT((element), field) = LIST_NEXT((helement), field); \
    LIST_NEXT((helement), field) = (element)

#define LIST_REMOVE_HEAD(head, field)                           \
    LIST_FIRST((head)) = LIST_NEXT(LIST_FIRST((head)), field)

#endif