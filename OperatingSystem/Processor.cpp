#include "Processor.h"

Processor::Processor(RegisterBank *r, ProgramControlRegister *f, ifstream *i, ofstream *o, ofstream *s) {
    this->registerBank = r;
    this->status = f;
    this->inFile = i;
    this->outFile = o;
    this->syslog = s;
}

void Processor::init(const string &jobID) {
    status->resetFlagsAndCounters();

    currJobID = jobID.substr(4, 4);
    status->totalTimeLimit = stoi(jobID.substr(8, 4));
    status->totalLineLimit = stoi(jobID.substr(12, 4));

    *syslog << "Job ID     : " << currJobID << endl;
    *syslog << "Time Limit : " << status->totalTimeLimit << endl;
    *syslog << "Line Limit : " << status->totalLineLimit << endl << endl;

    *outFile << "JID : " << currJobID << "  ";
    *outFile << "TTL : " << status->totalTimeLimit << "  ";
    *outFile << "TLL : " << status->totalLineLimit << endl << endl;

    this->done = false;
    this->isError = false;

    this->registerBank->init();
}

void Processor::run() {
    while (!isExecutionDone()) {
        memcpy(registerBank->IR, registerBank->memoryRegisters[registerBank->IC], 4);
        registerBank->IC++;
        readInput(registerBank->IR);
    }

    printJobOutput();

    this->registerBank->showRegister(syslog);
    *syslog << endl << "__________" << endl << endl;
}

bool Processor::isExecutionDone() {
    switch (status->errorCode) {
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

    *outFile << " with exit code " << status->errorCode << endl;

    if(isError) {
        *outFile << "Error Message: " << errorMessage << endl;
    }

    *outFile << "Interrupt Values - TI : " << status->timeInterrupt;
    *outFile << "  SI : " << status->serviceInterrupt;
    *outFile << "  PI : " << status->programInterrupt << endl;

    *outFile << "Counter Values   - TTC : " << status->totalTimeCounter;
    *outFile << "  TLC : " << status->totalLineCounter << endl;

    *outFile << endl;
}

void Processor::halt() {
    status->incrementTime(1);
    done = true;
}

//load data into memory if address is a multiple of 10
void Processor::getData(int address) {
    status->incrementLine();
    status->incrementTime(2);

    string line;
    streampos oldPosition = inFile->tellg();
    getline(*inFile, line);
    if(regex_match(line, regex("(\\$END)(.*)"))) {
        inFile->seekg(oldPosition);
        status->errorCode = 1;
        return;
    }

    if (address % 10 != 0) {
        status->programInterrupt = 2;
        status->errorCode = 5;
        return;
    }

    memcpy(&registerBank->memoryRegisters[address], line.c_str(), line.size());
}

//load next 40 bytes from start address into standard output
void Processor::printData(int address) {
    status->incrementTime(2);

    char line[40];
    memcpy(&line, &registerBank->memoryRegisters[address], 40);
    *outFile << line << endl;
}

//load data from memory address into register
void Processor::loadRegister(int address) {
    status->incrementTime(1);
    memcpy(registerBank->R, &registerBank->memoryRegisters[address], 4);
}

//store data from register into memory address
void Processor::storeRegister(int address) {
    status->incrementTime(1);
    memcpy(&registerBank->memoryRegisters[address], registerBank->R, 4);
}

//compare values given in two registers
void Processor::compareRegister(int address) {
    status->incrementTime(1);
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
    status->incrementTime(1);

    if (registerBank->C) {
        registerBank->IC = address;
    }
}


void Processor::readInput(const byte *instruction) {
    int address = (instruction[2] - '0') * 10 + (instruction[3] - '0');
    if(address < 0 || address > 99) {
        status->programInterrupt = 2;
        status->errorCode = 5;
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
        status->programInterrupt = 1;
        status->errorCode = 4;
        return;
    }
}