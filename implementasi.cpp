#include "header.h"
#include <iostream>

using namespace std;

void createDocument(Document &D) {
    D.firstLine = nullptr;
    D.lastLine = nullptr;
}

LineNode* createLine() {
    LineNode* L = new LineNode;
    L->prev = nullptr;
    L->next = nullptr;
    L->firstWord = nullptr;
    L->lastWord = nullptr;
    return L;
}

WordNode* createWord(infotype data) {
    WordNode* W = new WordNode;
    W->info = data;
    W->prev = nullptr;
    W->next = nullptr;
    return W;
}

// Mendapatkan baris ke-n (1-based). Jika createIfNotExist = true, maka akan membuat baris jika belum ada.
LineNode* getLine(Document &D, int lineNumber, bool createIfNotExist) {
    if (lineNumber <= 0) return nullptr;

    LineNode *current = D.firstLine;
    int currentLine = 1;
    while (current != nullptr && currentLine < lineNumber) {
        current = current->next;
        currentLine++;
    }

    // Jika baris tersebut sudah ada
    if (current != nullptr && currentLine == lineNumber) {
        return current;
    }

    // Jika baris belum ada dan kita boleh membuatnya
    if (createIfNotExist) {
        // Buat baris tambahan mulai dari currentLine hingga lineNumber
        // Jika current == nullptr berarti dokumen kosong atau tidak cukup banyak baris
        while (currentLine <= lineNumber) {
            LineNode *newLine = createLine();
            if (D.lastLine == nullptr) {
                // Dokumen masih kosong
                D.firstLine = newLine;
                D.lastLine = newLine;
            } else {
                D.lastLine->next = newLine;
                newLine->prev = D.lastLine;
                D.lastLine = newLine;
            }
            currentLine++;
        }
        // Setelah loop, currentLine = lineNumber+1, jadi baris yang kita butuhkan adalah D.lastLine (baris terakhir dibuat)
        return D.lastLine;
    }

    // Jika tidak diizinkan membuat baris baru, kembalikan nullptr
    return nullptr;
}


// Mendapatkan kata ke-n (1-based) dalam sebuah baris
WordNode* getWord(LineNode *L, int position) {
    if (L == nullptr || position <= 0) return nullptr;
    WordNode *current = L->firstWord;
    int pos = 1;
    while (current != nullptr && pos < position) {
        current = current->next;
        pos++;
    }
    return current;
}

