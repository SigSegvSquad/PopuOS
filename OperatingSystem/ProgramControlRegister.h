#ifndef POPUOS_PROGRAMCONTROLREGISTER_H
#define POPUOS_PROGRAMCONTROLREGISTER_H

#include "Utility.h"

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

class ProgramControlRegister {
public:
    int serviceInterrupt = 0;          // SI Interrupt
    int programInterrupt = 0;          // PI Interrupt
    int timeInterrupt = 0;             // TI Interrupt

    int totalTimeCounter = 0;
    int totalLineCounter = 0;
    int totalTimeLimit = 0;
    int totalLineLimit = 0;

    int errorCode = 0;

    void resetFlagsAndCounters();

    void incrementTime(int);
    void incrementLine();
};

#endif //POPUOS_PROGRAMCONTROLREGISTER_H
