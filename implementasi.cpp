#include "header.h"
#include <iostream>

using namespace std;

void createList(List &L) {
    L.first = nullptr;
    L.last = nullptr;
}

address createElementList(infotype data) {
    address P;

    P = new ElementList;
    P->info = data;
    P->next = nullptr;
    P->prev = nullptr;

    return P;
}

/*int countElementList(List L) {
    address temp;
    temp = L.first;
    int Count;
    Count = 0;

    while (temp != nullptr) {
        Count += 1;
        temp = temp->next;
    }

    return Count;
}*/

void deleteLine(List &L, int position, address P, Stack &Undo) {
    infotypeStck Q;
    int totalElement;
    totalElement = countElementList(L);

    if (totalElement == 0) {
        cout << "Tidak ada text yang tersimpan, tidak dapat menggunakan fitur Delete!" << endl;

    } else if (position > totalElement) {
        cout << "Posisi melebihi jumlah baris!" << endl;   //kalo posisi berlebih, looping input di main aja

    } else {
        if (position == 1) {
            if (L.first->next == nullptr) {
                P = L.first;
                Q.node = P;
                Q.beforenode = P->prev;
                Q.afternode = P->next;
                L.first = nullptr; 
                L.last = nullptr;
                Q.DL = true;
                Q.IL = false;
            } else {
                P = L.first;
                L.first = P->next;
                Q.node = P;
                Q.beforenode = P->prev;
                Q.afternode = P->next;
                P->next = nullptr;
                L.first->prev = nullptr;
                Q.DL = true;
                Q.IL = false;
            }

        } else if (position == totalElement) {
            if (L.first->next == nullptr) {
                P = L.first;
                Q.node = P;
                Q.beforenode = P->prev;
                Q.afternode = P->next;
                L.first = nullptr;
                L.last = nullptr;
                Q.DL = true;
                Q.IL = false;
            } else {
                P = L.last;
                L.last = L.last->prev;
                Q.node = P;
                Q.beforenode = P->prev;
                Q.afternode = P->next;
                P->prev = nullptr;
                L.last->next = nullptr;
                Q.DL = true;
                Q.IL = false;
            }

        } else {
            address tempL;
            int tempC;
            tempL = L.first;
            tempC = 0;

            while (tempC != position) {
                tempC += 1;
                tempL = tempL->next;
            }

            P = tempL;

            P->prev->next = P->next;
            P->next->prev = P->prev;
            Q.node = P;
            Q.beforenode = P->prev;
            Q.afternode = P->next;
            P->next = nullptr;
            P->prev = nullptr;
            Q.DL = true;
            Q.IL = false;
        }

        cout << "Text dengan posisi tersebut telah berhasil dihapus!" << endl;
        cout << "Text yang dihapus adalah: " << P->info << endl;
    }
    push(Undo,Q);
    delete P;
}

void displayText(List &L) {
    address temp;
    temp = L.first;

    if (temp == nullptr) {
        cout << "Tidak ada text yang tersimpan!" << endl;
    } else {
        cout << "Text yang tersimpan adalah: " << endl;
        while (temp != nullptr) {
            cout << temp->info << ", ";
            temp = temp->next;
        }
    }

    cout << endl;
}

// void copyPaste(List &L, int fromLine, int toLine) {
//     // validasi posisi baris yang diminta
//     if (fromLine < 1 || fromLine > countElementList(L) || toLine < 1 || toLine > countElementList(L) + 1) {
//         cout << "Nomor baris tidak valid!" << endl;
//         return;
//     } 

//     // navigasi ke baris yang akan disalin
//     address temp = L.first;
//     for (int i = 1; i < fromLine; i++) {
//         temp = temp->next;
//     }

//     // salin isi dari fromLine ke toLine
//     if (temp != nullptr) {
//         insertLine(L, toLine, temp->info);
//     }
// }


// BATAS IMPLEMENTASI BY RIZKA ANANDA PRATAMA

void insertLine(List &L, int position, int CL, int Line, infotype data, Stack &Undo, Stack &Redo) {
    infotypeStck Q;
    address P = createElementList(data);
    if 
    if (position <= 0 || position > countElementList(L) + 1) {
        cout << "Invalid position!\n";
    } else if (L.first == nullptr) {
        L.first = P;
        L.last = P;
    } else if (position == 1) {
        P->next = L.first;
        L.first->prev = P;
        L.first = P;
    } else if (position == countElementList(L) + 1) {
        P->prev = L.last;
        L.last->next = P;
        L.last = P;
    } else {
        address Q = L.first;
        for (int i = 1; i <= position - 1; i++) {
            Q = Q->next;
        }
        P->next = Q->next;
        P->prev = Q;
        Q->next->prev = P;
        Q->next = P;
    }
    Q.node = P;
    Q.beforenode = P->prev;
    Q.afternode = P->next;
    Q.DL = false;
    Q.IL = true;
    push(Undo,Q);
    clearRedo(Redo);
}

void createStack(Stack &S){
    S.top = 0;
}

bool isEmpty(Stack S){
    return S.top == 0;
}

bool isFull(Stack S){
    return S.top == MAX_STACK;
}

void push(Stack &S, infotypeStck P){
    if (!isFull(S)){
        S.top++;
        S.info[S.top] = P;
    } else {
        cout << "Stack Penuh";
    }
}

