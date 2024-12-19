#include <iostream>
#include "header.h"
#include <cstdlib>

using namespace std;

int main() {
    List L;
    OperationStack undoStack;
    OperationStack redoStack;

    createListBaris(L);
    createStack(undoStack);
    createStack(redoStack);

    int pilihan;
    bool stop = false;

    while (!stop) {
        cout << "====================================" << endl;
        cout << "           TEXT EDITOR MENU         " << endl;
        cout << "====================================" << endl;
        cout << "1. Tambahkan Baris" << endl;
        cout << "2. Hapus Baris" << endl;
        cout << "3. Tambahkan Kata ke Baris" << endl;
        cout << "4. Ganti Kata di Semua Baris" << endl;
        cout << "5. Cari Kata di Semua Baris" << endl;
        cout << "6. Tampilkan Kata di Semua Baris" << endl;
        cout << "7. Tampilkan Kata di Baris Tertentu" << endl;
        cout << "8. Undo" << endl;
        cout << "9. Redo" << endl;
        cout << "0. Keluar" << endl;
        cout << "==========================" << endl;
        cout << "Pilih menu (0-9): ";
        cin >> pilihan;
        clearScreen();

        if (pilihan == 1) {
            int baris;

            cout << "Masukkan nomor baris: ";
            cin >> baris;
            clearRedo(redoStack);
            insertBaris(L, baris, undoStack, true);

        } else if (pilihan == 2) {
            int baris;

            cout << "Masukkan nomor baris yang akan dihapus: ";
            cin >> baris;
            clearRedo(redoStack);

            deleteBaris(L, baris, undoStack, true);

        } else if (pilihan == 3) {
            int baris, posisi;
            string kata;

            cout << "Masukkan nomor baris: ";
            cin >> baris;

            cout << "Masukkan posisi kata: ";
            cin >> posisi;

            cout << "Masukkan kata: ";
            cin >> kata;
            clearRedo(redoStack);
            insertKata(L, baris, posisi, kata, undoStack, true);

        } else if (pilihan == 4) {
            string kataLama, kataBaru;

            cout << "Masukkan kata lama: ";

            cin >> kataLama;
            cout << "Masukkan kata baru: ";

            cin >> kataBaru;
            clearRedo(redoStack);
            replaceKata(L, kataLama, kataBaru, undoStack, true);

        } else if (pilihan == 5) {
            string kata;

            cout << "Masukkan kata yang dicari: ";
            cin >> kata;
            findKata(L, kata);

        } else if (pilihan == 6) {
            showAll(L);

        } else if (pilihan == 7) {
            int baris;

            cout << "Masukkan nomor baris: ";
            cin >> baris;
            showPerbaris(L, baris);

        } else if (pilihan == 8) {
            undo(L, undoStack, redoStack);

        } else if (pilihan == 9) {
            redo(L, undoStack, redoStack);

        } else if (pilihan == 0) {
            stop = true;

        } else {
            cout << "Pilihan tidak valid. Silakan coba lagi!" << endl;

        }

        if (!stop) {
            cout << endl;
        }
    }

    clearScreen();
    cout << "Keluar dari program. Terima kasih!" << endl;

    return 0;
}
