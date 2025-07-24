#ifndef __ERROR__
#define __ERROR__

#include<iostream>
#include<string>
#include "System/SystemInterface.h"

class ErrorHandler {
public:
    ErrorHandler();

    //errors
    void syntaxError(int line, const std::string& msg);
    void runtimeError(int, const std::string&); 
    void typeError(int, const std::string&);
    void systemError(const std::string&);
};

#endif