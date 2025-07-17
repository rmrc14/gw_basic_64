#ifndef __GWBasics64__
#define __GWBasics64__

#include "Lexer/Lexer.h"
#include "Parser/Parser.h"
#include "Storage/ProgramMemory.h"
#include "Errors/ErrorHandler.h"
#include "IO/ConsoleIO.h"
#include "Runtime/StatementExecuter.h"
#include "ProgramInterface/CommandLineEditor.h"

#include <string>

class GWBasic64 {

private:
    
    //module objects instantiation
    Lexer lexer;
    Parser parser;
    StatementExecuter executer;
    ProgramMemory programMemory;
    ErrorHandler errorHandler;
    ConsoleIO console;

    // member function
    void executeProgram();
    void executeLine(const std::string& line);

public:
    GWBasic64();

    void runREPL();       // Starts REPL (interactive shell)
    void loadAndRunFile(const std::string& filename);   // Loads and runs a .BAS file
};


#endif