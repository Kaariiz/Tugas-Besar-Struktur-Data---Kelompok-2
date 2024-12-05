#include "header.h"
#include <iostream>

using namespace std;

void createList(List &L) {
    // IS: List belum terinisialisasi.
    // FS: List terinisialisasi dengan L.first = nullptr dan L.last = nullptr.

    L.first = nullptr;
    L.last = nullptr;
}

address createElementList(infotype data) {
    // IS: Data yang akan dimasukkan sudah tersedia.
    // FS: Sebuah elemen baru dengan info = data dan pointer next/prev = nullptr dibuat.

    address P;

    P = new ElementList;
    P->info = data;
    P->next = nullptr;
    P->prev = nullptr;

    return P;
}

void insertText(List &L, int line, int position, infotype &data) {
    // IS: List mungkin kosong atau berisi elemen, data, line, dan position sudah didefinisikan.
    // FS: Data berhasil disisipkan di posisi (line, position) dalam list.

    address P = createElementList(data);
    address current = L.first;
    int currentLine = 1;

    if (L.first == nullptr) {
        if (line > 1) {
            cout << "Baris ke-" << line << " tidak ada, menambahkan baris baru." << endl;
        }

        L.first = P;
        L.last = P;

       cout << data << " telah disisipkan di baris ke-1, posisi ke-1." << endl;

    } else {
        address prev = nullptr;

        while (current != nullptr && currentLine < line) {
            if (current->info == "\n") {
                currentLine++;
            }
            prev = current;
            current = current->next;
        }
        
        while (currentLine < line) {
            address newLine = createElementList("\n");
            if (L.last != nullptr) {
                L.last->next = newLine;
                newLine->prev = L.last;
            }
            L.last = newLine;
            /*if (L.first == nullptr) { //L.first Ga mungkin null
                L.first = newLine;
            }*/
            currentLine++;
            prev = newLine;
        }

        if (prev != nullptr && prev->info == "\n") {
            current = prev->next;
        } else {
            current = L.first;
        }

        prev = nullptr;

        for (int i = 1; i < position && current != nullptr && current->info != "\n"; i++) {
            prev = current;
            current = current->next;
        }

        if (prev == nullptr) {
            P->next = current;
            if (current != nullptr) {
                current->prev = P;
            }
            if (L.first == current) {
                L.first = P;
            }
        } else {
            P->next = current;
            P->prev = prev;
            prev->next = P;
            if (current != nullptr) {
                current->prev = P;
            }
        }

        if (current == nullptr) {
            L.last = P;
        }

        cout << data << " telah disisipkan di baris ke-" << line << ", posisi ke-" << position << "." << endl;
    }
}



void deleteLine(List &L, int lineNumber) {
    // IS: List mungkin kosong atau berisi elemen, baris yang akan dihapus diketahui.
    // FS: Semua elemen di baris ke-lineNumber berhasil dihapus dari list.

    address current = L.first;
    int currentLine = 1;

    if (current == nullptr) {
        cout << "Tidak ada text yang tersimpan!" << endl;

    } else {
        while (current != nullptr && currentLine < lineNumber) {
            if (current->info == "\n") {
                currentLine++;
            }
            current = current->next;
        }

        if (currentLine != lineNumber) {
            cout << "Baris ke-" << lineNumber << " tidak ditemukan!" << endl;
        } else {
            while (current != nullptr && current->info != "\n") {
                address toDelete = current;
                current = current->next;

                if (toDelete->prev != nullptr) {
                    toDelete->prev->next = toDelete->next;
                } else {
                    L.first = toDelete->next;
                }

                if (toDelete->next != nullptr) {
                    toDelete->next->prev = toDelete->prev;
                } else {
                    L.last = toDelete->prev;
                }

                delete toDelete;
            }

            if (current != nullptr && current->info == "\n") {
                address toDelete = current;

                if (toDelete->prev != nullptr) {
                    toDelete->prev->next = toDelete->next;
                } else {
                    L.first = toDelete->next;
                }

                if (toDelete->next != nullptr) {
                    toDelete->next->prev = toDelete->prev;
                } else {
                    L.last = toDelete->prev;
                }

                delete toDelete;
            }

            if (L.first == nullptr) {
                L.last = nullptr;
            }

            cout << "Baris ke-" << lineNumber << " berhasil dihapus!" << endl;
        }
    }
}



