#ifndef __ERROR__
#define __ERROR__

#include<iostream>


class ErrorHandler {
public:
    ErrorHandler();

    //errors
    void syntaxError(int line, const std::string& msg);
    void runtimeError(int, const std::string&); 
};

#endif