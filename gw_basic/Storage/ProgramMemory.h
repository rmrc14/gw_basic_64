// gw_basic/Storage/ProgramMemory.h

#ifndef PROGRAM_MEMORY_H
#define PROGRAM_MEMORY_H

#include <string>
#include <map>
#include <vector>

namespace gw_basic::storage {

    class ProgramMemory {
    private:
        std::map<int, std::string> programLines; // Stores line numbers and corresponding code

    public:
        // Inserts or updates a line of code
        void insertLine(int lineNumber, const std::string& code);

        // Deletes a line of code
        void deleteLine(int lineNumber);

        // Retrieves a line of code
        std::string getLine(int lineNumber) const;

        // Lists all program lines
        std::vector<std::string> listProgram() const;

        // Saves the program to a file
        bool saveToFile(const std::string& filename) const;

        // Loads the program from a file
        bool loadFromFile(const std::string& filename);

        // Clears all program lines
        void clear();
    };

} // namespace gw_basic::storage

#endif // PROGRAM_MEMORY_H
