//#include "CommandLineEditor.h"

//CommandLineEditor::CommandLineEditor() {
    // TODO: Implement CommandLineEditor
//}

#include "CommandLineEditor.h"
#include <iostream>
#include <fstream>
#include "SystemInterface.h" // Interface with System for input/output


CommandLineEditor::CommandLineEditor() : cursorPosition(0) {}

bool CommandLineEditor::getLineFromCli(std::string& line) {
    buffer.clear();
    cursorPosition = 0;
    char c;

    while (true) {
        c = SystemInterface::getChar();  // Get character

        if (c == '\n') break;

        // Handle backspace
        if (c == '\b' && cursorPosition > 0) {
            buffer.erase(--cursorPosition, 1);
            SystemInterface::putChar('\b');
            SystemInterface::putChar(' ');
            SystemInterface::putChar('\b');
        }
        else {
            buffer += c;
            ++cursorPosition;
            SystemInterface::putChar(c); // Echo
        }
    }

    SystemInterface::putChar('\n');

    line = buffer;


    // vijay laxmi implement the command keys 
    // Simulate EOF handling if needed (you could check for Ctrl+Z or similar here)
    // For now, always return true unless you want to add special termination logic.
    return true;
}
