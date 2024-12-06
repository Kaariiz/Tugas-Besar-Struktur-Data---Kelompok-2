#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

#include <string>
#include <iostream>

using namespace std;

const int MAX_STACK = 100;
typedef string infotype;

// Node kata (Word)
struct WordNode {
    infotype info;
    WordNode *prev;
    WordNode *next;
};

// Node baris (Line)
struct LineNode {
    LineNode *prev;
    LineNode *next;
    WordNode *firstWord;
    WordNode *lastWord;
};

// Dokumen yang berisi banyak baris
struct Document {
    LineNode *firstLine;
    LineNode *lastLine;
};

struct Operation {
    string action;  // "insert", "delete", "copyPaste", "replace"
    int line;
    int position;
    infotype data;    // Data yang terlibat dalam operasi
    infotype oldData; // Data lama (untuk undo replace atau copyPaste)
    int srcLine;      // Untuk copyPaste: baris sumber
    int srcPos;       // Untuk copyPaste: posisi sumber
    int destLine;     // Untuk copyPaste: baris tujuan
    int destPos;      // Untuk copyPaste: posisi tujuan
};

struct OperationStack {
    Operation operations[MAX_STACK];
    int top;
};

// Fungsi dasar MLL
void createDocument(Document &D);
LineNode* createLine();
WordNode* createWord(infotype data);

LineNode* getLine(Document &D, int lineNumber, bool createIfNotExist = false);
WordNode* getWord(LineNode *L, int position);

void insertText(Document &D, int line, int position, infotype &data, OperationStack &undoStack);
void deleteWord(Document &D, int line, int position, OperationStack &undoStack);
void displayText(Document &D);
void findWord(Document &D, infotype word);
void replaceWord(Document &D, infotype oldWord, infotype newWord, int line, int position, OperationStack &undoStack);
void copyPaste(Document &D, int lineSrc, int posSrc, int lineDest, int posDest, OperationStack &undoStack);

// Stack
void createStack(OperationStack &S);
bool isEmpty(OperationStack S);
bool isFull(OperationStack S);
void push(OperationStack &S, Operation op);
void pop(OperationStack &S, Operation &op);
void clearRedo(OperationStack &redoStack);

// Undo/Redo
void undo(Document &D, OperationStack &undoStack, OperationStack &redoStack);
void redo(Document &D, OperationStack &undoStack, OperationStack &redoStack);

// Fungsi bantu
int countWords(LineNode *L);
int countLines(Document &D);

#endif // HEADER_H_INCLUDED
