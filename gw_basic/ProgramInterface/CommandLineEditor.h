#pragma once


#include <string>
#include <fstream>
#include "SystemInterface.h"
#include "GWBasic64.h"

class CommandLineEditor {
public:
    CommandLineEditor();

    // Captures user input line by line (Windows or custom OS)
    bool getLineFromCli(std::string& line);


private:
    std::string buffer;   // Holds the current line of input
    size_t cursorPosition;     // Tracks the current position of the cursor in the input
};


