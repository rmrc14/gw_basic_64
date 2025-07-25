#include "gw_basic/GWBasic64.h"
#include "System/SystemInterface.h"

#include <iostream>
#include<string>

bool hasBasExtension(const std::string& filename);

int main(int argc, char* argv[])
{
    
    GWBasic64 basic;
    SystemInterface::init();  // for minios initialisation no need for windows
    try
    {
        if (argc > 1)
        {
            //check whethter its .bas file or not 
            std::string filename = argv[1];
            if(hasBasExtension(filename))
            {
                basic.loadAndRunFile(filename);  // File mode .bas file is given directly
            }
            else
            {
                SystemInterface::printString("Error: Only .bas files are supported.\n");
            }
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
    catch (...)
    {
        SystemInterface::printString("Fatal Error: Unknown fatal error occurred.\n");
    }

    return 0;
}

bool hasBasExtension(const std::string& filename)
{
    return filename.size() >= 4 &&
        filename.substr(filename.size() - 4) == ".bas";
}
