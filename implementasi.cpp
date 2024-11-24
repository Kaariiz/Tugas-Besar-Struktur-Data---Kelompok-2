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

int countElementList(List L) {
    address temp;
    temp = L.first;
    int Count;
    Count = 0;

    while (temp != nullptr) {
        Count += 1;
        temp = temp->next;
    }

    return Count;
}

void deleteLine(List &L, int position, address P) {
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
                L.first = nullptr;
                L.last = nullptr;
            } else {
                P = L.first;
                L.first = P->next;
                P->next = nullptr;
                L.first->prev = nullptr;
            }

        } else if (position == totalElement) {
            if (L.first->next == nullptr) {
                P = L.first;
                L.first = nullptr;
                L.last = nullptr;
            } else {
                P = L.last;
                L.last = L.last->prev;
                P->prev = nullptr;
                L.last->next = nullptr;
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

            address Q;
            Q = tempL->prev;
            P = tempL;

            Q->next = P->next;
            P->next->prev = Q;
            P->next = nullptr;
            P->prev = nullptr;
        }

        cout << "Text dengan posisi tersebut telah berhasil dihapus!" << endl;
        cout << "Text yang dihapus adalah: " << P->info << endl;
    }
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





// BATAS IMPLEMENTASI BY RIZKA ANANDA PRATAMA
