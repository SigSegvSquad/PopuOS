#ifndef POPUOS_PROCESSOR_H
#define POPUOS_PROCESSOR_H

#include "Utility.h"
#include "RegisterBank.h"


class Processor {
private:
    int done = false;
    ifstream *inFile;
    ofstream *outFile;
    RegisterBank *registerBank;

    //instructions
    void halt();
    void getData(int address);
    void printData(int address);

    void loadRegister(int address);
    void storeRegister(int address);
    void compareRegister(int address);

    void branchIfTrue(int address);

public:
    Processor(RegisterBank *r, ifstream *i, ofstream *o);
    void readInput(const byte *instruction);
    void init();
    void run();
};


#endif //POPUOS_PROCESSOR_H
