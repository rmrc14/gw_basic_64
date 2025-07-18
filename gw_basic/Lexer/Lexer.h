#ifndef LEXER_H
#define LEXER_H
#include<string>
#include<vector>

#include "Token.h"
using namespace std;
//The Lexer class reads one line of Basiccode and breaks it into tokens
class Lexer {
public:

	//Lexer(string);
	Lexer();
	//This function does the main task of scanning and returning tokens
	std::vector<Token> tokenize(const std::string& inputLine);
private:
	string inputLine;//full line of code to process
	unsigned int currentIndex;//tracks which character we are looking at
	//Helper functions used while scanning the line
	void skipSpaces();
	char getCurrentChar() const;
	char moveToNextChar();
	//These function read specific parts of line and return tokens
	Token extractNumber();
	Token extractWordOrKeyword();
	Token extractQuotedText();
	Token extractCommentAfterREM();
	static bool isKeyword(const string& word);



};
#endif // !LEXER_H
