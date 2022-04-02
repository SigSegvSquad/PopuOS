#include "Processor.h"

Processor::Processor(RegisterBank *r, ifstream *i, ofstream *o, ofstream *s) {
    this->registerBank = r;
    this->inFile = i;
    this->outFile = o;
    this->syslog = s;
}

void Processor::init(const string &jobID) {
    registerBank->resetFlagsAndCounters();

    currJobID = jobID.substr(4, 4);
    registerBank->totalTimeLimit = stoi(jobID.substr(8, 4));
    registerBank->totalLineLimit = stoi(jobID.substr(12, 4));

    *syslog << "Job ID : " << currJobID << endl;
    *outFile << "Job ID : " << currJobID << endl;

    this->done = false;
    this->isError = false;

    this->registerBank->init();
}

void Processor::run() {
    while (!isExecutionDone()) {
        memcpy(registerBank->IR, registerBank->memoryRegisters[registerBank->getRealAddress(registerBank->IC)], 4);
        registerBank->IC++;
        readInput(registerBank->IR);
    }

    printJobOutput();

    this->registerBank->showRegister(syslog);
    *syslog << endl << "__________" << endl << endl;
}

bool Processor::isExecutionDone() {
    switch (registerBank->errorCode) {
        case 1: {
            errorMessage = "Out of Data";
            isError = true;
            return true;
        }
        case 2: {
            errorMessage = "Line Limit Exceeded";
            isError = true;
            return true;
        }
        case 3: {
            errorMessage = "Time Limit Exceeded";
            isError = true;
            return true;
        }
        case 4: {
            errorMessage = "Opcode Error";
            isError = true;
            return true;
        }
        case 5: {
            errorMessage = "Operand Error";
            isError = true;
            return true;
        }
        case 6: {
            errorMessage = "Page Fault Occurred";
            isError = true;
            return true;
        }
    }

    return done;
}

void Processor::printJobOutput() {
    if(!isError) {
        *outFile << endl << "Job terminated successfully";
    } else {
        *outFile << endl << "Job terminated abnormally";
    }

    *outFile << " with exit code " << (int) registerBank->errorCode << endl;

    if(isError) {
        *outFile << "Error Message: " << errorMessage << endl;
    }

    registerBank->printProgramControlValues(outFile);
    registerBank->printProgramControlValues(syslog);
}

void Processor::halt() {
    registerBank->incrementTime(1);
    done = true;
}

//load data into memory if address is a multiple of 10
void Processor::getData(int address) {
    registerBank->incrementLine();
    registerBank->incrementTime(2);

    string line;
    streampos oldPosition = inFile->tellg();
    getline(*inFile, line);
    if(regex_match(line, regex("(\\$END)(.*)"))) {
        inFile->seekg(oldPosition);
        registerBank->errorCode = 1;
        return;
    }

    if (address % 10 != 0) {
        registerBank->PI = 2;
        registerBank->errorCode = 5;
        return;
    }

    registerBank->SI = 1;
    registerBank->PI = 3;

    //master mode
    int realAddress = registerBank->getRealAddress(address);

    if(realAddress == -1) {
        registerBank->allocateMemory(address);
        realAddress = registerBank->getRealAddress(address);
    }

    memcpy(&registerBank->memoryRegisters[realAddress], line.c_str(), line.size());
}

//load next 40 bytes from start address into standard output
void Processor::printData(int address) {
    registerBank->incrementTime(2);

    char line[40];

    registerBank->SI = 2;

    //master mode;
    int realAddress = registerBank->getRealAddress(address);
    memcpy(&line, &registerBank->memoryRegisters[realAddress], 40);
    *outFile << line << endl;
}

//load data from memory address into register
void Processor::loadRegister(int address) {
    registerBank->incrementTime(1);

    int realAddress = registerBank->getRealAddress(address);
    memcpy(registerBank->R, &registerBank->memoryRegisters[realAddress], 4);
}

//store data from register into memory address
void Processor::storeRegister(int address) {
    registerBank->incrementTime(1);

    int realAddress = registerBank->getRealAddress(address);

    if(realAddress == -1) {
        registerBank->allocateMemory(address);
        realAddress = registerBank->getRealAddress(address);
    }

    memcpy(&registerBank->memoryRegisters[realAddress], registerBank->R, 4);
}

//compare values given in two registers
void Processor::compareRegister(int address) {
    registerBank->incrementTime(1);
    bool equal = true;

    for (int i = 0; i < 4; ++i) {
        if (registerBank->memoryRegisters[address][i] != registerBank->R[i]) {
            equal = false;
            break;
        }
    }
    registerBank->C = equal;
}

//go to specified address if value of C is 1
void Processor::branchIfTrue(int address) {
    registerBank->incrementTime(1);

    if (registerBank->C) {
        registerBank->IC = address;
    }
}


void Processor::readInput(const byte *instruction) {
    int address = (instruction[2] - '0') * 10 + (instruction[3] - '0');
    if(address < 0 || address > 99) {
        registerBank->PI = 2;
        registerBank->errorCode = 5;
        return;
    }

    if (instruction[0] == 'H') { halt(); }
    else if (instruction[0] == 'G' && instruction[1] == 'D') { getData(address); }
    else if (instruction[0] == 'P' && instruction[1] == 'D') { printData(address); }
    else if (instruction[0] == 'L' && instruction[1] == 'R') { loadRegister(address); }
    else if (instruction[0] == 'S' && instruction[1] == 'R') { storeRegister(address); }
    else if (instruction[0] == 'C' && instruction[1] == 'R') { compareRegister(address); }
    else if (instruction[0] == 'B' && instruction[1] == 'T') { branchIfTrue(address); }
    else {
        registerBank->PI = 1;
        registerBank->errorCode = 4;
        return;
    }
}