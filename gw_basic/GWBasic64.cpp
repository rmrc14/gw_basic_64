#include "GWBasic64.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <cctype>

#define DIRECT_MODE -1

// intialising the object using default const.
GWBasic64::GWBasic64()   
	:lexer(),parser(),executer(),programMemory(),errorHandler(),console() {}


//-----------------   running .bas directly from main()     --------------

void GWBasic64::loadAndRunFile(const std::string& filename) 
{
	std::fstream fin(filename);

	if (!fin.is_open())
	{
		throw std::runtime_error("unable to open file: " + filename);
	}
	std::string line;

	while (std::getline(fin, line))
	{
		if(!line.empty())   // made true if contains string
		{
			programMemory.storeLine(line);
		}
	}
	fin.close();
	executeProgram(); // calls it execute the line stored in the memory module
}

void GWBasic64::executeProgram()
{
	//map<int, std::string> ProgramMemory

	auto allLines = programMemory.getAllLines();  //stores in deduced map of int &string

	// now need to call the lexer parser 
	for (const auto& [lineNo, line] : allLines)
	{
		try
		{
			auto tokens = lexer.tokenize(line);     //calls lexer to tokenize using enum
			auto ast = parser.parse(tokens);
			executer.execute(ast);
		}
		catch (const std::exception& e)
		{
			errorHandler.runtimeError(lineNo, e.what());//calls runtime error handler 
			break;  //exits the loop after error is found
			
		}

	}


}

// ------------------------   run interpreter line by line through REPL ------------

void GWBasic64::runREPL()
{
	// RUN LIST NEW  every newline OK  and > 
	// cmd-> 1 LIST 2 NEW 3 LOAD" 4 SAVE" 5 CONT<- 6 LPT1 7 TRON 8 TROFF 9 KEY 0 SCREEN
	std::string line;
	while (true)
	{
		std::cout << "Ok" << std::endl;  //needs to print first on every iteration
		std::cout << "> ";
		if (!std::getline(std::cin, line)) break;
		try
		{
			if (line.empty()) continue;  //user presses enter twice
			
			if (isdigit(line[0]))
			{
				programMemory.storeLine(line);  //stores in memory
			}
			else if (line=="RUN")
			{
				executeProgram();  //just calls directly as already stored in memory
			}
			else if (line == "LIST")
			{
				//todo
				std::cout << "todo\n";
				//programMemory.list(console);
			}
			else
			{
				// happens when user enters w/o lineNo. direct line execution
				executeLine(line);
			}
		}
		catch (const std::exception& e)
		{
			errorHandler.runtimeError(DIRECT_MODE, e.what());
			break; 

		}
	}

}



void executeLine(const std::string& line)
{

	auto tokens = lexer.tokenize(line);     //calls lexer to tokenize using enum
	auto ast = parser.parse(tokens);
	executer.execute(ast);


}