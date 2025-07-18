#include "gw_basic/GWBasic64.h"
#include <iostream>
#include<string>

int main(int argc, char* argv[])
{
    //test version 14
    GWBasic64 basic;
    try
    {
        if (argc > 1)
        {
            basic.loadAndRunFile(argv[1]);  // File mode .bas file is given directly
        }
        else
        {
            basic.runREPL();                // REPL mode READ EVAL PRINT LOOP
        }
    }
    catch (const std::exception& e)
    {
        std::cerr << "Fatal Error: " << e.what() << std::endl;
    }
	

    return 0;
}

