
#include <iostream>
#include<fstream>
#include <string>

#include "SystemInterface.h"

static std::fstream fin;

void SystemInterface::init(){}

void SystemInterface::putChar(char c)
{
    std::cout << c << std::flush;
}

char SystemInterface::getChar() 
{
    char c;
    std::cin.get(c);
    return c;
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
    if (std::getline(fin, Line)) {
        return true;  // Successfully read a line
    }
    else {
        return false; // EOF or read error
    }
}
