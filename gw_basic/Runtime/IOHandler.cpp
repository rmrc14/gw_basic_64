#include "IOHandler.h"
#include <iostream>

    

    std::string IOHandler::getInput(const std::string & prompt) {
        std::cout << prompt;
        std::string input;
        std::getline(std::cin, input);
        return input;
}
