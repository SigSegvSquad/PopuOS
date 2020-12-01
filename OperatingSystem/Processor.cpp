#include "Processor.h"

Processor::Processor(RegisterBank *r, ifstream *i, ofstream *o) {
    inFile = i;
    outFile = o;
    registerBank = r;
}

void Processor::init() {
    this->done = false;
    this->registerBank->init();
}

void Processor::run() {
    while (!done) {
        memcpy(registerBank->IR, registerBank->memoryRegisters[registerBank->IC], 4);
        registerBank->IC++;
        readInput(registerBank->IR);
    }
}

void Processor::halt() {
    done = true;
}

void Processor::getData(int address) {
    string line;
    getline(*inFile, line);
    memcpy(&registerBank->memoryRegisters[address], line.c_str(), line.size());
}

void Processor::printData(int address) {
    char line[40];
    memcpy(&line, &registerBank->memoryRegisters[address], 40);
    *outFile << line << endl;
}

//load data from memory address into register
void Processor::loadRegister(int address) {
    memcpy(registerBank->R, &registerBank->memoryRegisters[address], 4);
}

//store data from register into memory address
void Processor::storeRegister(int address) {
    memcpy(&registerBank->memoryRegisters[address], registerBank->R, 4);
}

void Processor::compareRegister(int address) {
    bool equal = true;
    for (int i = 0; i < 4; i++) {
        if (registerBank->memoryRegisters[address][i] != registerBank->R[i]) {
            equal = false;
            break;
        }
    }
    registerBank->C = equal;
}

void Processor::branchIfTrue(int address) {
    if (registerBank->C) {
        registerBank->IC = address;
    }
}


void Processor::readInput(const byte *instruction) {
    int address = (instruction[2] - '0') * 10 + (instruction[3] - '0');
    if      (instruction[0] == 'H')                          { halt(); }
    else if (instruction[0] == 'G' && instruction[1] == 'D') { getData(address); }
    else if (instruction[0] == 'P' && instruction[1] == 'D') { printData(address); }
    else if (instruction[0] == 'L' && instruction[1] == 'R') { loadRegister(address); }
    else if (instruction[0] == 'S' && instruction[1] == 'R') { storeRegister(address); }
    else if (instruction[0] == 'C' && instruction[1] == 'R') { compareRegister(address); }
    else if (instruction[0] == 'B' && instruction[1] == 'T') { branchIfTrue(address); }
}