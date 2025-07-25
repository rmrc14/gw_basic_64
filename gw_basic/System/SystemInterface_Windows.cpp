
#include <iostream>
#include<fstream>
#include <string>
#include<conio.h>

#include "SystemInterface.h"

// DOS/Windows special key prefix
#define SPECIAL_KEY_BYTES_PREFIX_1 0x00 // NULL for the esc,function keys,etc
#define SPECIAL_KEY_BYTES_PREFIX_2 0xE0 // Extended key prefix (arrow key etc)




static std::fstream fin,fio;


void SystemInterface::init(){}

/*
Arrow Up: [0xE0][72]
F1:       [0][59]
 UP = 72,          // Arrow Up (after 0 or 0xE0)
    DOWN = 80,        // Arrow Down
    LEFT = 75,        // Arrow Left
    RIGHT = 77,       // Arrow Right
*/
check_SKey SystemInterface::readKey()
{
    int ch = _getch();

    // means special key with 2 bytes
    if (ch == SPECIAL_KEY_BYTES_PREFIX_1 || ch == SPECIAL_KEY_BYTES_PREFIX_2) 
    {
        int code = _getch();  // second getch gives the special key value
        switch (code)
        {
        case 59: return { true, 0, SpecialKey::F1 }; // 59
        case 60: return { true, 0, SpecialKey::F2 }; // 60
        case 61: return { true, 0, SpecialKey::F3 }; //  F3 = 61,
        case 62: return { true, 0, SpecialKey::F4 }; //  F4 = 62,
        case 63: return { true, 0, SpecialKey::F5 }; //  F5 = 63,
        case 64: return { true, 0, SpecialKey::F6 }; //  F6 = 64,
        case 65: return { true, 0, SpecialKey::F7 }; //  F7 = 65,
        case 66: return { true, 0, SpecialKey::F8 }; //  F8 = 66,
        case 67: return { true, 0, SpecialKey::F9 }; //  F9 = 67,
        case 68: return { true, 0, SpecialKey::F10 };//  F10 = 68,
             
          
        case 72: return { true, 0, SpecialKey::UP };
        case 80: return { true, 0, SpecialKey::DOWN };
        case 75: return { true, 0, SpecialKey::LEFT };
        case 77: return { true, 0, SpecialKey::RIGHT };
        
        
        default: return { true, 0, SpecialKey::NOT_DEFINED_KEY };
        }
    }
    else
    {
        if (ch == 27)  return { true, 0, SpecialKey::ESC };
        if (ch == 3)   return { true, 0, SpecialKey::CTRL_C };
        if (ch == 26)  return { true, 0, SpecialKey::CTRL_Z };
        return { false, static_cast<char>(ch), SpecialKey::NOT_DEFINED_KEY };
    }
}


void SystemInterface::putChar(char c)
{
    std::cout << c << std::flush;
}

char SystemInterface::getChar()  // decided to go with keyboard strokes
{
    return _getch();
}

void SystemInterface::printString(const char* str) 
{
    std::cout << str << std::flush;
}

void SystemInterface::clearScreen()
{
    std::cout << "\x1B[2J\x1B[H"; //Clear the entire screen (\x1B[2J)
    //Move the cursor to top - left(\x1B[H)   
}


//Moves the cursor to position(x, y) on the terminal.

//    ANSI escape sequence :
//    \x1B[<row>;<col>H
void SystemInterface::moveCursor(int x, int y) 
{
    std::cout << "\x1B[" << y << ";" << x << "H";
}

void SystemInterface::drawStatusBar()
{
    // Save current cursor position
    std::cout << "\033[s";

    // Move to bottom line (line 24, column 1)
    std::cout << "\033[30;1H";

   // std::cout << "\033[2K";          // Clear the line
    // Print status bar
    std::cout << "1LIST   2RUN    3LOAD   4SAVE   5CONT   6LPT    7TRON   8TROFF   9KEY   0SCREEN";

    // Restore cursor position
    std::cout << "\033[u";

    std::cout.flush(); // Ensure everything is output immediately
}

// -------------------------- file handling ---------------

bool SystemInterface::openFile(const std::string& filename) 
{
    fin.open(filename);
    return fin.is_open();
}
void  SystemInterface::closeFile()
{
    if (fin.is_open()) fin.close();
}

bool SystemInterface::readLineFromFile(std::string& Line)
{
    if (std::getline(fin, Line)) 
    {
        return true;  // Successfully read a line
    }
    else 
    {
        return false; // EOF or read error
    }
}


// file handling module for memory
void SystemInterface::createAndSaveFile(const std::string& path, const std::vector<std::string>& lines)
{
    fio.open(path,std::ios::out);
    if (!fio.is_open())
    {
        throw std::runtime_error("unable to open file: " + path);
    }
    for (const auto& l : lines)
    {
        fio<<l<<"\n";
    }

    fio.close();
}
    