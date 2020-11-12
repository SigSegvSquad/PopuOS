#include "RegisterBank.h"

void RegisterBank::init() {
    memset(this->memoryRegisters, EMPTY, INSTRUCTION_SIZE * INSTRUCTION_LIMIT * sizeof(byte));
    memset(this->IR, EMPTY, INSTRUCTION_SIZE * sizeof(byte));
    memset(this->R, EMPTY, INSTRUCTION_SIZE * sizeof(byte));
    this->IC = 0;
    this->C = false;
}

void RegisterBank::showRegister() {
    cout << "IC : " << this->IC << endl;
    cout << "IR : ";
    for (unsigned char j : this->IR) {
        cout << j;
    }
    cout << endl << "R  : ";
    for (unsigned char j : this->R) {
        cout << j;
    }
    cout << endl << "C  : " << C << endl << endl;

    for (int i = 0; i < INSTRUCTION_LIMIT; i++) {
        cout << "Address " << i << ": ";
        for (int j = 0; j < INSTRUCTION_SIZE; j++) {
            putchar(this->memoryRegisters[i][j]);
        }
        cout << endl;
    }
}