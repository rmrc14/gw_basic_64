
#include "ProgramMemory.h"
#include "System/SystemInterface.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include <map>

ProgramMemory::ProgramMemory() {
    // Initialize empty memory
}

void ProgramMemory::storeLine(int lineNumber, const std::string& code) {
    if (lineNumber < 0) {
        throw std::invalid_argument("Line number cannot be negative");
    }

    if (code.empty()) {
        memory.erase(lineNumber);  // Remove line if code is empty
    }
    else {
        memory[lineNumber] = code;  // Add/update line
    }
}

std::map<int, std::string> ProgramMemory::getAllLines() const {
    return memory;  // Return copy of the memory map
}

void ProgramMemory::list() const{
    for (const auto& entry : memory) {
        std::string line = std::to_string(entry.first) + " " + entry.second;
        SystemInterface::printString(line.c_str());
        SystemInterface::printString("\n");
    }
}


std::vector<std::string> ProgramMemory::getLineNumbers() const {
    std::vector<std::string> numbers;
    for (const auto& entry : memory) {
        numbers.push_back(std::to_string(entry.first));
    }
    return numbers;
}



void ProgramMemory::clearMemory() {
    memory.clear();
}

void ProgramMemory::storeLine(const std::string& line) {
    std::istringstream iss(line);
    int lineNumber;
    std::string code;

    if (!(iss >> lineNumber)) {
        throw std::invalid_argument("Missing or invalid line number in input");
    }

    std::getline(iss, code);
    size_t firstNonSpace = code.find_first_not_of(' ');
    if (firstNonSpace != std::string::npos) {
        code = code.substr(firstNonSpace);
    }
    else {
        code.clear();
    }

    storeLine(lineNumber, code);
}

// Save program to file using SystemInterface
bool ProgramMemory::saveToFile(const std::string& path) const {
    std::vector<std::string> lines;
    for (const auto& entry : memory) {
        lines.push_back(std::to_string(entry.first) + " " + entry.second);
    }

    SystemInterface::createAndSaveFile(path, lines);
    // Assuming createAndSaveFile returns void; add error handling if needed
    return true;
}

// Load program from file using SystemInterface
bool ProgramMemory::loadFromFile(const std::string& path) {
    if (!SystemInterface::openFile(path)) {
        return false;
    }

    memory.clear();
    std::string line;
    while (SystemInterface::readLineFromFile(line)) {
        try {
            storeLine(line);
        }
        catch (...) {
            // Invalid line format, ignore or handle error as needed
        }
    }

    SystemInterface::closeFile();
    return true;
}

bool ProgramMemory::lineExist(int lineNumber) const {
    return memory.find(lineNumber) != memory.end();
}

void ProgramMemory::deleteLine(int lineNumber) {
    memory.erase(lineNumber);
}

std::string ProgramMemory::getLine(int lineNumber) const {
    auto it = memory.find(lineNumber);
    return (it != memory.end()) ? it->second : "";
}
//23/07/2025 newly added 
void ProgramMemory::renumber(int newStart, int oldStart, int increment) {
    // Temporary map to store the new renumbered lines
    std::map<int, std::string> newMemory;

    int currentNumber = newStart;

    for (const auto& [lineNumber, code] : memory) {
        if (lineNumber >= oldStart) {
            // Assign a new line number
            newMemory[currentNumber] = code;
            currentNumber += increment;
        }
        else {
            // Keep old lines (less than oldStart) unchanged
            newMemory[lineNumber] = code;
        }
    }

    // Replace old memory with renumbered memory
    memory = std::move(newMemory);
}

