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

    while (true) {
        check_SKey key = SystemInterface::readKey();
        // Let SpecialKeyHandler process F1?F10, etc.
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
                    ScreenRenderer::moveCursor(cursorPosition);
                }
                break;
            case SpecialKey::RIGHT:
                if (cursorPosition < buffer.size()) {
                    cursorPosition++;
                    ScreenRenderer::moveCursor(cursorPosition);
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
            else if (ch == '\b') {
                if (cursorPosition > 0) {
                    buffer.erase(buffer.begin() + cursorPosition - 1);
                    cursorPosition--;
                    ScreenRenderer::redrawBuffer(buffer, cursorPosition);
                }
            }
            else {
                buffer.insert(buffer.begin() + cursorPosition, ch);
                cursorPosition++;
                ScreenRenderer::redrawBuffer(buffer, cursorPosition);
            }
        }
    }
}

