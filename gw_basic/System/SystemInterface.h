#pragma once

namespace SystemInterface {

    // Console I/O
    void putChar(char c);
    char getChar();
    void printString(const char* str);

    // Screen/UI
    void clearScreen();
    void moveCursor(int x, int y);


    // Setup
    void init();

    
}
