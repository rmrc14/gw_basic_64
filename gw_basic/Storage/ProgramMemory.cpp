// ProgramMemory.cpp

#include "ProgramMemory.h"
#include <sstream>
#include <iostream>

// Constructor
ProgramMemory::ProgramMemory() {}

// Store a line into memory
void ProgramMemory::storeLine(const std::string& inputLine) {
    int lineNumber;
    std::string code;

    // Try to parse the input
    if (!parseLine(inputLine, lineNumber, code)) {
        std::cerr << "Invalid input line: " << inputLine << "\n";
        return;
    }

    if (code.empty()) {
        // If code is empty, delete the line (like in GW-BASIC)
        programLines.erase(lineNumber);
    }
    else {
        // Otherwise, insert/update the line
        programLines[lineNumber] = code;
    }
}

// Return all stored lines in ascending order
std::vector<std::string> ProgramMemory::getAllLines() const {
    std::vector<std::string> lines;
    for (const auto& [lineNumber, code] : programLines) {
        lines.push_back(std::to_string(lineNumber) + " " + code);
    }
    return lines;
}

// Clear all lines from memory
void ProgramMemory::clear() {
    programLines.clear();
}

// Parse an input like "10 PRINT \"HELLO\"" into lineNumber and code
bool ProgramMemory::parseLine(const std::string& inputLine, int& lineNumber, std::string& code) {
    std::istringstream iss(inputLine);
    if (!(iss >> lineNumber)) {
        return false;
    }

    std::getline(iss, code);
    if (!code.empty() && code[0] == ' ') {
        code.erase(0, 1); // Remove leading space
    }

    return true;
}
