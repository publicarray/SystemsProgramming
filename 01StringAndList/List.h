#ifndef LIST_H
#define LIST_H

typedef struct List_Struct {
    int length;
    int bufferLength;
    void* * data;
    // struct List_Struct * root // to refer to left or right
} List;

void initList(List * self); // initialises the list
void* addList(List* self, void* data, int numberOfObjects); // adds ptr to list
void* getList(List* self, int index); // gets the item in the list
void* * getListPtr(List* self, int index); // gets the list
// void setList(List* self); // deletes the list
void freeList(List* self); // deletes the list

#endif
