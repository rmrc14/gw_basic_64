#ifndef LEXER_H
#define LEXER_H
#include<string>
#include<vector>

#include "Token.h"

//The Lexer class reads one line of Basiccode and breaks it into tokens
class Lexer {
public:

	//Lexer(string);
	Lexer();
	//This function does the main task of scanning and returning tokens
	std::vector<Token> tokenize(const std::string& inputLine);
private:
	std::string inputLine;//full line of code to process
	unsigned int currentIndex;//tracks which character we are looking at
	//Helper functions used while scanning the line
	void skipSpaces();
	char getCurrentChar() const;
	char moveToNextChar();
	//These function read specific parts of line and return tokens
	Token extractNumber();
	Token extractWordOrKeyword();
	Token extractCommentAfterRem();
	Token extractQuotedText();
	Token extractOperatororSymbol();



	static bool isKeyword(const std::string& word);
	static std::string toUpper(const std::string& s);



};
#endif // !LEXER_H
