#include "ProgramControlRegister.h"

void ProgramControlRegister::resetFlagsAndCounters() {
    serviceInterrupt = 0;
    programInterrupt = 0;
    timeInterrupt = 0;

    totalTimeCounter = 0;
    totalLineCounter = 0;
    totalTimeLimit = 0;
    totalLineLimit = 0;

    errorCode = 0;
}

void ProgramControlRegister::incrementTime(int timeCost) {
    totalTimeCounter += timeCost;

    if(totalTimeCounter == totalTimeLimit) {
        timeInterrupt = 2;
        errorCode = 3;
    }
}

void ProgramControlRegister::incrementLine() {
    totalLineCounter++;

    if(totalLineCounter > totalLineLimit) {
        errorCode = 2;
    }
}