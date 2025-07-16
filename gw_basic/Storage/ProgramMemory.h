#pragma once
#include <string>
#include <map>
#include <vector>

//namespace gw_basic::storage {

class ProgramMemory {
public:
    void insertLine(int lineNumber, const std::string& code);
    void deleteLine(int lineNumber);
    std::string getLine(int lineNumber) const;
    std::vector<std::string> listProgram() const;

    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    void clear();

private:
    std::map<int, std::string> programLines;
};

//}
