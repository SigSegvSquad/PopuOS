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
    for (unsigned char j: this->IR) {
        *syslog << j;
    }
    *syslog << endl << "R  : ";
    for (unsigned char j: this->R) {
        *syslog << j;
    }
    *syslog << endl << "C  : " << C << endl << endl;

    for (int i = 0; i < INSTRUCTION_LIMIT; i++) {
        if (i % 10 == 0) {
            *syslog << endl << "Page " << i / 10 << endl;

        }

        if(i/10 == PTR/10 && memoryRegisters[i][0] != EMPTY) {
            *syslog << "Address ";
            if (i < 10) *syslog << '0';
            *syslog << i << ": ";
            for (int j = 0; j < INSTRUCTION_SIZE; j++) {
                *syslog << (int) (this->memoryRegisters[i][j]);
            }
            *syslog << endl;
            continue;
        }

        *syslog << "Address ";
        if (i < 10) *syslog << '0';
        *syslog << i << ": ";
        for (int j = 0; j < INSTRUCTION_SIZE; j++) {
            *syslog << (this->memoryRegisters[i][j]);
        }
        *syslog << endl;
    }
}

void RegisterBank::resetFlagsAndCounters() {
    SI = 0;
    PI = 0;
    TI = 0;

    totalTimeCounter = 0;
    totalLineCounter = 0;
    totalTimeLimit = 0;
    totalLineLimit = 0;

    errorCode = 0;
}

void RegisterBank::incrementTime(int timeCost) {
    totalTimeCounter += timeCost;

    if (totalTimeCounter == totalTimeLimit) {
        TI = 2;
        errorCode = 3;
    }
}

void RegisterBank::incrementLine() {
    totalLineCounter++;

    if (totalLineCounter > totalLineLimit) {
        errorCode = 2;
    }
}

void RegisterBank::printProgramControlValues(ostream *outFile) {
    *outFile << "TTL : " << (int) totalTimeLimit << "  ";
    *outFile << "TLL : " << (int) totalLineLimit << "  ";
    *outFile << "PTR : " << (int) PTR << endl << endl;

    *outFile << "Interrupt Values - TI : " << (int) TI;
    *outFile << "  SI : " << (int) SI;
    *outFile << "  PI : " << (int) PI << endl;

    *outFile << "Counter Values   - TTC : " << (int) totalTimeCounter;
    *outFile << "  TLC : " << (int) totalLineCounter << endl;

    *outFile << endl;
}

void RegisterBank::initialisePageTable() {
    // set PTR to an impossible value, so it doesn't affect the randomness
    PTR = -1;
    PTR = generateRandomPage();
}

void RegisterBank::allocateMemory(int logicalAddress) {
    int pageNumber = generateRandomPage() / 10;

    int wordIndex = 0;
    while (memoryRegisters[PTR + wordIndex][0] != EMPTY) {
        wordIndex++;
    }

    memoryRegisters[PTR + wordIndex][0] = (int) logicalAddress / 10;
    memoryRegisters[PTR + wordIndex][1] = (int) logicalAddress % 10;
    memoryRegisters[PTR + wordIndex][2] = (int) pageNumber / 10;
    memoryRegisters[PTR + wordIndex][3] = (int) pageNumber % 10;

    // As GD will set SI -> 1 and PI -> 3
    SI = 0;
    PI = 0;
    errorCode = 0;
}

int RegisterBank::generateRandomPage() {
    int range = INSTRUCTION_LIMIT / PAGE_SIZE;
    int pageNumber = (rand() % range) * 10;

    while (pageNumber == PTR && isEmpty(pageNumber)) {
        pageNumber = (rand() % range) * 10;
    }

    return pageNumber;
}

int RegisterBank::getRealAddress(int logicalAddress) {
    int pageNumber = -1;

    int wordIndex = 0;
    while (memoryRegisters[PTR + wordIndex][0] != EMPTY) {
        if (memoryRegisters[PTR + wordIndex][0] == logicalAddress / 10) {
            pageNumber = memoryRegisters[PTR + wordIndex][2] * 10 + memoryRegisters[PTR + wordIndex][3];
            break;
        }
        wordIndex++;
    }

    // If page fault occurs
    if (pageNumber == -1) {
        PI = 3;
        errorCode = 6;
        return -1;
    }

    return (pageNumber * 10 + logicalAddress % 10);
}

bool RegisterBank::isEmpty(int pageAddress) {
    for (int i = pageAddress; i < pageAddress + 10; i++) {
        for (int j = 0; j < INSTRUCTION_LIMIT; j++) {
            if (memoryRegisters[i][j] != EMPTY) {
                return false;
            }
        }
    }

    return true;
}