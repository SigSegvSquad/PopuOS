#include "OperatingSystem/Utility.h"
#include "OperatingSystem/RegisterBank.h"
#include "OperatingSystem/Processor.h"

int main() {

    ifstream inFile("../input.txt");
    ofstream outFile("../output.txt");
    string cardData[10];
    bool dataFlag = false;
    int numData = 0, numLine = 0;

    RegisterBank registerBank{};
    Processor processor(&registerBank, cardData, &inFile, &outFile);
    string line;

    //read everything
    while (getline(inFile, line)) {
        cout << line << endl;
        if (regex_match(line, regex("(\\$AMJ)(.*)"))) {
            registerBank.init();
        } else if (regex_match(line, regex("(\\$DTA)(.*)"))) {
            dataFlag = true;
        }  else if (regex_match(line, regex("(\\$END)(.*)"))) {
            processor.run();
            processor.init();
            registerBank.showRegister();
            numData = 0;
            dataFlag = false;
            numLine = 0;
            continue;
        } else if (dataFlag) {
            cardData[numData++] = line;
        }else {
            memcpy(&registerBank.memoryRegisters[numLine][0], line.c_str(), line.size());
            numLine += ceil(line.size() / 4);
        }
    }

    inFile.close();
    outFile.close();

    return 0;
}
