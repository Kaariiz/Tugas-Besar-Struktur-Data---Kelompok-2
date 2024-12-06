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

struct Operation {
    string action;  // "insert", "delete", "copyPaste", "replace"
    int line;
    int position;
    infotype data;    // Data yang terlibat dalam operasi
    infotype oldData; // Data yang lama (untuk undo pada replace atau copyPaste)
    int srcLine;      // Untuk operasi copyPaste: baris sumber
    int srcPos;       // Untuk operasi copyPaste: posisi sumber
    int destLine;     // Untuk operasi copyPaste: baris tujuan
    int destPos;      // Untuk operasi copyPaste: posisi tujuan
};

struct OperationStack {
    Operation operations[MAX_STACK];
    int top;
};

// Subprogram untuk Doubly Linked List
void createList(List &L);
address createElementList(infotype data);
void insertText(List &L, int line, int position, infotype &data, OperationStack &undoredo);
void deleteWord(List &L, int line, int position, OperationStack &undoredo);
void displayText(List &L);
void findWord(List L, infotype word);
void replaceWord(List &L, infotype oldWord, infotype newWord, int line, int position, OperationStack &undoredo);
void copyPaste(List &L, int lineSrc, int posSrc, int lineDest, int posDest, OperationStack &undoredo);


// Subprogram untuk Stack
void createStack(OperationStack &S);
bool isEmpty(OperationStack S);
bool isFull(OperationStack S);
void push(OperationStack &S, Operation op);
void pop(OperationStack &S, Operation &op);
void clearRedo(OperationStack &redow);

// Subprogram untuk Undo/Redo
void undo(List &L, OperationStack &undoStack, OperationStack &redoStack);
void redo(List &L, OperationStack &undoStack, OperationStack &redoStack);

#endif // HEADER_H_INCLUDED
