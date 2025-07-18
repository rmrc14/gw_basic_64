#pragma once

class SystemInterface {
    
public:
    // Setup
    static void init();

    // Console I/O
    static void putChar(char c);
    static char getChar();
    static void printString(const char* str);
    // File IO
    static bool openFile(const std::string& filename);
    static void closeFile();
    static bool readLineFromFile(std::string&);
    // Screen/UI
    static void clearScreen();
    static void moveCursor(int x, int y);
   
};
