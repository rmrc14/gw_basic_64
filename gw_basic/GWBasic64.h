#pragma once

#include <string>

class GWBasic64 {
public:
    GWBasic64();

    // Starts REPL (interactive shell)
    void runREPL();

    // Loads and runs a .BAS file
    void loadAndRunFile(const std::string& filename);

private:
    // Internal helpers (may be implemented later)
    void initialize();
    void executeLine(const std::string& line);

    // Future member: ProgramMemory, Lexer, Parser, Executor, etc.
};
