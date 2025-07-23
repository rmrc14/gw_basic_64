#include "GWBasic64.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


#define DIRECT_MODE -1

// intialising the object using default const.
GWBasic64::GWBasic64()   
	:lexer(),parser(),executor(symbolTable),programMemory(),errorHandler(),cli() {}


//-----------------   running .bas directly from main()     --------------

void GWBasic64::loadAndRunFile(const std::string& filename) 
{

	if (!SystemInterface::openFile(filename))
	{
		throw std::runtime_error("unable to open file: " + filename);
	}
	
	std::string line;

	while (SystemInterface::readLineFromFile(line))// made true if contains string
	{
		programMemory.storeLine(line);
	}

	SystemInterface::closeFile();
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
			executor.execute(ast);
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

	//for auto mode
	bool automode = false;
	int currentAutoLine = 10;
	int autoIncrement = 10;

	SystemInterface::printString("Ok\n"); //needs to print first on every iteration
	while (true)
	{
		
		SystemInterface::printString("> ");

		if (!cli.getLineFromCli(line)) break;
		try
		{
			if (line.empty())
			{
				SystemInterface::printString("Ok\n");
				continue;  //user presses enter twice
			}
			
			if (isdigit(line[0]))
			{
				programMemory.storeLine(line);  //stores in memory
				
			}
			else if (line=="RUN")
			{
				executeProgram();  //just calls directly as already stored in memory
				SystemInterface::printString("Ok\n");
			}
			else if (line == "LIST")
			{
				programMemory.list();
				SystemInterface::printString("Ok\n");
			}
			else if (line == "NEW")
			{
				programMemory.clearMemory();
				SystemInterface::printString("Ok\n");
			}
			else if (line._Starts_with("EDIT "))
			{
				int ln = std::stoi(line.substr(5));
				if (!programMemory.lineExist(ln))
				{
					SystemInterface::printString("line number does not exist\n");
					continue;
				}
				std::string oldLine = programMemory.getLine(ln);
				SystemInterface::printString(oldLine.c_str());
				SystemInterface::printString("\n");

				std::string newLine;
				cli.getLineFromCli(newLine);

				if (newLine.empty())  // if user presses enter 2 times the line is deleted
				{
					programMemory.deleteLine(ln);
				}
				else
				{
					programMemory.storeLine(newLine);
				}
				SystemInterface::printString("Ok\n");
			}
			else if (line._Starts_with("SAVE ")) // SAVE to file-> SAVE "FILENAME.BAS"
			{
				std::string filename = line.substr(5);
				auto start = filename.find_first_of("\"") ;
				auto end = filename.find_last_of("\"") ;
				if (start != std::string::npos && end!=std::string::npos&& end>start)
				{
					filename = filename.substr(start+1, end-start-1); //start and length for substr
					programMemory.saveToFile(filename);
				}
				else
				{
					SystemInterface::printString("syntax error: SAVE \" filename.bas\".bas\n");
				}
				SystemInterface::printString("Ok\n");

			}
			else if (line._Starts_with("LOAD ")) //load from file
			{
				std::string filename =line.substr(5);
				auto start = filename.find_first_of("\"");
				auto end = filename.find_last_of("\"");
				if (start != std::string::npos && end != std::string::npos && end > start)
				{
					filename = filename.substr(start + 1, end - start - 1); //start and length for substr
					loadAndRunFile(filename);

				}
				else
				{
					SystemInterface::printString("syntax error: LOAD \" filename.bas\".bas\n");
				}
				SystemInterface::printString("Ok\n");
			}
			else if (line._Starts_with("DELETE "))
			{
				std::string arguments = line.substr(7); //rest of the line is stored
				auto findCommaPos = line.find(',');

				if (findCommaPos == std::string::npos)  // no comma means only one line number
				{
					int ln = std::stoi(arguments);
					programMemory.deleteLine(ln);
				}
				else    // 2 line number
				{
					int startLineNum = std::stoi(arguments.substr(0, findCommaPos));
					int endLineNum = std::stoi(arguments.substr(findCommaPos + 1));

					for (int ln = startLineNum; ln <= endLineNum;ln++)
					{
						programMemory.deleteLine(ln);
					}
				}
				SystemInterface::printString("Ok\n");
			}
			else if (line._Starts_with("AUTO "))
			{
				//AUTO             → start at 10, increment by 10
				//AUTO 100         → start at 100, increment 10
				//AUTO 100, 20     → start at 100, increment by 20


			}
			else if (line._Starts_with("RENUMBER "))
			{
				//(int newStart = 10, int oldStart = 0, int increment = 10
				//RENUMBER 100,200,5  — renumber from 200 as 100, 105, 
				//RENUMBER — defaults to 10,0,10
				//RENUMBER 100,200 — renumber lines from 200 with start=100
				std::istringstream iss(line.substr(9));  // Skip "RENUMBER "
				std::string token;
				int newStart = 10, oldStart = 0, increment = 10;

				if (std::getline(iss, token, ',')) 
					newStart = std::stoi(token);
				if (std::getline(iss, token, ',')) 
					oldStart = std::stoi(token);
				if (std::getline(iss, token, ',')) 
					increment = std::stoi(token);
				
				programMemory.renumber(newStart, oldStart, increment);
				SystemInterface::printString("Ok\n");


			}
			else if(line=="CLS")
			{
				SystemInterface::clearScreen();
				SystemInterface::printString("Ok\n");
			}
			else if (line == "CONT")
			{
				continue;
			}
			else if (line == "EXIT")
			{
				break;
			}
			else
			{
				// happens when user enters w/o lineNo. direct line execution
				executeLine(line);
				SystemInterface::printString("Ok\n");
			}


		}
		catch(const std::exception& e)
		{
			errorHandler.runtimeError(DIRECT_MODE, e.what());
			//break; 

		}
	}

}


void  GWBasic64::executeLine(const std::string& line)
{

	auto tokens = lexer.tokenize(line);     //calls lexer to tokenize using enum
	auto ast = parser.parse(tokens);
	executor.execute(ast);


}

