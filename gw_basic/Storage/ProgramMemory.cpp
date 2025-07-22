#include "ProgramMemory.h"
#include "System/SystemInterface.h"
#include <algorithm>
#include <sstream>
#include <vector>
#include "FileIO.h" 

//namespace gw_basic {
  //  namespace storage {

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

//  std::map<int, std::string> ProgramMemory::getAllLines() const {
std::map<int, std::string> ProgramMemory::getAllLines() const {
    return memory;  // Return copy of the memory map
}

// void ProgramMemory::list(gw_basic::IO::ConsoleIO& console) const {
//void ProgramMemory::list() {
  //  for (const auto& entry : memory) {
        // print the map content using systeminterface 
    //}
//}
void ProgramMemory::list() {
    for (const auto& entry : memory) {
        // Create a full line string: "10 PRINT "HELLO""
        std::string line = std::to_string(entry.first) + " " + entry.second;

        // Use printString to print it (need c_str() because printString accepts const char*)
        SystemInterface::printString(line.c_str());

        // Also print newline (since printString doesn't print newline)
        SystemInterface::printString("\n");
    }
}



std::vector<int> ProgramMemory::getLineNumbers() const {
    std::vector<int> numbers;
    for (const auto& entry : memory) {
        numbers.push_back(entry.first);
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

    // Read the line number first
    if (!(iss >> lineNumber)) {
        throw std::invalid_argument("Missing or invalid line number in input");
    }

    // Get the rest of the line (code), including spaces
    std::getline(iss, code);

    // Remove leading spaces from code, if any
    size_t firstNonSpace = code.find_first_not_of(' ');
    if (firstNonSpace != std::string::npos) {
        code = code.substr(firstNonSpace);
    }
    else {
        code.clear(); // Entire string is spaces
    }

    // Store using the main method
    storeLine(lineNumber, code);


}
// ? NEW: Save program to file
bool ProgramMemory::saveToFile(const std::string& path) const {
    try {
        std::vector<std::string> lines;
        for (const auto& entry : memory) {
            lines.push_back(std::to_string(entry.first) + " " + entry.second);
        }
        FileIO::writeLines(path, lines);
        return true;
    }
    catch (...) {
        return false;
    }
}

// ? NEW: Load program from file
bool ProgramMemory::loadFromFile(const std::string& path) {
    try {
        auto lines = FileIO::readLines(path);
        memory.clear();
        for (const auto& line : lines) {
            storeLine(line);
        }
        return true;
    }
    catch (...) {
        return false;
    }
}
