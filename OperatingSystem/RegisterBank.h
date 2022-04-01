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

    void init();
    void showRegister(ostream *);
};

#endif //POPUOS_REGISTERBANK_H
