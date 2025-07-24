#include "GWBasic64.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdexcept>


#define DIRECT_MODE -1

std::string trim(const std::string& s) 
{
	size_t start = s.find_first_not_of(" \t");
	if (start == std::string::npos) return "";
	size_t end = s.find_last_not_of(" \t");
	return s.substr(start, end - start + 1);
}


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
	/*bool automode = false;
	int currentAutoLine = 10;
	int autoIncrement = 10;*/

	SystemInterface::printString("Ok\n"); //needs to print first on every iteration
	while (true)
	{
		
		SystemInterface::printString("> ");

		if (!cli.getLineFromCli(line)) break;
		try
		{
			std::string trimmed = trim(line);
			if (trimmed.empty())
			{
				SystemInterface::printString("Ok\n");
				continue;  //user presses enter twice
			}
			
			if (isdigit(trimmed[0]))
			{
				programMemory.storeLine(trimmed);  //stores in memory
				
			}
			else if (trimmed =="RUN")
			{
				executeProgram();  //just calls directly as already stored in memory
				SystemInterface::printString("Ok\n");
			}
			else if (trimmed == "LIST")
			{
				programMemory.list();
				SystemInterface::printString("Ok\n");
			}
			else if (trimmed == "NEW")
			{
				programMemory.clearMemory();
				SystemInterface::printString("Ok\n");
			}
			else if (trimmed._Starts_with("EDIT "))
			{
				try {
					int ln = std::stoi(trimmed.substr(5));
					if (!programMemory.lineExist(ln)) 
					{
						SystemInterface::printString("Line number does not exist\n");
						continue;
					}
					std::string oldLine = programMemory.getLine(ln);
					SystemInterface::printString(oldLine.c_str());
					SystemInterface::printString("\n");

					std::string newLine;
					cli.getLineFromCli(newLine);

					if (newLine.empty()) 
					{
						SystemInterface::printString("No changes made.\n");
						//programMemory.deleteLine(ln);
					}
					else 
					{
						programMemory.storeLine(newLine);
					}
					SystemInterface::printString("Ok\n");
				}
				catch (...) {
					SystemInterface::printString("Invalid line number.\n");
				}
				
			}
			else if (trimmed._Starts_with("SAVE ")) // SAVE to file-> SAVE "FILENAME.BAS"
			{
				std::string filename = trimmed.substr(5);
				auto start = filename.find_first_of("\"") ;
				auto end = filename.find_last_of("\"") ;
				if (start != std::string::npos && end!=std::string::npos&& end>start)
				{
					filename = filename.substr(start+1, end-start-1); //start and length for substr
					programMemory.saveToFile(filename);
				}
				else
				{
					SystemInterface::printString("syntax error: SAVE \" filename.bas\"\n");
				}
				SystemInterface::printString("Ok\n");

			}
			else if (trimmed._Starts_with("LOAD ")) //load from file
			{
				std::string filename = trimmed.substr(5);
				auto start = filename.find_first_of("\"");
				auto end = filename.find_last_of("\"");
				auto checkBas = filename.find(".bas");

				if (start != std::string::npos && end != std::string::npos && end > start 
					 && checkBas != std::string::npos)
				{
					if (start + 1 >= end)   // if the filename is empty warning to be given
					{
						SystemInterface::printString("filename cannot be empty\n");
						continue;
					}

					filename = filename.substr(start + 1, end - start - 1); //start and length for substr
					loadAndRunFile(filename);

				}
				else
				{
					SystemInterface::printString("syntax error: use LOAD \" filename.bas\".bas\n");
				}
				SystemInterface::printString("Ok\n");
			}
			else if (trimmed._Starts_with("DELETE "))
			{
				std::string arguments = trimmed.substr(7); //rest of the line is stored
				auto findCommaPos = arguments.find(',');

				if (findCommaPos == std::string::npos)  // no comma means only one line number
				{
					try
					{
						int ln = std::stoi(arguments);
						programMemory.deleteLine(ln);
					}
					catch (...)
					{
						SystemInterface::printString("Invalid line number\n");
					}
				}
				else    // 2 line number
				{
					try
					{
						int startLineNum = std::stoi(arguments.substr(0, findCommaPos));
						int endLineNum = std::stoi(arguments.substr(findCommaPos + 1));
						if (startLineNum > endLineNum) {
							SystemInterface::printString("Invalid range: start > end\n");
							continue;
						}

						for (int ln = startLineNum; ln <= endLineNum;ln++)
						{
							programMemory.deleteLine(ln);
						}
					}
					catch (...)
					{
						SystemInterface::printString("Invalid line number\n");
					}

					
				}
				SystemInterface::printString("Ok\n");
			}
			else if (trimmed._Starts_with("AUTO ")|| trimmed =="AUTO")
			{
				//AUTO             - start at 10, increment by 10
				//AUTO 100         - start at 100, increment 10
				//AUTO 100, 20     - start at 100, increment by 20
				int start = 10, increment = 10;
				std::string token;

				std::string args = (trimmed.length() > 4) ? trimmed.substr(4) : "";
				args.erase(0, args.find_first_not_of(" \t")); // trim leading spaces

				std::istringstream iss(args);

				if (std::getline(iss, token, ','))
				{
					try
					{
						start = std::stoi(token);  //stoi will fail for non integer
					}
					catch (...)
					{
						start = 10;
					}
				}
				if (std::getline(iss, token, ','))
				{
					try
					{
						increment = std::stoi(token);
					}
					catch (...)
					{
						increment = 10;
					}
				}

				int currentLine = start;
				/* 
				 inside an auto loop 
				need to print ~ for auto mode
				* should exit auto mode after empty line 
				* use storeLine(int lineNumber, const std::string& code) for storing
				
				*/
				while(true)
				{
					std::string toPrintAutoNumLine = std::to_string(currentLine) + "~ ";
					SystemInterface::printString(toPrintAutoNumLine.c_str());

					std::string code;
					if (!cli.getLineFromCli(code))  //gets code line and also verifies any break statement or interupt happens in getline
					{
						break; // exits if true --- error occurs during geting line
					}
					if (code.empty())
					{
						break; //exits the auto mode if user press enter 2 times 
					}
					programMemory.storeLine(currentLine, code);
					currentLine += increment;
				}
				SystemInterface::printString("Ok\n");

			}
			else if (trimmed._Starts_with("RENUMBER "))
			{
				//(int newStart = 10, int oldStart = 0, int increment = 10
				//RENUMBER 100,200,5  — renumber from 200 as 100, 105, 
				//RENUMBER — defaults to 10,0,10
				//RENUMBER 100,200 — renumber lines from 200 with start=100
				std::istringstream iss(trimmed.substr(9));  // Skip "RENUMBER "
				std::string token;
				int newStart = 10, oldStart = 0, increment = 10;

				if (std::getline(iss, token, ',')) 
				{
					try
					{
						newStart = std::stoi(token);
					}
					catch (...)
					{
						newStart = 10;
					}
				}
				if (std::getline(iss, token, ',')) 
				{
					try
					{
						oldStart = std::stoi(token);
					}
					catch (...)
					{
						oldStart = 0;
					}
				}
				if (std::getline(iss, token, ',')) 
				{
					try
					{
						increment = std::stoi(token);
					}
					catch (...)
					{
						increment = 10;
					}
				}
				if (increment <= 0||newStart < 0 ||oldStart < 0) {
					SystemInterface::printString("Value cannot be negative\n");
					continue;
				}

				programMemory.renumber(newStart, oldStart, increment);
				SystemInterface::printString("Ok\n");


			}
			else if(trimmed =="CLS")
			{
				SystemInterface::clearScreen();
				SystemInterface::printString("Ok\n");
			}
			else if (trimmed == "CONT")
			{
				continue;
			}
			else if (trimmed == "EXIT")
			{
				break;
			}
			else
			{
				// happens when user enters w/o lineNo. direct line execution
				executeLine(trimmed);
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

