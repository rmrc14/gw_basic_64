#pragma once
#include <string>
#include <vector>

class FileIO {
public:
    // Reads all lines from a text file
    static std::vector<std::string> readLines(const std::string& path);

    // Writes lines to a text file
    static void writeLines(const std::string& path, const std::vector<std::string>& lines);
};
