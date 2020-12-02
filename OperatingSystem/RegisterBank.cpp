#include "RegisterBank.h"

void RegisterBank::init() {
    memset(this->memoryRegisters, EMPTY, INSTRUCTION_SIZE * INSTRUCTION_LIMIT * sizeof(byte));
    memset(this->IR, EMPTY, INSTRUCTION_SIZE * sizeof(byte));
    memset(this->R, EMPTY, INSTRUCTION_SIZE * sizeof(byte));
    this->IC = 0;
    this->C = false;
}

void RegisterBank::showRegister(ostream *syslog) {
    *syslog << "IC : " << this->IC << endl;
    *syslog << "IR : ";
    for (unsigned char j : this->IR) {
        *syslog << j;
    }
    *syslog << endl << "R  : ";
    for (unsigned char j : this->R) {
        *syslog << j;
    }
    *syslog << endl << "C  : " << C << endl << endl;

    for (int i = 0; i < INSTRUCTION_LIMIT; i++) {
        *syslog << "Address ";
        if(i < 10) *syslog << '0';
        *syslog << i << ": ";
        for (int j = 0; j < INSTRUCTION_SIZE; j++) {
            *syslog << (this->memoryRegisters[i][j]);
        }
        *syslog << endl;
    }
}