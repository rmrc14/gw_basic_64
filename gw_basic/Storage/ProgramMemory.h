
/*class ProgramMemory {
public:
    ProgramMemory();

    void storeLine(std::string);//todo magesh
    /*void ProgramMemory::storeLine(const std::string&);
    std::map<int, std::string> ProgramMemory::getAllLines();
    void ProgramMemory::list(ConsoleIO&);*/
    // ProgramMemory.h*/
#pragma once

#include <map>
#include <string>
#include <vector>

    class ProgramMemory {
    public:
        // Constructor
        ProgramMemory();

        // Store a line in memory (like entering "10 PRINT \"HELLO\"")
        void storeLine(const std::string& inputLine);

        // Return all stored lines in order for listing or saving
        std::vector<std::string> getAllLines() const;

        // Clear all stored program lines
        void clear();

    private:
        std::map<int, std::string> programLines;

        // Helper: parse a line like "10 PRINT \"HELLO\"" into {lineNumber, code}
        bool parseLine(const std::string& inputLine, int& lineNumber, std::string& code);
    };

