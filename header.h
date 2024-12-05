#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <string>
#include <iostream>

using namespace std;

const int MAX_STACK = 100;

typedef string infotype;
typedef struct ElementList *address;

struct ElementList {
    infotype info;
    address prev;
    address next;
};

struct List {
    address first;
    address last;
};

struct Stack {
    infotype info[MAX_STACK];
    int top;
};

//sub-program yang menggunakan Doubly Linked List
void createList(List &L);
address createElementList(infotype data);
void insertText(List &L, int line, int position, infotype &data); //upadated
void deleteLine(List &L, int lineNumber); //updated
void displayText(List &L); // updated
void navigasiCepat(List &L, int lineNumber);
void copyPasteWord(List &L, int sourceLine, int sourcePosition, int targetLine, int targetPosition); //updated
void findWord(List L, infotype word); // updated
void replaceWord(List &L, infotype oldWord, infotype newWord); //updated

//sub-program yang menggunakan Stack
void createStack(Stack &S);
bool isEmpty(Stack S);
bool isFull(Stack S);
void push(Stack &S, infotype P);
void pop(Stack &S, infotype &operation);
void undo(List &L, Stack &undoStack, Stack &redoStack);
void redo(List &L, Stack &undoStack, Stack &redoStack);

#endif // HEADER_H_INCLUDED
