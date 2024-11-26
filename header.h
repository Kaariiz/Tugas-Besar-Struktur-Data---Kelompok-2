#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <string>
#include <iostream>

using namespace std;

const int MAX_STACK = 100;

typedef string infotype;
typedef struct ElementList *address;
struct infotypeStck{
    address node;
    address beforenode;
    address afternode;
    bool IL, DL;
};

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
    infotypeStck info[MAX_STACK];
    int top;
};

//sub-program yang menggunakan Doubly Linked List
void createList(List &L);
address createElementList(infotype data);
int countElementList(List L);    //digunakan sebagai pengganti index, agar mempermudah implementasi sub-program yang menggunakan parameter position
void insertLine(List &L, int position, address P);
void deleteLine(List &L, int position, address P);
void displayText(List &L);
void navigasiCepat(List &L, int lineNumber);
void copyPaste(List &L, int fromLine, int toLine);
bool findWord(List L, infotype word);
void replaceWord(List &L, infotype oldWord, infotype newWord);

//sub-program yang menggunakan Stack
void createStack(Stack &S);
bool isEmpty(Stack S);
bool isFull(Stack S);
void push(Stack &S, infotypeStck P);
void pop(Stack &S, infotypeStck &operation);
void undo(List &L, Stack &undoStack, Stack &redoStack);
void redo(List &L, Stack &undoStack, Stack &redoStack);

#endif // HEADER_H_INCLUDED
