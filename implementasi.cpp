#include <iostream>
#include "header.h"
#include <cstdlib>

using namespace std;

void createStack(OperationStack &S) {
    S.top = nullptr;
}

bool isEmpty(OperationStack S) {
    return S.top == nullptr;
}

void push(OperationStack &S, Operation op) {
    OperationNode* newNode = new OperationNode;
    newNode->data = op;
    newNode->next = S.top;
    S.top = newNode;
}

Operation pop(OperationStack &S) {
    if (isEmpty(S)) {
        return {"", -1, -1, "", ""};
    } else {
        OperationNode* temp = S.top;
        Operation op = temp->data;
        S.top = temp->next;
        return op;
    }
}


void createListBaris(List &L) {
    L.first = nullptr;
    L.last = nullptr;
}

adrParent createElmBaris(infoParent data) {
    adrParent P;

    P = new ElmParent;

    P->info = data;
    P->next = nullptr;
    P->prev = nullptr;
    P->child = nullptr;

    return P;
}

void insertBaris(List &L, int baris, OperationStack &undoStack, bool addToStack) {
    adrParent P;

    P = new ElmParent;

    P->info = baris;
    P->next = nullptr;
    P->prev = nullptr;
    P->child = nullptr;

    if (baris < 1) {
        cout << "Inputan baris tidak valid!" << endl;
    } else {
        if (L.first == nullptr) { // Jika list masih kosong
            P->info = 1; // Pastikan baris pertama adalah 1
            L.first = P;
            L.last = P;

            if (baris > 1) {
                cout << "Dokumen masih kosong, sehingga baris pertama ditambahkan secara otomatis.";

            } else {
                cout << "Baris pertama berhasil ditambahkan!" << endl;
            }

        } else {
            int hitung = 0;
            adrParent temp = L.first;

            // Hitung jumlah elemen dalam list
            while (temp != nullptr) {
                hitung++;
                temp = temp->next;
            }

            temp = L.first; // Reset ke elemen pertama untuk proses penambahan

            if (baris > hitung + 1) { // Jika baris melebihi jumlah baris + 1
                P->info = hitung + 1;
                L.last->next = P;
                P->prev = L.last;
                L.last = P;
                cout << "Baris ke-" << baris << " terlalu jauh dari jumlah baris saat ini (" << hitung << " baris)" << endl;
                cout << "Baris ke-" << hitung + 1 << " ditambahkan secara otomatis!" << endl;
                baris = hitung + 1;
            } else if (baris == hitung + 1) { // Jika baris adalah baris berikutnya
                L.last->next = P;
                P->prev = L.last;
                L.last = P;

                cout << "Baris ke-" << baris << " berhasil ditambahkan!" << endl;
            } else { // Jika baris berada di tengah atau sudah ada
                bool sudahAda = false;
                while (temp != nullptr) {
                    if (temp->info == baris) {
                        sudahAda = true;
                    }
                    temp = temp->next;
                }

                temp = L.first; // Reset ke elemen pertama lagi
                if (sudahAda) {
                    cout << "Baris ke-" << baris << " sudah ada atau tidak valid untuk ditambahkan!" << endl;
                } else { // Baris valid dan di tengah
                    int posisi = 1;
                    while (temp != nullptr && posisi < baris - 1) {
                        temp = temp->next;
                        posisi++;
                    }

                    // Sisipkan elemen di posisi yang sesuai
                    if (temp != nullptr) {
                        P->next = temp->next;
                        P->prev = temp;

                        if (temp->next != nullptr) {
                            temp->next->prev = P;
                        }

                        temp->next = P;

                        if (P->next == nullptr) {
                            L.last = P; // Perbarui pointer last jika perlu
                        }

                        cout << "Baris ke-" << baris << " berhasil ditambahkan di posisi tengah!" << endl;
                    }
                }
            }
        }
        if (addToStack) {
            Operation op;
            op.type = "insertBaris";
            op.line = baris;
            push(undoStack, op);
        }
    }
}

