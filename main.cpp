#include "OperatingSystem/Utility.h"
#include "OperatingSystem/RegisterBank.h"
#include "OperatingSystem/Processor.h"

using namespace std;

RegisterBank registerBank{};

int main() {

    string line;
    Processor processor;

    ifstream inFile("../input.txt");
    ofstream outFile("../output.txt", ios::out);
    outFile<<"";
    outFile.close();

    //read everything
    while (getline(inFile, line)) {
        if (regex_match(line, regex("(\\$AMJ)(.*)"))) {
            registerBank.init();
        } else if (regex_match(line, regex("(\\$DTA)(.*)"))) {
            processor.dataFlag = true;
        } else if (processor.dataFlag) {
            processor.cardData[processor.numData++] = line;
        } else if (regex_match(line, regex("(\\$END)(.*)"))) {
            processor.dataFlag = false;
            break;
        } else {
            memcpy(&registerBank.memoryRegisters[processor.numLine][0], line.c_str(), line.size());
            processor.numLine++;
        }
    }

    while (!processor.done) {
        memcpy(registerBank.IR, registerBank.memoryRegisters[registerBank.IC++], 4);
        processor.readInput(registerBank.IR, registerBank);
    }

    registerBank.showRegister();
    inFile.close();

    return 0;
}
