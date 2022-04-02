#ifndef POPUOS_REGISTERBANK_H
#define POPUOS_REGISTERBANK_H

#include "Utility.h"

using namespace std;

class RegisterBank {
public:
    byte memoryRegisters[INSTRUCTION_LIMIT][INSTRUCTION_SIZE];      // Contains program and memory content
    byte R[INSTRUCTION_SIZE];                                       // General purpose register
    byte IR[INSTRUCTION_SIZE];                                      // Instruction Register
    u16 IC;                                                         // Instruction Count
    bool C;                                                         // Toggle
    
    byte PTR[INSTRUCTION_SIZE];                                     // Page Table Register

    byte serviceInterrupt = 0;                                      // SI Interrupt
    byte programInterrupt = 0;                                      // PI Interrupt
    byte timeInterrupt = 0;                                         // TI Interrupt

    byte totalTimeCounter = 0;                                      // TTC
    byte totalLineCounter = 0;                                      // TLC
    byte totalTimeLimit = 0;                                        // TTL
    byte totalLineLimit = 0;                                        // TLL

    byte errorCode = 0;


    void init();
    void showRegister(ostream *);

    // Program Control
    void resetFlagsAndCounters();
    void incrementTime(int);
    void incrementLine();

    void printProgramControlValues(ostream *);

    // Paging
    void allocateMemory(int sizeWords);
};

#endif //POPUOS_REGISTERBANK_H
