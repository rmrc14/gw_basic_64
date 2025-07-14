#include "GWBasic64.h"
#include <iostream>
#include <fstream>
#include <sstream>

GWBasic64::GWBasic64() {
    initialize();
}

void GWBasic64::initialize() {
    // TODO: Initialize components like ProgramMemory, Lexer, etc.
    std::cout << "[GWBasic64] Initialized\n";
}

void GWBasic64::runREPL() {
    std::cout << "GW-BASIC 64 Interpreter (REPL mode)\n";
    std::string line;
    while (true) {
        std::cout << "READY. ";
        if (!std::getline(std::cin, line)) break;
        if (line == "EXIT" || line == "QUIT") break;

        executeLine(line); // Process the command or statement
    }
}

void GWBasic64::loadAndRunFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cerr << "[Error] Cannot open file: " << filename << "\n";
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string line;
    while (std::getline(buffer, line)) {
        executeLine(line); // Process each line from the file
    }
}

void GWBasic64::executeLine(const std::string& line) {
    std::cout << "[Execute] " << line << "\n";
    // TODO: Tokenize, parse, and evaluate/execute line
}