void deleteBaris(List &L, int baris, OperationStack &undoStack, bool addToStack) {
    adrParent P;

    if (L.first == nullptr) {
        cout << "Tidak ada teks yang tersimpan, tidak dapat melakukan penghapusan!" << endl;
    } else {
        adrParent Q = L.first;
        bool found = false;

        // Cari node dengan info == baris
        while (Q != nullptr && !found) {
            if (Q->info == baris) {
                found = true;
            } else {
                Q = Q->next;
            }
        }

        if (found) {

            // Hapus node Q
            if (Q == L.first && Q == L.last) { // Jika hanya ada satu elemen
                L.first = nullptr;
                L.last = nullptr;
            } else if (Q == L.first) { // Jika elemen pertama
                L.first = Q->next;
                L.first->prev = nullptr;
            } else if (Q == L.last) { // Jika elemen terakhir
                L.last = Q->prev;
                L.last->next = nullptr;
            } else { // Jika elemen di tengah
                Q->prev->next = Q->next;
                Q->next->prev = Q->prev;
            }

            // Lepaskan node Q
            Q->next = nullptr;
            Q->prev = nullptr;

            // Hapus semua child node
            adrChild C = Q->child;
            while (C != nullptr) {
                adrChild temp = C;
                C = C->next;
                temp->next = nullptr;
                temp->prev = nullptr;
            }
            Q->child = nullptr;

            // Hapus node dari memori
            delete Q;

            // Perbarui info dari node setelah node yang dihapus
            adrParent temp = L.first;
            int nomor = 1;
            while (temp != nullptr) {
                temp->info = nomor;
                nomor++;
                temp = temp->next;
            }

            cout << "Baris ke-" << baris << " berhasil dihapus, dan nomor baris diperbarui!" << endl;

            if (addToStack) {
                Operation op;
                op.type = "deleteBaris";
                op.line = baris;
                push(undoStack, op);
            }
        } else {
            cout << "Tidak ada baris ke-" << baris << endl;
        }
    }
}

void insertKata(List &L, int baris, int posisi, string kata, OperationStack &undoStack, bool addToStack) {
    adrParent Q = L.first;

    if (L.first == nullptr && baris > 0) { // List kosong
        cout << "Baris kosong. Menambahkan baris pertama secara otomatis." << endl;

        insertBaris(L, 1, undoStack, true); // Tambahkan baris pertama
        Q = L.first; // Arahkan ke baris pertama
        Q->child = new ElmChild; // Tambahkan child pertama
        Q->child->info = kata;
        Q->child->next = nullptr;
        Q->child->prev = nullptr;

        cout << "Kata '" << kata << "' berhasil ditambahkan pada baris pertama di posisi pertama." << endl;

        if (addToStack){
            Operation op;

            op.type = "insertKata";
            op.line = baris;
            op.position = posisi;
            op.newData = kata;

            push(undoStack, op);
        }

    } else { // List tidak kosong
        // Cari baris yang sesuai
        bool found = false;
        while (Q != nullptr && !found) {
            if (Q->info == baris) {
                found = true;
            } else {
                Q = Q->next;
            }
        }

        if (found) {
            adrChild R = Q->child;
            adrChild newChild = new ElmChild;
            newChild->info = kata;
            newChild->next = nullptr;
            newChild->prev = nullptr;

            if (posisi == 1) { // Disisipkan di posisi pertama
                if (R == nullptr) { // Jika child list kosong
                    Q->child = newChild;
                } else {
                    newChild->next = R;
                    R->prev = newChild;
                    Q->child = newChild;
                }
                cout << "Kata '" << kata << "' berhasil ditambahkan pada baris ke-" << baris << " di posisi pertama." << endl;

                if (addToStack){
                    Operation op;

                    op.type = "insertKata";
                    op.line = baris;
                    op.position = posisi;
                    op.newData = kata;

                    push(undoStack, op);
                }

            } else { // Disisipkan di posisi selain pertama
                int hitung = 1;
                adrChild prevChild = nullptr;

                while (R != nullptr && hitung < posisi) { // Cari posisi yang dimaksud
                    prevChild = R;
                    R = R->next;
                    hitung++;
                }

                if (hitung == posisi) { // Posisi ditemukan di tengah
                    prevChild->next = newChild;
                    newChild->prev = prevChild;
                    newChild->next = R;
                    if (R != nullptr) {
                        R->prev = newChild;
                    }
                    cout << "Kata '" << kata << "' berhasil ditambahkan pada baris ke-" << baris << " di posisi ke-" << posisi << "." << endl;

                    if (addToStack){
                        Operation op;

                        op.type = "insertKata";
                        op.line = baris;
                        op.position = hitung;
                        op.newData = kata;

                        push(undoStack, op);
                    }

                } else if (hitung == 1){
                    if (R == nullptr) { // Jika child list kosong
                        Q->child = newChild;
                    } else {
                        newChild->next = R;
                        R->prev = newChild;
                        Q->child = newChild;
                    }
                    cout << "Kata '" << kata << "' berhasil ditambahkan pada baris ke-" << baris << " di posisi pertama." << endl;
                } else { // Posisi melebihi jumlah child, tambahkan di akhir
                    prevChild->next = newChild;
                    newChild->prev = prevChild;
                    cout << "Kata '" << kata << "' berhasil ditambahkan pada baris ke-" << baris << " di posisi terakhir, karena posisi ke-" << posisi << " tidak valid." << endl;

                    if (addToStack){
                        Operation op;

                        op.type = "insertKata";
                        op.line = baris;
                        op.position = hitung;
                        op.newData = kata;

                        push(undoStack, op);
                    }
                }
            }


        } else {
            cout << "Baris ke-" << baris << " tidak ditemukan!" << endl;
        }
    }
}

