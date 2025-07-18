#include "ConsoleIO.h"
#include "SystemInterface.h"

ConsoleIO::ConsoleIO() {
    SystemInterface::init(); // Optional setup for minios , not for windows
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