int countWords(LineNode *L) {
    int count = 0;
    WordNode *current = L->firstWord;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

int countLines(Document &D) {
    int count = 0;
    LineNode *current = D.firstLine;
    while (current != nullptr) {
        count++;
        current = current->next;
    }
    return count;
}

void insertText(Document &D, int line, int position, infotype &data, OperationStack &undoStack) {
    // Pastikan line tersedia
    LineNode *L = getLine(D, line, true);
    if (L == nullptr) return;

    // Buat word baru
    WordNode *newWord = createWord(data);
    int totalWords = countWords(L);

    if (totalWords == 0) {
        // Baris kosong, langsung masukkan
        L->firstWord = newWord;
        L->lastWord = newWord;
    } else {
        if (position > totalWords + 1) position = totalWords + 1; // Jika posisi melebihi panjang baris, append di akhir
        if (position == 1) {
            // Insert di depan
            newWord->next = L->firstWord;
            L->firstWord->prev = newWord;
            L->firstWord = newWord;
        } else if (position == totalWords + 1) {
            // Insert di akhir
            newWord->prev = L->lastWord;
            L->lastWord->next = newWord;
            L->lastWord = newWord;
        } else {
            // Insert di tengah
            WordNode *target = getWord(L, position);
            // Jika getWord mengembalikan kata di posisi exact
            // Kita akan insert sebelum target
            if (target != nullptr) {
                newWord->next = target;
                newWord->prev = target->prev;
                if (target->prev) target->prev->next = newWord;
                target->prev = newWord;
                if (position == 1) L->firstWord = newWord;
            }
        }
    }

    cout << "Insert: Line=" << line << ", Pos=" << position << ", Data=" << data << endl;

    // Push operation ke undo stack
    Operation op;
    op.action = "insert";
    op.line = line;
    op.position = position;
    op.data = data;
    push(undoStack, op);
}

void deleteWord(Document &D, int line, int position, OperationStack &undoStack) {
    LineNode *L = getLine(D, line, false);
    if (L == nullptr) {
        cout << "Line tidak ditemukan!" << endl;
        return;
    }

    WordNode *W = getWord(L, position);
    if (W == nullptr) {
        cout << "Word tidak ditemukan pada line tersebut!" << endl;
        return;
    }

    infotype delword = W->info;

    // Hapus node W dari baris
    if (W->prev != nullptr) W->prev->next = W->next;
    else L->firstWord = W->next;

    if (W->next != nullptr) W->next->prev = W->prev;
    else L->lastWord = W->prev;

    delete W;

    cout << "Kata '" << delword << "' berhasil dihapus dari line " << line << " posisi " << position << endl;

    Operation op;
    op.action = "delete";
    op.line = line;
    op.position = position;
    op.data = delword;
    push(undoStack, op);

    // Jika baris menjadi kosong setelah penghapusan, baris tetap ada (bisa dihapus jika mau),
    // tetapi di sini kita biarkan baris kosong tetap ada untuk menjaga konsistensi indexing.
}

void displayText(Document &D) {
    if (D.firstLine == nullptr) {
        cout << "Dokumen kosong" << endl;
        return;
    }

    LineNode *linePtr = D.firstLine;
    int lineNum = 1;
    while (linePtr != nullptr) {
        cout << lineNum << ": ";
        WordNode *wordPtr = linePtr->firstWord;
        if (wordPtr == nullptr) {
            cout << "(empty line)";
        } else {
            while (wordPtr != nullptr) {
                cout << wordPtr->info << " ";
                wordPtr = wordPtr->next;
            }
        }
        cout << endl;
        linePtr = linePtr->next;
        lineNum++;
    }
}

void findWord(Document &D, infotype word) {
    LineNode *linePtr = D.firstLine;
    int lineNum = 1;
    bool found = false;
    while (linePtr != nullptr) {
        WordNode *wordPtr = linePtr->firstWord;
        int pos = 1;
        while (wordPtr != nullptr) {
            if (wordPtr->info == word) {
                cout << "Kata \"" << word << "\" ditemukan di Baris " << lineNum << ", Posisi " << pos << endl;
                found = true;
            }
            pos++;
            wordPtr = wordPtr->next;
        }
        linePtr = linePtr->next;
        lineNum++;
    }

    if (!found) {
        cout << "Kata \"" << word << "\" tidak ditemukan!" << endl;
    }
}

void replaceWord(Document &D, infotype oldWord, infotype newWord, int line, int position, OperationStack &undoStack) {
    LineNode *L = getLine(D, line, false);
    if (L == nullptr) {
        cout << "Line tidak ditemukan!" << endl;
        return;
    }

    WordNode *W = getWord(L, position);
    if (W == nullptr) {
        cout << "Word tidak ditemukan pada posisi tersebut!" << endl;
        return;
    }

    if (W->info == oldWord) {
        infotype replacedData = W->info;
        W->info = newWord;
        cout << "Kata " << replacedData << " diganti menjadi " << newWord << endl;

        Operation op;
        op.action = "replace";
        op.line = line;
        op.position = position;
        op.data = newWord;
        op.oldData = replacedData;
        push(undoStack, op);
    } else {
        cout << "Kata yang ingin diganti tidak cocok!" << endl;
    }
}

void copyPaste(Document &D, int lineSrc, int posSrc, int lineDest, int posDest, OperationStack &undoStack) {
    LineNode *LSrc = getLine(D, lineSrc, false);
    if (LSrc == nullptr) {
        cout << "Sumber line tidak ditemukan!" << endl;
        return;
    }

    WordNode *WSrc = getWord(LSrc, posSrc);
    if (WSrc == nullptr) {
        cout << "Kata sumber tidak ditemukan pada posisi tersebut!" << endl;
        return;
    }

    infotype copiedData = WSrc->info;

    // Insert di line tujuan
    LineNode *LDest = getLine(D, lineDest, true);
    insertText(D, lineDest, posDest, copiedData, undoStack);

    cout << "Copy-Paste berhasil: " << copiedData << " dari line " << lineSrc << " pos " << posSrc
         << " ke line " << lineDest << " pos " << posDest << endl;

    // Update operation yang di-push terakhir dengan info copyPaste
    // InsertText di atas push action=insert, kita timpa dengan copyPaste agar Undo/Redo sesuai.
    if (!isEmpty(undoStack)) {
        Operation &lastOp = undoStack.operations[undoStack.top];
        if (lastOp.action == "insert" && lastOp.data == copiedData && lastOp.line == lineDest && lastOp.position == posDest) {
            lastOp.action = "copyPaste";
            lastOp.srcLine = lineSrc;
            lastOp.srcPos = posSrc;
            lastOp.destLine = lineDest;
            lastOp.destPos = posDest;
        }
    }
}

// Stack
void createStack(OperationStack &S){
    S.top = -1;
}

bool isEmpty(OperationStack S){
    return S.top == -1;
}

bool isFull(OperationStack S){
    return S.top == MAX_STACK - 1;
}

void push(OperationStack &S, Operation P){
    if (!isFull(S)){
        S.top++;
        S.operations[S.top] = P;
    } else {
        cout << "Stack Penuh" << endl;
    }
}

void pop(OperationStack &S, Operation &operation){
    if (!isEmpty(S)){
        operation = S.operations[S.top];
        S.top--;
    } else {
        cout << "Stack Kosong" << endl;
    }
}

void clearRedo(OperationStack &redow){
    Operation Q;
    while (!isEmpty(redow)){
        pop(redow,Q);
    }
}

void undo(Document &D, OperationStack &undoStack, OperationStack &redoStack) {
    if (isEmpty(undoStack)) {
        cout << "Tidak ada operasi untuk di-undo!" << endl;
        return;
    }

    Operation lastOp;
    pop(undoStack, lastOp);
    cout << "Melakukan Undo operasi: " << lastOp.action << " pada baris " << lastOp.line << ", posisi " << lastOp.position << endl;

    if (lastOp.action == "insert") {
        // Undo insert = hapus kata yang baru diinsert
        // Data kata yang diinsert adalah lastOp.data
        // Cukup panggil deleteWord, tapi hasilnya masuk ke redoStack
        deleteWord(D, lastOp.line, lastOp.position, redoStack);
    } else if (lastOp.action == "delete") {
        // Undo delete = insert kembali kata yang dihapus
        infotype w = lastOp.data;
        insertText(D, lastOp.line, lastOp.position, w, redoStack);
    } else if (lastOp.action == "copyPaste") {
        // Undo copyPaste = hapus kata yang dipaste
        deleteWord(D, lastOp.destLine, lastOp.destPos, redoStack);
    } else if (lastOp.action == "replace") {
        // Undo replace = kembalikan kata lama
        replaceWord(D, lastOp.data, lastOp.oldData, lastOp.line, lastOp.position, redoStack);
    }

    cout << "Undo Berhasil Dilakukan" << endl;
}

void redo(Document &D, OperationStack &undoStack, OperationStack &redoStack) {
    if (isEmpty(redoStack)) {
        cout << "Tidak ada operasi untuk di-redo!" << endl;
        return;
    }

    Operation lastOp;
    pop(redoStack, lastOp);
    cout << "Melakukan redo operasi: " << lastOp.action << " pada baris " << lastOp.line << ", posisi " << lastOp.position << endl;

    if (lastOp.action == "insert") {
        // Redo delete berarti insert -> delete lagi
        // Karena undo-nya insert, sekarang redo delete dengan hapus lagi
        deleteWord(D, lastOp.line, lastOp.position, undoStack);
    } else if (lastOp.action == "delete") {
        // Redo insert berarti delete -> insert lagi
        infotype w = lastOp.data;
        insertText(D, lastOp.line, lastOp.position, w, undoStack);
    } else if (lastOp.action == "copyPaste") {
        // Redo copyPaste = copyPaste lagi
        // Sebenarnya copyPaste sudah di handle mirip insert,
        // jadi kita insert lagi data ke posisi tujuan
        infotype w = lastOp.data;
        insertText(D, lastOp.destLine, lastOp.destPos, w, undoStack);
        // Update operation terakhir jadi copyPaste lagi
        if (!isEmpty(undoStack)) {
            Operation &opRef = undoStack.operations[undoStack.top];
            if (opRef.action == "insert" && opRef.data == w && opRef.line == lastOp.destLine && opRef.position == lastOp.destPos) {
                opRef.action = "copyPaste";
                opRef.srcLine = lastOp.srcLine;
                opRef.srcPos = lastOp.srcPos;
                opRef.destLine = lastOp.destLine;
                opRef.destPos = lastOp.destPos;
            }
        }
    } else if (lastOp.action == "replace") {
        // Redo replace = replace lagi ke kata baru
        replaceWord(D, lastOp.oldData, lastOp.data, lastOp.line, lastOp.position, undoStack);
    }

    cout << "Redo berhasil dilakukan." << endl;
}
