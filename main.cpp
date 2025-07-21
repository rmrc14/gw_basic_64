#include "gw_basic/GWBasic64.h"
#include "System/SystemInterface.h"

#include <iostream>
#include<string>

int main(int argc, char* argv[])
{
    //test version 14
    GWBasic64 basic;
    SystemInterface::init();  // for minios initialisation no need for windows
    try
    {
        if (argc > 1)
        {
            //todo check whethter its .bas file or not other wise error
            basic.loadAndRunFile(argv[1]);  // File mode .bas file is given directly
        }
        else
        {
            basic.runREPL();                // REPL mode READ EVAL PRINT LOOP
        }
    }
    catch (const std::exception& e)
    {
        SystemInterface::printString("Fatal Error: ");
        SystemInterface::printString(e.what());
        SystemInterface::printString("\n");
    }
	

    return 0;
}

