#include "OperatingSystem/Utility.h"
#include "OperatingSystem/RegisterBank.h"
#include "OperatingSystem/Processor.h"

string replaceString(string subject, const std::string &search, const string &replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != string::npos) {
        subject.replace(pos, search.length(), replace);
        pos += replace.length();
    }
    return subject;
}

int main() {
    ifstream inFile("../input.txt");
    ofstream outFile("../output.txt");
    bool instructionFlag = false;
    int numLine = 0;
    string line;

    RegisterBank registerBank{};
    Processor processor(&registerBank, &inFile, &outFile);

    //read till EoF
    while (getline(inFile, line)) {
        cout << line << endl;
        if (regex_match(line, regex("(\\$AMJ)(.*)"))) {
            processor.init();
            instructionFlag = true;
        } else if (regex_match(line, regex("(\\$DTA)(.*)"))) {
            processor.run();
//            registerBank.showRegister(); //debugging
            instructionFlag = false;
            numLine = 0;
        } else if (regex_match(line, regex("(\\$END)(.*)"))) {
            instructionFlag = false;
            outFile << endl << endl;
        } else if (instructionFlag) {
            line = replaceString(line, "H", "H000");
            memcpy(&registerBank.memoryRegisters[numLine][0], line.c_str(), line.size());
            numLine += ceil(line.size() / 4);
        }
    }

    inFile.close();
    outFile.close();

    return 0;
}
