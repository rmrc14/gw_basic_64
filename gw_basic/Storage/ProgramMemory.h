#ifndef PROGRAM_MEMORY_H
#define PROGRAM_MEMORY_H

#include <string>
#include <map>
#include <algorithm>
#include <sstream>
#include <vector>
/*

void saveToFile(std::string filename)

bool lineExist(int linenumber)  // gives true if line found in memory

void ProgramMemory::deleteLine(int lineNumber)

std::string getLine(int lineNumber) const;

void ProgramMemory::renumber(int newStart, int oldStart, int increment=10);


RENUM                   ' default renumber (from 10, increment 10)
RENUM 100               ' renumber starting at 100
RENUM 100, 30           ' renumber lines starting at 100, but only lines from old line 30
RENUM 100, 30, 5        ' renumber lines starting at 100, from old line 30, with increment 5


*/

class ProgramMemory {
public:
    ProgramMemory(); // Constructor

    void storeLine(int lineNumber, const std::string& code);

    // Function to store a line in memory
    void storeLine(const std::string& line);

    // Function to get all lines stored in memory
    std::map<int, std::string> getAllLines() const;

    std::vector<int> getLineNumbers() const;

    // Function to list all stored lines on the console
    void list();
    //new
    void clearMemory();

private:
    int line_counter;  // Counter to generate unique line numbers
    std::map<int, std::string> memory;  // Map to store lines with their corresponding line numbers
};

#endif // PROGRAM_MEMORY_H
