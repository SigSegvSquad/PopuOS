#ifndef POPUOS_PROCESSOR_H
#define POPUOS_PROCESSOR_H

#include "Utility.h"
#include "RegisterBank.h"


class Processor {
private:
    int dataNum = 0;
    int done = false;
    ofstream *outFile;
    ifstream *inFile;
    string *cardData;
    RegisterBank *registerBank;

    void halt();
    void getData(int address);
    void printData(int address);

    void loadRegister(int address);
    void storeRegister(int address);
    void compareRegister(int address);

    void branchIfTrue(int address);

public:
    Processor(RegisterBank *r, string *s, ifstream *i, ofstream *o);
    void readInput(const byte *instruction);
    void init();
    void run();
};


#endif //POPUOS_PROCESSOR_H
