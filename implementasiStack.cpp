#include "header.h"
#include <iostream>

using namespace std;

void createStack(Stack &S){
    S.top = 0;
}

bool isEmpty(Stack S){
    return S.top == 0;
}

bool isFull(Stack S){
    return S.top == MAX_STACK;
}

void push(Stack &S, infotype P){
    if (!isFull(S)){
        S.top++;
        S.info[S.top] = P;
    } else {
        cout << "Stack Penuh";
    }
}

void pop(Stack &S, infotype &operation){
    if (!isEmpty(S)){
        S.info[S.top] = operation;
        S.top--;
    } else {
        cout << "Stack Kosong";
    }
}

void undo(List &L, Stack &undoStack, Stack &redoStack, bool IL, bool DL, address P, address Q){
    if (IL){
        
    } else if (DL){

    }
}

void redo(List &L, Stack &undoStack, Stack &redoStack){
    
}