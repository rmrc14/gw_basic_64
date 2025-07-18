#include "ProgramMemory.h"
#include "IO/ConsoleIO.h"
#include <algorithm>
#include <sstream>
#include <vector>

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
void ProgramMemory::list(ConsoleIO& console) const {
    for (const auto& entry : memory) {
        int lineNumber = entry.first;
        const std::string& code = entry.second;
        std::ostringstream oss;
        oss << lineNumber << " " << code;
        console.printLine(oss.str().c_str());
    }
}


std::vector<int> ProgramMemory::getLineNumbers() const {
    std::vector<int> numbers;
    for (const auto& entry : memory) {
        numbers.push_back(entry.first);
    }
    return numbers;
}

void ProgramMemory::clear() {
    memory.clear();
}

//   } // namespace storage
//} // namespace gw_basic
