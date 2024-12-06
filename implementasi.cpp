#include "header.h"
#include <iostream>

using namespace std;

void createList(List &L) {
    L.first = nullptr;
    L.last = nullptr;
}

address createElementList(infotype data) {
    address P = new ElementList;
    P->info = data;
    P->next = nullptr;
    P->prev = nullptr;
    return P;
}

void insertText(List &L, int line, int position, infotype &data, OperationStack &undoredo) {
    cout << "Insert: Line=" << line << ", Position=" << position << ", Data=" << data << endl;

    address current = L.first;
    int currentLine = 1;
    int po = position;

    while (current != nullptr && currentLine < line) {
        if (current->info == "\n") currentLine++;
        current = current->next;
    }

    while (currentLine < line) {
        address newLine = createElementList("\n");
        if (L.last != nullptr) {
            L.last->next = newLine;
            newLine->prev = L.last;
        } else {
            L.first = newLine;
        }
        L.last = newLine;
        currentLine++;
    }

    address newElement = createElementList(data);
    if (current == nullptr) {
        if (L.last == nullptr) {
            L.first = newElement;
            L.last = newElement;
        } else {
            L.last->next = newElement;
            newElement->prev = L.last;
            L.last = newElement;
        }
    } else {
        while (current != nullptr && position > 1 && current->info != "\n") {
            position--;
            current = current->next;
        }
        if (current == nullptr || current->info == "\n") {
            if (L.last != nullptr) {
                L.last->next = newElement;
                newElement->prev = L.last;
                L.last = newElement;
            }
        } else {
            newElement->next = current;
            newElement->prev = current->prev;
            if (current->prev != nullptr) current->prev->next = newElement;
            current->prev = newElement;
            if (L.first == current) L.first = newElement;
        }
    }
    cout << "Insert berhasil" << endl;
    Operation op;
    op.action = "insert";
    op.line = line;
    op.position = po;
    op.data = data;
    push(undoredo, op);
}


void deleteWord(List &L, int line, int position, OperationStack &undoredo) {
    address current = L.first;
    int currentLine = 1;
    infotype delword;
    // Cari baris yang sesuai
    while (current != nullptr && currentLine < line) {
        if (current->info == "\n") currentLine++;
        current = current->next;
    }
    int po = position;
    // Cari posisi dalam baris
    while (current != nullptr && position > 1 && current->info != "\n") {
        position--;
        current = current->next;
    }

    // Hapus node jika ditemukan
    if (current != nullptr && current->info != "\n") {
        delword = current->info; // Simpan kata yang dihapus
        if (current->prev != nullptr) current->prev->next = current->next;
        if (current->next != nullptr) current->next->prev = current->prev;
        if (current == L.first) L.first = current->next;
        if (current == L.last) L.last = current->prev;
        delete current;

        cout << "Kata berhasil dihapus" << endl;
        Operation op;
        op.action = "delete";
        op.line = line;
        op.position = po;
        op.data = delword;
        push(undoredo, op);
    } else {
        delword = ""; // Tidak ada kata yang dihapus
        cout << "Posisi tidak valid atau kata tidak ditemukan!" << endl;
        return;
    }

    // Cek apakah baris kosong setelah penghapusan
    current = L.first;
    currentLine = 1;

    while (current != nullptr && currentLine < line) {
    if (current->info == "\n") currentLine++;
    current = current->next;
}

// Periksa apakah baris hanya berisi `\n`
    bool isEmptyLine = true;
    address temp = current;
    while (temp != nullptr && temp->info != "\n") {
        if (!temp->info.empty()) { // Cek apakah info bukan string kosong
            isEmptyLine = false;
            break;
        }
        temp = temp->next;
    }

    // Jika baris kosong dan `current` menunjuk ke `\n`
    if (isEmptyLine && current != nullptr && current->info == "\n") {
        // Periksa apakah baris tersebut adalah baris terakhir
        if (current == L.last) {
            // Jika baris hanya terdiri dari \n, hapus baris tersebut
            if (current->prev != nullptr) {
                current->prev->next = nullptr;
                L.last = current->prev;
                delete current;
            } else {
                // Jika hanya ada satu baris (L.first == L.last)
                L.first = nullptr;
                L.last = nullptr;
                delete current;
            }
            cout << "Baris ke-" << line << " telah dihapus karena kosong." << endl;
        } else {
            // Jika bukan baris terakhir, lakukan penghapusan seperti biasa
            if (current->prev != nullptr) current->prev->next = current->next;
            if (current->next != nullptr) current->next->prev = current->prev;
            if (current == L.first) L.first = current->next;
            delete current;

            // Setelah penghapusan, cek apakah baris berikutnya kosong
            address tempNode = current->next;
            while (tempNode != nullptr) {
                if (tempNode->info == "\n") {
                    break; // Jika menemukan baris baru, stop.
                }
                currentLine++;
                tempNode = tempNode->next;
            }
            cout << "Baris ke-" << line << " telah dihapus karena kosong." << endl;
        }
    }
}

