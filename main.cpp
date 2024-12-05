#include <iostream>
#include "header.h"
#include <string>

#ifdef _WIN32
    #define CLEAR_SCREEN "cls" // Windows
#else
    #define CLEAR_SCREEN "clear" // Unix-based systems
#endif

using namespace std;

int main() {
    List L;
    createList(L);
    int choice;
    bool stop = false;

    while (!stop) {
        cout << "==========================" << endl;
        cout << "      Text Editor Menu    " << endl;
        cout << "==========================" << endl;
        cout << "1. Display Text" << endl;
        cout << "2. Delete Line" << endl;
        cout << "3. Copy & Paste Word" << endl;
        cout << "4. Insert Word" << endl;
        cout << "5. Undo" << endl;
        cout << "6. Redo" << endl;
        cout << "7. Search Word" << endl;
        cout << "8. Replace Word" << endl;
        cout << "9. Quick Navigation" << endl;
        cout << "10. Exit" << endl;
        cout << "Pilih Menu: ";
        cin >> choice;

        if (choice == 1) {
            system(CLEAR_SCREEN);
            displayText(L);

        } else if (choice == 2) {
            system(CLEAR_SCREEN);

            int lineNumber;
            cout << "Masukkan nomor baris yang ingin dihapus: ";
            cin >> lineNumber;

            if (lineNumber < 1) {
                cout << "Nomor baris tidak valid!" << endl;
            } else {
                deleteLine(L, lineNumber);
            }

        } else if (choice == 3) {
            system(CLEAR_SCREEN);

            int sourceLine, sourcePosition, targetLine, targetPosition;

            cout << "Masukkan baris dan posisi sumber (baris posisi): ";
            cin >> sourceLine;
            cin >> sourcePosition;

            cout << "Masukkan baris dan posisi target (baris posisi): ";
            cin >> targetLine;
            cin >> targetPosition;

            if (sourceLine < 1 || sourcePosition < 1 || targetLine < 1 || targetPosition < 1) {
                cout << "Input tidak valid!" << endl;
            } else {
                copyPasteWord(L, sourceLine, sourcePosition, targetLine, targetPosition);
            }

        } else if (choice == 4) {
            system(CLEAR_SCREEN);

            int line, position;
            infotype data;

            cout << "Masukkan baris dan posisi penyisipan (baris posisi): ";
            cin >> line >> position;

            cout << "Masukkan teks yang ingin disisipkan: ";
            cin >> data;

            if (line < 1 || position < 1) {
                cout << "Input tidak valid!" << endl;
            } else {
                insertText(L, line, position, data);
            }

        } else if (choice == 5) {
            system(CLEAR_SCREEN);

           // undo(L, undoStack, redoStack);
            cout << "belum dibikin";

        } else if (choice == 6) {
            system(CLEAR_SCREEN);

            //redo(L, undoStack, redoStack);
            cout << "belum dibikin";

        } else if (choice == 7) {
            system(CLEAR_SCREEN);

            infotype word;

            cout << "Masukkan kata yang ingin dicari: ";
            cin >> word;

            findWord(L, word);

        } else if (choice == 8) {
            system(CLEAR_SCREEN);

            infotype oldWord, newWord;

            cout << "Masukkan kata yang ingin diganti: ";
            cin >> oldWord;
            cout << "Masukkan kata pengganti: ";
            cin >> newWord;

            replaceWord(L, oldWord, newWord);

        } else if (choice == 9) {
            system(CLEAR_SCREEN);

            int lineNumber;

            /*cout << "Masukkan nomor baris untuk navigasi cepat: ";
            cin >> lineNumber;

            if (lineNumber < 1) {
                cout << "Nomor baris tidak valid!" << endl;
            } else {
                navigasiCepat(L, lineNumber);
            }*/
            cout << "belum dibikin";

        } else if (choice == 10) {
            system(CLEAR_SCREEN);

            stop = true;

        } else {
            system(CLEAR_SCREEN);

            cout << "Pilihan tidak valid!" << endl;
        }
    }

    cout << "Program selesai. Terima kasih!" << endl;
    return 0;


}