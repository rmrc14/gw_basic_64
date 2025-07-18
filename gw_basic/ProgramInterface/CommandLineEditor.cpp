//#include "CommandLineEditor.h"

//CommandLineEditor::CommandLineEditor() {
    // TODO: Implement CommandLineEditor
//}

#include "CommandLineEditor.h"
#include <iostream>
#include <fstream>
#include "SystemInterface.h" // Interface with System for input/output


CommandLineEditor::CommandLineEditor() : cursorPosition(0) {}

std::string CommandLineEditor::getLine() {
    currentLine.clear();
    cursorPosition = 0;
    char c;

    while (true) {
        c = SystemInterface::getChar(); // Get character input from system

        if (c == '\n') break;  // Enter key: End of input
        // Handle backspace if needed, but without calling handleEditing (excluded)
        if (c == '\b' && cursorPosition > 0) {  // Backspace: Handle editing
            currentLine.erase(--cursorPosition, 1);
            SystemInterface::putChar('\b');  // Move the cursor back and erase character
            SystemInterface::putChar(' ');   // Overwrite with space
            SystemInterface::putChar('\b');  // Move back to the cursor
        }
        else {
            currentLine += c;
            SystemInterface::putChar(c); // Echo the character
        }
    }
    SystemInterface::putChar('\n');  // Move to new line after input
    return currentLine;
}

bool CommandLineEditor::getLine(std::fstream& fin, std::string& line) {
    if (std::getline(fin, line)) {
        return true;
    }
    return false;
}

