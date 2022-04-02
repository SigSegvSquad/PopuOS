#ifndef POPUOS_REGISTERBANK_H
#define POPUOS_REGISTERBANK_H

#include "Utility.h"

using namespace std;

/*
 Interrupt Values
 (0 for no Interrupt)
 SI = 1 on GD
    = 2 on PD
    = 3 on H
 PI = 1 on Opcode Error
    = 2 on Operand Error
    = 3 on Page Fault
    = 4 on Data Limit Exceeded
 TI = 2 on Time Limit Exceeded
*/

/*
 Error Codes
 EC = 0 No Error
    = 1 Out of Data
    = 2 Line Limit Exceeded
    = 3 Time Limit Exceeded
    = 4 Operation Code Error
    = 5 Operand Error
    = 6 Invalid Page Fault
*/

class RegisterBank {
public:
    byte memoryRegisters[INSTRUCTION_LIMIT][INSTRUCTION_SIZE];      // Contains program and memory content
    byte R[INSTRUCTION_SIZE];                                       // General purpose register
    byte IR[INSTRUCTION_SIZE];                                      // Instruction Register
    u16 IC;                                                         // Instruction Count
    bool C;                                                         // Toggle

    int PTR = 0;                                                    // Page Table Register

    byte SI = 0;                                                    // Service Interrupt
    byte PI = 0;                                                    // Program Interrupt
    byte TI = 0;                                                    // Time Interrupt

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
    void initialisePageTable();
    void allocateMemory(int);

    int generateRandomPage();
    int getRealAddress(int);

    bool isEmpty(int pageAddress);
};

#endif //POPUOS_REGISTERBANK_H
