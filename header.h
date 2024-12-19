#ifndef HEADER_H_INCLUDED
#define HEADER_H_INCLUDED

using namespace std;

typedef string infoChild;
typedef int infoParent;

typedef struct ElmParent *adrParent;
typedef struct ElmChild *adrChild;

struct ElmParent {
    infoParent info;
    adrParent next, prev;
    adrChild child;
};

struct ElmChild {
    infoChild info;
    adrChild next, prev;
};

struct List {
    adrParent first, last;
};

struct Operation {
    string type;         // "insert", "delete", "replace"
    int line;           // Line affected
    int position;       // Position in the line (if applicable)
    string oldData;     // Data before the operation (for undo)
    string newData;     // Data after the operation (for redo)
};

struct OperationNode {
    Operation data;
    OperationNode* next;
};

struct OperationStack {
    OperationNode* top;
};

void createStack(OperationStack &S);
bool isEmpty(OperationStack S);
void push(OperationStack &S, Operation op);
Operation pop(OperationStack &S);

void createListBaris(List &L);
adrParent createElmBaris(infoParent data);
void deleteBaris(List &L, int baris, OperationStack &undoStack, bool addToStack);
void insertBaris(List &L, int baris, OperationStack &undoStack, bool addToStack);
void insertKata(List &L, int baris, int posisi, string kata, OperationStack &undoStack, bool addToStack);
void replaceKata(List &L, string kataLama, string kataBaru, OperationStack &undoStack, bool addToStack);
void findKata(List L, string kata);    // updated
void showAll(List L);   // updated
void showPerbaris(List L, int baris); // updated
void undo(List &L, OperationStack &undoStack, OperationStack &redoStack);
void redo(List &L, OperationStack &undoStack, OperationStack &redoStack);
void clearRedo(OperationStack &redoStack);

void clearScreen();

#endif // HEADER_H_INCLUDED
