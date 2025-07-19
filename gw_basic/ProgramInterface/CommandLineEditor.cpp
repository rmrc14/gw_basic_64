//#include "CommandLineEditor.h"

//CommandLineEditor::CommandLineEditor() {
    // TODO: Implement CommandLineEditor
//}

#include "CommandLineEditor.h"
#include <iostream>
#include <fstream>
#include "SystemInterface.h" // Interface with System for input/output


    // vijay laxmi implement the command keys 
    // Simulate EOF handling if needed (you could check for Ctrl+Z or similar here)
    // For now, always return true unless you want to add special termination logic.

CommandLineEditor::CommandLineEditor() : cursorPosition(0) {}

bool CommandLineEditor::getLineFromCli(std::string& line) {
    buffer.clear();
    cursorPosition = 0;

    while (true) {
        check_SKey evt = SystemInterface::readKey();

        if (evt.isSpecial) {
            switch (evt.sKey) {
            case SpecialKey::LEFT:
                if (cursorPosition > 0) {
                    cursorPosition--;
                    SystemInterface::moveCursor(cursorPosition + 1, 1);  // x is 1-based
                }
                break;

            case SpecialKey::RIGHT:
                if (cursorPosition < buffer.length()) {
                    cursorPosition++;
                    SystemInterface::moveCursor(cursorPosition + 1, 1);
                }
                break;

            case SpecialKey::ESC:
                return false;  // Esc to cancel
            case SpecialKey::CTRL_Z:
                return false;  // Ctrl+Z as EOF simulation
            default:
                break;
            }
        }
        else {
            char c = evt.ch;

            if (c == '\b' && cursorPosition > 0) {
                buffer.erase(--cursorPosition, 1);
                SystemInterface::putChar('\b');
                SystemInterface::putChar(' ');
                SystemInterface::putChar('\b');
            }
            else if (c == '\r') {
                SystemInterface::putChar('\n');
                break;
            }
            else {
                buffer.insert(buffer.begin() + cursorPosition, c);
                ++cursorPosition;
                SystemInterface::putChar(c);
            }
        }
    }

    line = buffer;
    return true;
}