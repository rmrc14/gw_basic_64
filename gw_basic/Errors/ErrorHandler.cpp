#include "ErrorHandler.h"

ErrorHandler::ErrorHandler() {
    // TODO: Implement ErrorHandler
}


void ErrorHandler::runtimeError(int linNo, const std::string& msg)
{
    std::string errorMsg;

    if (linNo == -1)
    {
        errorMsg = "[Runtime Error] (directMode): " + msg;
        SystemInterface::printString(errorMsg.c_str());
    }
    else
    {
        errorMsg = "[Runtime Error] line " + std::to_string(linNo)+" : " + msg;
        SystemInterface::printString(errorMsg.c_str());
        
    }
}

void ErrorHandler::syntaxError(int linNo, const std::string& msg)
{
    std::string errorMsg;
    if (linNo == -1)
    {
        //std::cerr << "[Syntax Error] directMode: " << msg << std::endl;
        errorMsg = "[Syntax Error] (directMode): " + msg;
        SystemInterface::printString(errorMsg.c_str());
    }
    else
    {
        //std::cerr << "[Syntax Error] line " << linNo << " : " << msg << std::endl;
        errorMsg = "[Syntax Error] line " + std::to_string(linNo) + " : " + msg;
        SystemInterface::printString(errorMsg.c_str());
    }
}