void replaceKata(List &L, string kataLama, string kataBaru, OperationStack &undoStack, bool addToStack) {
    adrParent Q = L.first;
    bool found = false;
    int countReplaced = 0; // Menghitung jumlah kata yang berhasil diganti

    if (L.first == nullptr) {
        cout << "Tidak ada text yang tersimpan. Tidak dapat melakukan penggantian kata!" << endl;
    } else {
        while (Q != nullptr) {
            adrChild R = Q->child;

            while (R != nullptr) {
                if (R->info == kataLama) {
                    R->info = kataBaru;
                    countReplaced++;
                    found = true;
                }
                R = R->next;
            }
            Q = Q->next;

            if (addToStack){
                Operation op;
                op.type = "replace";
                op.oldData = kataLama;
                op.newData = kataBaru;

                push(undoStack, op);
            }
        }

        if (found) {
            cout << "Sebanyak " << countReplaced << " kata '" << kataLama
                 << "' telah diganti dengan '" << kataBaru << "'!" << endl;
        } else {
            cout << "Kata '" << kataLama << "' tidak ditemukan di dalam baris manapun!" << endl;
        }
    }
}

void findKata(List L, string kata) {
    adrParent Q = L.first; // Mulai dari elemen pertama
    bool found = false;    // Flag untuk menandai apakah kata ditemukan

    if (L.first == nullptr) { // Jika list kosong
        cout << "Tidak ada text yang tersimpan!" << endl;
    } else {
        while (Q != nullptr) { // Jelajahi setiap baris
            adrChild R = Q->child; // Ambil child dari parent saat ini

            while (R != nullptr) { // Jelajahi setiap kata di baris ini
                if (R->info == kata) { // Jika kata ditemukan
                    cout << "Kata '" << kata << "' terdapat pada baris ke-" << Q->info << endl;
                    found = true; // Tandai kata ditemukan
                }
                R = R->next; // Pindah ke kata berikutnya
            }
            Q = Q->next; // Pindah ke baris berikutnya
        }

        if (!found) { // Jika tidak ada kata yang ditemukan
            cout << "Kata '" << kata << "' tidak ditemukan!" << endl;
        }
    }
}

void showAll(List L) {
    if (L.first == nullptr) { // Jika list kosong
        cout << "Tidak ada text yang disimpan!" << endl;
    } else {
        adrParent current = L.first; // Mulai dari baris pertama

        while (current != nullptr) { // Traversal setiap baris
            cout << "Baris ke-" << current->info << ": ";

            adrChild child = current->child; // Mulai dari kata pertama di baris ini

            if (child == nullptr) { // Jika tidak ada kata di baris ini
                cout << "Kosong";
            } else {
                while (child != nullptr) { // Traversal kata
                    cout << child->info << " ";
                    child = child->next;
                }
            }

            cout << endl; // Pindah ke baris berikutnya
            current = current->next;
        }
    }
}

void showPerbaris(List L, int baris) {
    if (L.first == nullptr) {  // Cek apakah list kosong
        cout << "Tidak ada text yang disimpan!" << endl;
    } else {
        adrParent current = L.first;

        // Cari parent yang sesuai dengan baris yang dicari
        while (current != nullptr && current->info != baris) {
            current = current->next;
        }

        if (current == nullptr) {  // Jika tidak ditemukan parent yang cocok
            cout << "Baris tidak ditemukan!" << endl;
        } else {
            adrChild child = current->child;

            // Cek apakah ada child yang tersimpan di baris ini
            if (child == nullptr) {
                cout << "Tidak ada text yang tersimpan di baris ini!" << endl;
            } else {
                // Tampilkan semua child pada baris tersebut
                while (child != nullptr) {
                    cout << child->info << " ";
                    child = child->next;
                }
                cout << endl;
            }
        }
    }
}

