#include <iostream>
#include "header.h"

using namespace std;

int main() {
    Document D;
    createDocument(D);
    OperationStack undoStack, redoStack;
    createStack(undoStack);
    createStack(redoStack);

    int choice;
    while (true) {
        cout << "Menu:\n";
        cout << "1. Display Text\n";
        cout << "2. Insert Word\n";
        cout << "3. Delete Word\n";
        cout << "4. Undo\n";
        cout << "5. Redo\n";
        cout << "6. Copy-Paste Word\n";
        cout << "7. Replace Word\n";
        cout << "8. Find Word\n";
        cout << "9. Exit\n";
        cout << "Pilih: ";
        cin >> choice;

        if (choice == 1) {
            displayText(D);
        } else if (choice == 2) {
            int line, pos;
            string word;
            cout << "Masukkan baris dan posisi: ";
            cin >> line >> pos;
            cout << "Masukkan kata: ";
            cin >> word;
            clearRedo(redoStack);
            insertText(D, line, pos, word, undoStack);
        } else if (choice == 3) {
            int line, pos;
            cout << "Masukkan baris dan posisi: ";
            cin >> line >> pos;
            clearRedo(redoStack);
            deleteWord(D, line, pos, undoStack);
        } else if (choice == 4) {
            undo(D, undoStack, redoStack);
        } else if (choice == 5) {
            redo(D, undoStack, redoStack);
        } else if (choice == 6) {
            int lineSrc, posSrc, lineDest, posDest;
            cout << "Masukkan baris dan posisi sumber (line pos): ";
            cin >> lineSrc >> posSrc;
            cout << "Masukkan baris dan posisi tujuan (line pos): ";
            cin >> lineDest >> posDest;
            clearRedo(redoStack);
            copyPaste(D, lineSrc, posSrc, lineDest, posDest, undoStack);
        } else if (choice == 7) {
            string oldWord, newWord;
            int line, pos;
            cout << "Masukkan kata yang akan diganti: ";
            cin >> oldWord;
            cout << "Masukkan kata pengganti: ";
            cin >> newWord;
            cout << "Masukkan baris dan posisi: ";
            cin >> line >> pos;
            clearRedo(redoStack);
            replaceWord(D, oldWord, newWord, line, pos, undoStack);
        } else if (choice == 8) {
            string word;
            cout << "Masukkan kata yang dicari: ";
            cin >> word;
            findWord(D, word);
        } else if (choice == 9) {
            cout << "Keluar dari program." << endl;
            break;
        } else {
            cout << "Pilihan tidak valid!" << endl;
        }
    }

    return 0;
}
