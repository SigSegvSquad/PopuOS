#ifndef POPUOS_FLAGS_H
#define POPUOS_FLAGS_H

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

 TI = 2 on Time Limit Exceeded
*/

class Flags {
public:
    int serviceInterrupt = 0;          // SI Interrupt
    int programInterrupt = 0;          // PI Interrupt
    int timeInterrupt = 0;             // TI Interrupt
};

#endif //POPUOS_FLAGS_H
