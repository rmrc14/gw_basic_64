

/*void saveToFile(std::string filename)

bool lineExist(int linenumber)  // gives true if line found in memory

void ProgramMemory::deleteLine(int lineNumber ,int secondline number)
DELETE 100         ' Deletes line 100
DELETE 100,200     ' Deletes lines 100 through 200 (inclusive)



std::string getLine(int lineNumber) const;

void ProgramMemory::renumber(int newStart, int oldStart, int increment=10);


RENUM                   ' default renumber (from 10, increment 10)
RENUM 100               ' renumber starting at 100
RENUM 100, 30           ' renumber lines starting at 100, but only lines from old line 30
RENUM 100, 30, 5        ' renumber lines starting at 100, from old line 30, with increment 5


*/



#ifndef PROGRAM_MEMORY_H
#define PROGRAM_MEMORY_H

#include <string>
#include <map>
#include <vector>


class SystemInterface; // Forward declaration if needed

class ProgramMemory {
public:
    ProgramMemory();

    // Store a line by number and code
    void storeLine(int lineNumber, const std::string& code);

    // Store a line like "10 PRINT HELLO"
    void storeLine(const std::string& line);

    // Get a copy of all stored lines
    std::map<int, std::string> getAllLines() const;

    // Get just the line numbers
   // std::vector<int> getLineNumbers() const;

    std::vector<std::string> getLineNumbers() const;


    // List all lines using system output
    void list() const;

//clear    void clearMemory();

    // File I/O
    bool loadFromFile(const std::string& path);
    bool saveToFile(const std::string& path) const;

    // Utilities
    bool lineExist(int lineNumber) const;
    void deleteLine(int lineNumber);
    std::string getLine(int lineNumber) const;

    // Renumber lines
    void renumber(int newStart = 10, int oldStart = 0, int increment = 10);
    int getFirstLineNumber() const;
    int getNextLineNumber(int current) const;

    void clearMemory();


private:
    int line_counter = 0;  // Unused currently, can be used for auto-numbering
    std::map<int, std::string> memory;
};

#endif // PROGRAM_MEMORY_H



