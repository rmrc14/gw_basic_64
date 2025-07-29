#include "CommandLineEditor.h"
#include "ScreenRenderer.h"
#include "SystemInterface.h"

CommandLineEditor::CommandLineEditor()
    : cursorPosition(0), specialKeyHandler(nullptr) {}

void CommandLineEditor::setSpecialKeyHandler(SpecialKeyHandler* handler) {
    specialKeyHandler = handler;
}

bool CommandLineEditor::getLineFromCli(std::string& line) {
    buffer.clear();
    cursorPosition = 0;

    //SystemInterface::printString("< ");

    while (true) {
        check_SKey key = SystemInterface::readKey();

        if (key.isSpecial) {
            if (specialKeyHandler && specialKeyHandler->handleSpecialKey(key, buffer)) {
                SystemInterface::putChar('\n');
                line = buffer;
                return true;
            }

            switch (key.sKey) {
            case SpecialKey::LEFT:
                if (cursorPosition > 0) {
                    cursorPosition--;
                    SystemInterface::printString("\x1B[D"); // Move left
                }
                break;
            case SpecialKey::RIGHT:
                if (cursorPosition < buffer.size()) {
                    cursorPosition++;
                    SystemInterface::printString("\x1B[C"); // Move right
                }
                break;
            case SpecialKey::ESC:
            case SpecialKey::CTRL_C:
                return false;
            default:
                break;
            }
        }
        else {
            char ch = key.ch;

            if (ch == '\r' || ch == '\n') {
                SystemInterface::putChar('\n');
                line = buffer;
                return true;
            }

            else if (ch == '\b' || ch == 127) {
                if (cursorPosition > 0) {
                    buffer.erase(buffer.begin() + cursorPosition - 1);
                    cursorPosition--;

                    // Move left, shift rest, clear trailing char
                    SystemInterface::printString("\b");
                    for (size_t i = cursorPosition; i < buffer.size(); ++i)
                        SystemInterface::putChar(buffer[i]);
                    SystemInterface::printString(" \b");

                    // Move cursor back to original spot
                    for (size_t i = cursorPosition + 1; i < buffer.size() + 1; ++i)
                        SystemInterface::printString("\x1B[D");
                }
            }

            else {
                buffer.insert(buffer.begin() + cursorPosition, ch);

                // Print character and shift rest
                for (size_t i = cursorPosition; i < buffer.size(); ++i)
                    SystemInterface::putChar(buffer[i]);
                cursorPosition++;

                // Move cursor back to correct spot
                for (size_t i = cursorPosition; i < buffer.size(); ++i)
                    SystemInterface::printString("\x1B[D");
            }
        }
    }
}
