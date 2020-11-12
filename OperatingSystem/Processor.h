
#ifndef POPUOS_PROCESSOR_H
#define POPUOS_PROCESSOR_H

#include "Utility.h"
#include "RegisterBank.h"


class Processor {
public:
    int numLine;
    int numData;
    bool dataFlag;
    string cardData[10];
    int dataNum;
    bool done;

    Processor();
    void halt();
    void getData(int address, RegisterBank &registerBank);
    void printData(int address, RegisterBank &registerBank);
    void loadRegister(int address, RegisterBank &registerBank);
    void storeRegister(int address, RegisterBank &registerBank);
    void compareRegister(int address, RegisterBank &registerBank);
    void branchIfTrue(int address, RegisterBank &registerBank);
    void readInput(const byte *instruction, RegisterBank &registerBank);
};


#endif //POPUOS_PROCESSOR_H