void deleteKata(List &L, int baris, int posisi, string kata, OperationStack &redoStack, bool addToStack) {
    adrParent Q = L.first;
    // Mencari baris
    while (Q != nullptr && Q->info != baris) {
        Q = Q->next;
    }

    if (Q != nullptr) {
        adrChild C = Q->child;
        int currentPos = 1;

        // Mencari kata pada posisi
        while (C != nullptr && currentPos < posisi) {
            C = C->next;
            currentPos++;
        }

        if (C != nullptr && currentPos == posisi) {
            // Hapus node C
            if (C->prev == nullptr) {
                // Jika elemen pertama
                Q->child = C->next;
                if (Q->child != nullptr) {
                    Q->child->prev = nullptr;
                }
            } else if (C->next == nullptr) {
                // Jika elemen terakhir
                C->prev->next = nullptr;
            } else {
                // Jika elemen di tengah
                C->prev->next = C->next;
                C->next->prev = C->prev;
            }

            cout << "Kata pada posisi " << posisi << " berhasil dihapus dari baris " << baris << endl;

            if (addToStack){
                Operation op;
                op.type = "deleteKata";
                op.newData = kata;
                op.position = posisi;
                push(redoStack, op);
            }
        } else {
            cout << "Posisi tidak valid pada baris " << baris << endl;
        }
    } else {
        cout << "Baris tidak ditemukan!" << endl;
    }
}

void undo(List &L, OperationStack &undoStack, OperationStack &redoStack) {
    if (isEmpty(undoStack)) {
        cout << "Tidak ada operasi untuk di-undo" << endl;
    } else {
        Operation op = pop(undoStack); // Mengambil operasi dari undoStack
        push(redoStack, op); // Memindahkan ke redoStack

        if (op.type == "insertBaris") {
            cout << "Undo: Menghapus baris " << op.line << endl;
            deleteBaris(L, op.line, redoStack, false);

        } else if (op.type == "deleteBaris") {
            cout << "Undo: Mengembalikan baris " << op.line << endl;
            insertBaris(L, op.line, redoStack, false);

        } else if (op.type == "insertKata") {
            cout << "Undo: Menghapus kata '" << op.newData << "' dari baris " << op.line << " dan" << " posisi " << op.position << endl;
            deleteKata(L, op.line, op.position, op.newData, redoStack, false);

        } else if (op.type == "deleteKata") {
            cout << "Undo: Mengembalikan kata '" << op.newData << "' dari baris " << op.line << " dan" << " posisi " << op.position << endl;
            insertKata(L, op.line, op.position, op.newData, redoStack, false);

        } else if (op.type == "replace") {
            cout << "Undo: Mengganti kata '" << op.newData << "' kembali ke '" << op.oldData << "'" << endl;
            replaceKata(L, op.newData, op.oldData, redoStack, true);

        } else {
            cout << "Operasi tidak dikenal untuk undo: " << op.type << endl;
        }
    }
}

void redo(List &L, OperationStack &undoStack, OperationStack &redoStack) {
    if (isEmpty(redoStack)) {
        cout << "Tidak ada operasi untuk di-redo";

    } else {
        Operation op;
        op = pop(redoStack);

        if (op.type == "insertBaris") {
            cout << "Redo: Menambahkan kembali baris " << op.line << endl;
            insertBaris(L, op.line, undoStack, true);

        } else if (op.type == "deleteBaris") {
            cout << "Redo: Menghapus kembali baris " << op.line << endl;
            deleteBaris(L, op.line, undoStack, true);

        } else if (op.type == "insertKata") {
            cout << "Redo: Menambahkan kembali kata '" << op.newData << "' ke baris " << op.line << endl;
            insertKata(L, op.line, op.position, op.newData, undoStack, true);

        } else if (op.type == "deleteKata") {
            cout << "Redo: Menghapus kembali kata '" << op.newData << "' ke baris " << op.line << endl;
            deleteKata(L, op.line, op.position, op.newData, undoStack, true);

        } else if (op.type == "replace") {
            cout << "Redo: Mengganti kembali kata '" << op.newData << "' ke '" << op.oldData << "'" << endl;
            replaceKata(L, op.newData, op.oldData, undoStack, true);

        } else {
            cout << "Operasi tidak dikenal untuk redo: " << op.type << endl;
        }
    }
}

void clearRedo(OperationStack &redoStack) {
    while (!isEmpty(redoStack)) {
        pop(redoStack);  // Pop elemen dari stack redo sampai kosong
    }
}

void clearScreen() {
    if (_WIN32) {
        system("cls"); // untuk Windows
    } else {
        system("clear"); // untuk Linux/Mac
    }
}
