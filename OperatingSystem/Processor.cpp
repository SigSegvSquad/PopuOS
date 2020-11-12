
#include "Processor.h"



Processor :: Processor(){
    dataNum = 0;
    done = false;
    numLine = 0;
    numData = 0;
    dataFlag = false;
}

void Processor :: halt() {
    done = true;
}

void Processor :: getData(int address, RegisterBank &registerBank) {
    string line = cardData[dataNum++];
    memcpy(&registerBank.memoryRegisters[address], line.c_str(), line.size());
}

void Processor :: printData(int address, RegisterBank &registerBank) {
    ofstream outFile("../output.txt", ios::app);
    char line[40];
    memcpy(&line, &registerBank.memoryRegisters[address], 40);
    outFile << line << endl;
    outFile.close();
}

//load data from memory address into register
void Processor :: loadRegister(int address, RegisterBank &registerBank) {
    memcpy(registerBank.R, &registerBank.memoryRegisters[address], 4);
}

//store data from register into memory address
void Processor :: storeRegister(int address, RegisterBank &registerBank) {
    memcpy(&registerBank.memoryRegisters[address], registerBank.R, 4);
}

void Processor :: compareRegister(int address, RegisterBank &registerBank) {
    bool equal = true;
    for (int i = 0; i < 4; i++) {
        cout << registerBank.memoryRegisters[address][i] << ' ' << registerBank.R[i] << endl;
        if (registerBank.memoryRegisters[address][i] != registerBank.R[i]) {
            equal = false;
            break;
        }
    }
    registerBank.C = equal;
}

void Processor :: branchIfTrue(int address, RegisterBank &registerBank) {
    if (registerBank.C) {
        registerBank.IC = address;
    }
}


void Processor :: readInput(const byte *instruction, RegisterBank &registerBank) {
    int address = (instruction[2] - '0') * 10 + (instruction[3] - '0');
    if (instruction[0] == 'H') { halt(); }
    else if (instruction[0] == 'G' && instruction[1] == 'D') { getData(address, registerBank); }
    else if (instruction[0] == 'P' && instruction[1] == 'D') { printData(address, registerBank); }
    else if (instruction[0] == 'L' && instruction[1] == 'R') { loadRegister(address, registerBank); }
    else if (instruction[0] == 'S' && instruction[1] == 'R') { storeRegister(address, registerBank); }
    else if (instruction[0] == 'C' && instruction[1] == 'R') { compareRegister(address, registerBank); }
    else if (instruction[0] == 'B' && instruction[1] == 'T') { branchIfTrue(address, registerBank); }
}