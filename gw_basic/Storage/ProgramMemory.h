#ifndef PROGRAM_MEMORY_H
#define PROGRAM_MEMORY_H

#include <string>
#include <map>

// Forward declaration of ConsoleIO class
class ConsoleIO;

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
