#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() {
    // TODO: Implement ErrorHandler
}


void ErrorHandler::runtimeError(int linNo, const std::string& msg)
{
    if (linNo == -1)
    {
        std::cerr << "[Runtime Error] (directMode): " << msg << std::endl;
    }
    else
    {
        std::cerr << "[Runtime Error] line " << linNo << " : " << msg << std::endl;
    }
}

void ErrorHandler::syntaxError(int linNo, const std::string& msg)
{
    if (linNo == -1)
    {
        std::cerr << "[Syntax Error] directMode: " << msg << std::endl;
    }
    else
    {
        std::cerr << "[Syntax Error] line " << linNo << " : " << msg << std::endl;
    }
}