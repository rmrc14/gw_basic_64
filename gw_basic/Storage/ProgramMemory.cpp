#include "ProgramMemory.h"
//#include "IO/ConsoleIO.h"


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
void ProgramMemory::list() {
    for (const auto& entry : memory) {
        // print the map content using systeminterface 
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