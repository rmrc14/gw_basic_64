#include "ProgramMemory.h"
#include <fstream>
#include <sstream>
using namespace std;

//namespace gw_basic::storage {

void ProgramMemory::insertLine(int lineNumber, const std::string& code) {
    if (code.empty()) {
        programLines.erase(lineNumber); // BASIC: empty line = delete
    }
    else {
        programLines[lineNumber] = code;
    }
}

void ProgramMemory::deleteLine(int lineNumber) {
    programLines.erase(lineNumber);
}

std::string ProgramMemory::getLine(int lineNumber) const {
    auto it = programLines.find(lineNumber);
    return (it != programLines.end()) ? it->second : "";
}

std::vector<std::string> ProgramMemory::listProgram() const {
    std::vector<std::string> lines;
    for (const auto& [lineNum, code] : programLines) {
        lines.push_back(std::to_string(lineNum) + " " + code);
    }
    return lines;
}

bool ProgramMemory::saveToFile(const std::string& filename) const {
    std::ofstream out(filename);
    if (!out.is_open()) return false;

    for (const auto& [lineNum, code] : programLines) {
        out << lineNum << " " << code << "\n";
    }

    return true;
}

bool ProgramMemory::loadFromFile(const std::string& filename) {
    std::ifstream in(filename);
    if (!in.is_open()) return false;

    programLines.clear();
    std::string line;
    while (std::getline(in, line)) {
        std::istringstream iss(line);
        int lineNum;
        std::string code;
        if (!(iss >> lineNum)) continue;
        std::getline(iss, code);
        if (!code.empty() && code[0] == ' ') code.erase(0, 1);
        programLines[lineNum] = code;
    }

    return true;
}

void ProgramMemory::clear() {
    programLines.clear();
}

//}