void copyPaste(List &L, int lineSrc, int posSrc, int lineDest, int posDest, OperationStack &undoredo) {
    address currentSrc = L.first;
    address currentDest = L.first;
    int currentLine = 1;
    infotype copiedData = "";

    // Cari baris sumber
    while (currentSrc != nullptr && currentLine < lineSrc) {
        if (currentSrc->info == "\n") currentLine++;
        currentSrc = currentSrc->next;
    }

    // Cari posisi pada baris sumber
    int poS = posSrc;
    while (currentSrc != nullptr && posSrc > 1 && currentSrc->info != "\n") {
        posSrc--;
        currentSrc = currentSrc->next;
    }

    // Salin teks dari posisi yang ditemukan
    if (currentSrc != nullptr && currentSrc->info != "\n") {
        copiedData = currentSrc->info;  // Menyimpan teks yang akan disalin
    }

    // Cari baris tujuan
    currentLine = 1;
    while (currentDest != nullptr && currentLine < lineDest) {
        if (currentDest->info == "\n") currentLine++;
        currentDest = currentDest->next;
    }

    // Cari posisi pada baris tujuan
    int poD = posDest;
    while (currentDest != nullptr && posDest > 1 && currentDest->info != "\n") {
        posDest--;
        currentDest = currentDest->next;
    }

    // Sisipkan salinan teks pada baris dan posisi tujuan
    address newElement = createElementList(copiedData);
    if (currentDest == nullptr || currentDest->info == "\n") {
        if (L.last != nullptr) {
            L.last->next = newElement;
            newElement->prev = L.last;
            L.last = newElement;
        } else {
            L.first = newElement;
            L.last = newElement;
        }
    } else {
        newElement->next = currentDest;
        newElement->prev = currentDest->prev;
        if (currentDest->prev != nullptr) currentDest->prev->next = newElement;
        currentDest->prev = newElement;
        if (L.first == currentDest) L.first = newElement;
    }

    cout << "Copy-Paste berhasil" << endl;

    // Simpan operasi untuk undo
    Operation op;
    op.action = "copyPaste";
    op.line = lineDest;
    op.position = posDest;
    op.data = copiedData;
    op.srcLine = lineSrc;
    op.srcPos = poS;
    op.destLine = lineDest;
    op.destPos = poD;
    push(undoredo, op);
}

void replaceWord(List &L, infotype oldWord, infotype newWord, int line, int position, OperationStack &undoredo) {
    address current = L.first;
    int currentLine = 1;
    infotype replacedData = "";

    // Cari baris yang sesuai
    while (current != nullptr && currentLine < line) {
        if (current->info == "\n") currentLine++;
        current = current->next;
    }

    // Cari posisi dalam baris
    int po = position;
    while (current != nullptr && position > 1 && current->info != "\n") {
        position--;
        current = current->next;
    }

    // Ganti kata jika ditemukan
    if (current != nullptr && current->info == oldWord) {
        replacedData = current->info; // Simpan data yang diganti
        current->info = newWord;      // Ganti dengan kata baru

        cout << "Kata berhasil diganti" << endl;

        // Simpan operasi untuk undo
        Operation op;
        op.action = "replace";
        op.line = line;
        op.position = po;
        op.data = newWord;
        op.oldData = replacedData;
        push(undoredo, op);
    } else {
        cout << "Kata tidak ditemukan!" << endl;
    }
}

void findWord(List L, infotype word) {
    address current = L.first;
    int currentLine = 1;
    int position = 1;
    bool found = false;

    while (current != nullptr) {
        if (current->info == word) {
            cout << "Kata \"" << word << "\" ditemukan di Baris " << currentLine << ", Posisi " << position << endl;
            found = true;
        }

        position++;
        if (current->info == "\n") {
            currentLine++;
            position = 1;
        }

        current = current->next;
    }

    if (!found) {
        cout << "Kata \"" << word << "\" tidak ditemukan!" << endl;
    }
}