void displayText(List &L) {
    // IS: List mungkin kosong atau berisi elemen.
    // FS: Semua elemen dalam list ditampilkan sesuai format baris.

    address current = L.first;

    if (current == nullptr) {
        cout << "Tidak ada text yang tersimpan!" << endl;

    } else {
        while (current != nullptr) {
            if (current->info == "\n") {
                cout << endl;
            } else {
                cout << current->info << " ";
            }
            current = current->next;
        }
        cout << endl;
    }
}


void copyPasteWord(List &L, int sourceLine, int sourcePosition, int targetLine, int targetPosition) {
     // IS: Elemen pada posisi sumber sudah diketahui, posisi target diketahui.
    // FS: Elemen pada posisi sumber berhasil disalin ke posisi target

    address current = L.first;
    int currentLine = 1;

    if (current == nullptr) {
        cout << "Tidak ada text yang tersimpan!" << endl;

    } else {
        while (current != nullptr && currentLine < sourceLine) {
            if (current->info == "\n") {
                currentLine++;
            }
            current = current->next;
        }

        if (currentLine != sourceLine) {
            cout << "Baris sumber ke-" << sourceLine << " tidak ditemukan!" << endl;

        } else {
            for (int i = 1; i < sourcePosition && current != nullptr && current->info != "\n"; i++) {
                current = current->next;
            }

            if (current == nullptr || current->info == "\n") {
                cout << "Posisi sumber ke-" << sourcePosition << " tidak valid di baris ke-" << sourceLine << "!" << endl;
            } else {
                infotype copiedWord = current->info;
                insertText(L, targetLine, targetPosition, copiedWord);
                cout << "Kata '" << copiedWord << "' berhasil disalin dari baris ke-" << sourceLine << ", posisi ke-" << sourcePosition << " ke baris ke-" << targetLine << ", posisi ke-" << targetPosition << "!" << endl;
            }
        }
    }
}




void findWord(List L, infotype word) {
    // IS: Kata yang dicari diketahui, list mungkin kosong atau berisi elemen.
    // FS: Semua posisi kata dalam list ditampilkan, atau pesan jika tidak ditemukan.

    address current = L.first;
    int Line = 1; // nomor baris dimulai dari 1
    int wordIndex = 1;  // indeks kata dalam baris dimulai dari 1
    bool found = false;

    if (current == nullptr) {
        cout << "Tidak ada text yang tersimpan!" << endl;

    } else {
        while (current != nullptr) {
            if (current->info == "\n") {
                Line++;
                wordIndex = 1;
            } else if (current->info == word) {
                cout << "Kata /" << word << "/ ditemukan di baris ke-" << Line << ", dan posisi ke-" << wordIndex << endl;
                found = true;
            }
            current = current->next;
            if (current != nullptr && current->info != "\n") {
                wordIndex++;
            }
        }

        if (!found) {
            cout << "Kata /" << word << "/ tidak ditemukan!" << endl;
        }
    }
}

void replaceWord(List &L, infotype oldWord, infotype newWord) {
    // IS: Kata yang akan diganti dan pengganti sudah diketahui, list mungkin kosong atau berisi elemen.
    // FS: Semua kemunculan kata oldWord diganti dengan newWord.

    address current = L.first;
    bool found = false;

    if (current == nullptr) {
        cout << "Tidak ada text yang tersimpan!" << endl;

    } else {
        while (current != nullptr) {
            if (current->info == oldWord) {
                current->info = newWord;
                found = true;
            }
            current = current->next;
        }

        if (found) {
            cout << "Kata \"" << oldWord << "\" berhasil diganti dengan \"" << newWord << "\"." << endl;
        } else {
            cout << "Kata \"" << oldWord << "\" tidak ditemukan dalam teks." << endl;
        }
    }
}