#ifndef LIST_H
#define LIST_H

/*
    struct page *p1 = NULL;
    struct page *p2 = NULL;
    struct page *p3 = NULL;
    struct page *current = NULL;
    LIST_HEAD(page_list_type, page) page_list = LIST_HEAD_INITIALIZER(page_list);

    LIST_INIT(&page_list);

    p1 = malloc(sizeof(struct page));
    p1->physaddr = (void*) 0x1337babe;
    p1->virtaddr = (void*) 0xcafedead;
    LIST_INSERT_HEAD(&page_list, p1, list);

    p2 = malloc(sizeof(struct page));
    p2->physaddr = (void*) 0x1000;
    p2->virtaddr = (void*) 0x1337babe;
    LIST_INSERT_AFTER(p1, p2, list);

    p3 = malloc(sizeof(struct page));
    p3->physaddr = (void*) 0x1;
    p3->virtaddr = (void*) 0x2;
    LIST_INSERT_AFTER(p1, p3, list);

    LIST_REMOVE(&page_list, p3, page, list);

    LIST_FOREACH(current, &page_list, list)
    {
        printf("%p -> %p\n", current->virtaddr, current->physaddr);
    }

    free(p1);
    free(p2);
    free(p3);
*/

// Declarations
#define LIST_HEAD(headname, type)       \
    struct headname                     \
    {                                   \
        struct type *first;             \
        struct type *last;              \
    }

#define LIST_HEAD_INITIALIZER(head) { 0, 0 }

#define LIST_ENTRY(type)    \
    struct                  \
    {                       \
        struct type *next;  \
    }

// Functions
#define LIST_EMPTY(head) ((head)->first == 0)
#define LIST_FIRST(head) ((head)->first)
#define LIST_LAST(head) ((head)->last)

#define LIST_INIT(head)     \
    LIST_FIRST((head)) = 0; \
    LIST_LAST((head)) = 0

#define LIST_NEXT(element, field) (element->field.next)

#define LIST_CONCAT(head1, head2, field)                            \
    if(!LIST_EMPTY(head2))                                          \
    {                                                               \
        LIST_NEXT(LIST_LAST((head1)), field) = LIST_FIRST((head2)); \
        LIST_LAST((head1)) = LIST_LAST((head2));                    \
    }                                                               \
    LIST_INIT((head2))

#define LIST_FOREACH(var, head, field)      \
    for((var) = LIST_FIRST((head));         \
        (var);                              \
        (var) = LIST_NEXT((var), field))

#define LIST_FOREACH_FROM(var, head, field)             \
    for((var) = ((var) ? (var) : LIST_FIRST((head)));   \
        (var);                                          \
        (var) = LIST_NEXT((var), field))

#define LIST_INSERT_HEAD(head, element, field)                  \
    if((LIST_NEXT((element), field) = LIST_FIRST((head))) == 0) \
    {                                                           \
        LIST_LAST((head)) = (element);                          \
    }                                                           \
    LIST_FIRST((head)) = (element)

#define LIST_INSERT_AFTER(head, helement, element, field)                   \
    if((LIST_NEXT((element), field) = LIST_NEXT((helement), field)) == 0)   \
    {                                                                       \
        LIST_LAST((head)) = (element);                                      \
    }                                                                       \
    LIST_NEXT((helement), field) = (element)

#define LIST_INSERT_TAIL(head, element, field)          \
    LIST_NEXT((element), field) = 0;                    \
    LIST_NEXT((LIST_LAST((head))), field) = element;    \
    LIST_LAST((head)) = element

#define LIST_REMOVE_HEAD(head, field)                                                           \
    if(!LIST_EMPTY((head)) && (LIST_FIRST((head)) = LIST_NEXT(LIST_FIRST((head)), field)) == 0) \
    {                                                                                           \
        LIST_LAST((head)) = 0;                                                                  \
    }

#define LIST_REMOVE_AFTER(head, element, field)                                       \
    if((LIST_NEXT((element), field) = LIST_NEXT(LIST_NEXT((element), field), field)) == 0) \
    {\
        LIST_LAST((head)) = element; \
    }

#define LIST_REMOVE(head, element, type, field)                     \
    if(LIST_FIRST((head)) == ((element)))                           \
    {                                                               \
        LIST_REMOVE_HEAD((head), field);                            \
    } else {                                                        \
        struct type *current_element = LIST_FIRST((head));          \
        while(LIST_NEXT(current_element, field) != (element))       \
        {                                                           \
            current_element = LIST_NEXT(current_element, field);    \
        }                                                           \
        LIST_REMOVE_AFTER(head, current_element, field);            \
    }

#endif