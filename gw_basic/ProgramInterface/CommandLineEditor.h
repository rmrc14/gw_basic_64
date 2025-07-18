#pragma once
//class CommandLineEditor {
//public:
 //   CommandLineEditor();
    //std::string CommandLineEditor::getLine(); todo vijaylaxmi

//};


#include <string>
#include <fstream>
#include "SystemInterface.h"
#include "GWBasic64.h"

class CommandLineEditor {
public:
    CommandLineEditor();

    // Captures user input line by line (Windows or custom OS)
    std::string getLine();

    // Reads a line of input from a file (e.g., for file-based execution)
    bool getLine(std::fstream& fin, std::string& line);

private:
    std::string currentLine;   // Holds the current line of input
    size_t cursorPosition;     // Tracks the current position of the cursor in the input
};


