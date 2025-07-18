#include "ConsoleIO.h"
#include "SystemInterface.h"

ConsoleIO::ConsoleIO() {
    SystemInterface::init(); // Optional setup
}

std::string ConsoleIO::readLine() {
    std::string input;
    char c;
    while (true) {
        c = SystemInterface::getChar();

        if (c == '\r' || c == '\n') {
            SystemInterface::putChar('\n');
            break;
        }
        else if (c == '\b' || c == 127) {  // handle backspace
            if (!input.empty()) {
                input.pop_back();
                SystemInterface::putChar('\b');
                SystemInterface::putChar(' ');
                SystemInterface::putChar('\b');
            }
        }
        else {
            input += c;
            SystemInterface::putChar(c);
        }
    }
    return input;
}

void ConsoleIO::print(const std::string& text) {
    SystemInterface::printString(text.c_str());
}

void ConsoleIO::printLine(const char* text) {
    SystemInterface::printString(text);
    SystemInterface::putChar('\n');
}

void ConsoleIO::clear() {
    SystemInterface::clearScreen();
}

void ConsoleIO::moveCursor(int x, int y) {
    SystemInterface::moveCursor(x, y);
}
