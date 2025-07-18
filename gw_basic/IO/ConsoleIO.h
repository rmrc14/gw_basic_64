#pragma once

#include <string>

class ConsoleIO {
public:
    ConsoleIO();

    // Reads a full line from the user input (e.g., BASIC command)
    std::string readLine();

    // Prints a string to the console
    void print(const std::string& text);

    // Prints a string followed by newline
    void printLine(const char* text);

    // Clears the screen
    void clear();

    // Moves the cursor to a specific location
    void moveCursor(int x, int y);

private:
    std::string buffer;  // Optional: can be used for advanced input editing later
};