void displayText(List &L) {
    if (L.first != nullptr){
        address current = L.first;
        while (current != nullptr) {
            if (current->info == "\n") {
                cout << endl;
            } else {
                cout << current->info << " ";
            }
            current = current->next;
        }
        cout << endl;
    } else {
        cout << "Tidak Terdapat Text pada list"<< endl;
    }
}

void createStack(OperationStack &S){
    S.top = -1;
}

bool isEmpty(OperationStack S){
    return S.top == -1;
}

bool isFull(OperationStack S){
    return S.top == MAX_STACK;
}

void push(OperationStack &S, Operation P){
    if (!isFull(S)){
        S.top++;
        S.operations[S.top] = P;
    } else {
        cout << "Stack Penuh";
    }
}

void pop(OperationStack &S, Operation &operation){
    if (!isEmpty(S)){
        operation = S.operations[S.top];
        S.top--;
    } else {
        cout << "Stack Kosong";
    }
}

void undo(List &L, OperationStack &undoStack, OperationStack &redoStack) {
    if (isEmpty(undoStack)) {
        cout << "Tidak ada operasi untuk di-undo!" << endl;
        return;
    }

    Operation lastOp;
    pop(undoStack, lastOp);
    cout << "Melakukan Undo operasi: Action = " << lastOp.action
         << ", Baris = " << lastOp.line
         << ", Posisi = " << lastOp.position
         << ", Data = " << lastOp.data << endl;

    if (lastOp.action == "insert") {
        deleteWord(L, lastOp.line, lastOp.position, redoStack);
    } else if (lastOp.action == "delete") {
        insertText(L, lastOp.line, lastOp.position, lastOp.data, redoStack);
    } else if (lastOp.action == "copyPaste") {
        // Undo CopyPaste berarti menghapus data yang disalin
        deleteWord(L, lastOp.destLine, lastOp.destPos, redoStack);  // Menghapus salinan yang dipaste

        // Jika Anda ingin mengembalikan data asli di tempat sumber, Anda dapat menyisipkannya kembali
        //insertText(L, lastOp.srcLine, lastOp.srcPos, lastOp.data, redoStack); // Menyisipkan kembali data yang disalin di tempat asalnya
    } else if (lastOp.action == "replace") {
        // Undo replace berarti mengembalikan kata yang diganti
        replaceWord(L, lastOp.data, lastOp.oldData, lastOp.line, lastOp.position, redoStack);
    }

    cout << "Undo Berhasil Dilakukan" << endl;
}



void redo(List &L, OperationStack &undoStack, OperationStack &redoStack) {
    if (isEmpty(redoStack)) {
        cout << "Tidak ada operasi untuk di-redo!" << endl;
        return;
    }

    Operation lastOp;
    pop(redoStack, lastOp);

    cout << "Melakukan redo operasi: Action = " << lastOp.action
         << ", Baris = " << lastOp.line
         << ", Posisi = " << lastOp.position
         << ", Data = " << lastOp.data << endl;

    if (lastOp.action == "delete") {
        // Masukkan teks ke posisi yang sama seperti sebelumnya
        insertText(L, lastOp.line, lastOp.position, lastOp.data, undoStack);
    } else if (lastOp.action == "insert") {
        // Hapus teks dari posisi yang sama seperti sebelumnya
        deleteWord(L, lastOp.line, lastOp.position, undoStack);
    } else if (lastOp.action == "copyPaste") {
        // Redo CopyPaste berarti menyalin data ke posisi tujuan
        insertText(L, lastOp.destLine, lastOp.destPos, lastOp.data, undoStack); // Menambahkan salinan yang dipaste

        // Jika perlu, kita juga bisa memulihkan data di tempat sumber (jika itu diubah selama proses copy-paste)
        // Contoh: Jika data yang dipaste berasal dari tempat yang berubah, kita bisa menambahkan data sumber kembali
        //insertText(L, lastOp.srcLine, lastOp.srcPos, lastOp.data, undoStack); // Menambahkan data sumber lagi
    } else if (lastOp.action == "replace") {
        // Redo replace berarti mengganti kata lagi
        replaceWord(L, lastOp.oldData, lastOp.data, lastOp.line, lastOp.position, undoStack);
    }

    cout << "Redo berhasil dilakukan." << endl;
}


void clearRedo(OperationStack &redow){
    Operation Q;
    while (!isEmpty(redow)){
        pop(redow,Q);
    }
}


