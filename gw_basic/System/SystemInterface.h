#pragma once
#include<string>
#include<vector>

enum class SpecialKey
{
    UP, DOWN, LEFT, RIGHT,
    F1, F2, F3, F4, F5,
    F6, F7, F8, F9, F10,
    ESC,
    CTRL_Z,
    CTRL_C,
    NOT_DEFINED_KEY
};

struct check_SKey
{
    bool isSpecial;  // to differentiate btw special key and simple characters
    char ch;         // valid when isspecial= false
    SpecialKey sKey; //  isspecial = true 

};


class SystemInterface {
    
public:
    // Setup
    static void init();

    // Console I/O
    static check_SKey readKey();  // need to differntiate keys typed
    static void putChar(char c);
    static char getChar();
    static void printString(const char* str);
    // File IO
    static bool openFile(const std::string& filename);
    static void closeFile();
    static bool readLineFromFile(std::string&);
    static void createAndSaveFile(const std::string& path, const std::vector<std::string>& lines);
    // Screen/UI
    static void clearScreen();
    static void moveCursor(int x, int y);
   

};