void pop(Stack &S, infotypeStck &operation){
    if (!isEmpty(S)){
        S.info[S.top] = operation;
        S.top--;
    } else {
        cout << "Stack Kosong";
    }
}

void undo(List &L, Stack &undoStack, Stack &redoStack){
    infotypeStck Q;
    address temp1 = L.first; 
    if (!isEmpty(undoStack)){
        pop(undoStack, Q);
        if (Q.IL){
            while(temp1 != nullptr && temp1->info != Q.node->info){
                temp1 = temp1 ->next;
            }
            if (temp1 ->next == nullptr && temp1 ->prev == nullptr){
                L.first = nullptr;
                L.last = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(redoStack, Q);
                delete temp1;
            } else if(temp1 ->next == nullptr) {
                L.last = temp1 ->prev;
                L.last ->next = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(redoStack, Q);
                delete temp1;
            } else if(temp1 ->prev == nullptr){
                L.first = temp1 ->next;
                L.first ->prev = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(redoStack, Q);
                delete temp1;
            } else {
                temp1->prev->next = temp1 ->next;
                temp1->next->prev = temp1 ->prev;
                temp1 ->next = nullptr;
                temp1 ->prev = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(redoStack, Q);
                delete temp1;
            }
        } else if (Q.DL){
            if (Q.beforenode == nullptr && Q.afternode == nullptr){
                duplicatenode(temp1, Q.node);
                L.first = temp1;
                L.last = temp1;
                Q.DL = false;
                Q.IL = true;
                push(redoStack, Q);
            } else if(Q.afternode == nullptr) {
                duplicatenode(temp1, Q.node);
                temp1 ->prev = L.last;
                L.last ->next = temp1;
                L.last = temp1;
                Q.DL = false;
                Q.IL = true;
                push(redoStack, Q);
            } else if(Q.beforenode == nullptr){
                duplicatenode(temp1, Q.node);
                temp1 ->next = L.first;
                L.first ->prev = temp1;
                Q.DL = false;
                Q.IL = true;
                push(redoStack, Q);
            } else {
                while (temp1 != nullptr && temp1 ->info != Q.beforenode ->info){
                    temp1 = temp1 ->next;
                }
                address temp2 = nullptr;
                duplicatenode(temp2, Q.node);
                temp1->next->prev = temp2;
                temp2->next = temp1 ->next;
                temp1 ->next = temp2;
                temp2 ->prev = temp1;
                Q.DL = false;
                Q.IL = true;
                push(redoStack, Q);
            }
        }
    } else {
        cout << "Belum terdapat perubahan"<< endl;
    }
}

void redo(List &L, Stack &undoStack, Stack &redoStack){
    infotypeStck Q;
    address temp1 = L.first;
    if (!isEmpty(redoStack)){
        pop(redoStack, Q);
        if (Q.IL){
            while(temp1 != nullptr && temp1->info != Q.node->info){
                temp1 = temp1 ->next;
            }
            if (temp1 ->next == nullptr && temp1 ->prev == nullptr){
                L.first = nullptr;
                L.last = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(undoStack, Q);
                delete temp1;
            } else if(temp1 ->next == nullptr) {
                L.last = temp1 ->prev;
                L.last ->next = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(undoStack, Q);
                delete temp1;
            } else if(temp1 ->prev == nullptr){
                L.first = temp1 ->next;
                L.first ->prev = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(undoStack, Q);
                delete temp1;
            } else {
                temp1->prev->next = temp1 ->next;
                temp1->next->prev = temp1 ->prev;
                temp1 ->next = nullptr;
                temp1 ->prev = nullptr;
                Q.DL = true;
                Q.IL = false;
                push(undoStack, Q);
                delete temp1;
            }
        } else if (Q.DL){
            if (Q.beforenode == nullptr && Q.afternode == nullptr){
                duplicatenode(temp1, Q.node);
                L.first = temp1;
                L.last = temp1;
                Q.DL = false;
                Q.IL = true;
                push(undoStack, Q);
            } else if(Q.afternode == nullptr) {
                duplicatenode(temp1, Q.node);
                temp1 ->prev = L.last;
                L.last ->next = temp1;
                L.last = temp1;
                Q.DL = false;
                Q.IL = true;
                push(undoStack, Q);
            } else if(Q.beforenode == nullptr){
                duplicatenode(temp1, Q.node);
                temp1 ->next = L.first;
                L.first ->prev = temp1;
                Q.DL = false;
                Q.IL = true;
                push(undoStack, Q);
            } else {
                while (temp1 != nullptr && temp1 ->info != Q.beforenode ->info){
                    temp1 = temp1 ->next;
                }
                address temp2 = nullptr;
                duplicatenode(temp2, Q.node);
                temp1->next->prev = temp2;
                temp2->next = temp1 ->next;
                temp1 ->next = temp2;
                temp2 ->prev = temp1;
                Q.DL = false;
                Q.IL = true;
                push(undoStack, Q);
            }
        }
    } else {
        cout << "Terdeteksi belum melakukan Undo"<< endl;
    }
}

void duplicatenode(address &P, address S){
    P ->info = S->info;
    P ->next = S->next;
    P ->prev = S->prev;
} 

void clearRedo(Stack &P){
    infotypeStck Q;
    while (!isEmpty(P)){
        pop(P, Q);
    }
}
