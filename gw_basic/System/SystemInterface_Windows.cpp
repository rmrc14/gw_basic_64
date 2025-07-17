
#include <iostream>

#include "SystemInterface.h"

namespace SystemInterface {

    void init() {}

    void putChar(char c) {
        std::cout << c << std::flush;
    }

    char getChar() {
        char c;
        std::cin.get(c);
        return c;
    }

    void printString(const char* str) {
        std::cout << str << std::flush;
    }

    void clearScreen() {
        std::cout << "\x1B[2J\x1B[H"; //Clear the entire screen (\x1B[2J)
                                       //Move the cursor to top - left(\x1B[H)   
    }


    //Moves the cursor to position(x, y) on the terminal.

    //    ANSI escape sequence :
    //    \x1B[<row>;<col>H
    void moveCursor(int x, int y) {
        std::cout << "\x1B[" << y << ";" << x << "H";
    }

    

}
