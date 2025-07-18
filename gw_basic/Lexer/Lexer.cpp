#include "Lexer.h"
#include<cctype>//for character checks
#include<vector>//for storing keywords as simple list
#include "Token.h"
using namespace std;
//basic keywords stored as a simple list
const vector<string> Keywords = {
	"PRINT",
	"INPUT",
	"LET",
	"IF",
	"THEN",
	"GOTO",
	"FOR",
	"TO",
	"NEXT",
	"REM",
	"END",


};
//Lexer::Lexer(string input ):currentIndex(0){}
Lexer::Lexer() :currentIndex(0) {}
//get the character at currentIndex
char Lexer::getCurrentChar()const {
	if (currentIndex < inputLine.length()) {
		return inputLine[currentIndex];
	}
	return '\0'; //return null char if end of input
}
//move to the next character and return it
char Lexer::moveToNextChar() {
	if (currentIndex < inputLine.length()) {
		return inputLine[currentIndex++];
	}
	return '\0';
}
//skip all spaces and tabs
void Lexer::skipSpaces() {
	while (std::isspace(getCurrentChar())) {
		moveToNextChar();

	}
}
//check if a guven word is a Basic Keyword
bool Lexer::isKeyword(const std::string& word) {
	for (const std::string& keyword : Keywords) {
		if (word == keyword) {
			return true;
		}
	}
	return false;
}
//Reads numbers 
Token Lexer::extractNumber() {
	std::string numberText;
	unsigned int startPos = currentIndex;
	while (std::isdigit(getCurrentChar())) {
		numberText += moveToNextChar();
	}
	return Token(TokenType::Number, numberText, startPos);
}
//Read either a keyword or a identifier
Token Lexer::extractWordOrKeyword() {
	std::string word;
	unsigned int startPos = currentIndex;
	while (std::isalnum(getCurrentChar())) {
		word = word + moveToNextChar();
	}
	//REM is a special  case:everything after its a comment
	if (word == "REM") {
		return extractCommentAfterREM();

	}
	if (isKeyword(word)) {
		return Token(TokenType::Keyword, word, startPos);
	}
	return Token(TokenType::Identifier, word, startPos);
}
//Read a string inside double quotes
Token Lexer::extractQuotedText() {
	size_t startPos = currentIndex;
	moveToNextChar();//skip opening quote
	std::string text;
	while (getCurrentChar() != '"' && getCurrentChar() != '\0') {
		text += moveToNextChar();
	}
	moveToNextChar();//skip closing quote
	return Token(TokenType::String, text, startPos);
}
//Read everything after REM as a comment
Token Lexer::extractCommentAfterREM() {
	size_t startPos = (currentIndex >= 3) ? currentIndex - 3 : 0;//REM was already read
	std::string commentText = "REM";
	while (getCurrentChar() != '\0') {
		commentText += moveToNextChar();

	}
	return Token(TokenType::Comment, commentText, startPos);
}
//This is the main function that process the whole input line



std::vector <Token>Lexer::tokenize(const std::string& inputLine) {
	this->inputLine = inputLine;//Fixed
	currentIndex = 0;//Reset each time
	std::vector<Token>tokens;
	while (currentIndex < inputLine.length()) {
		skipSpaces();//ignore spaces
		char currentChar = getCurrentChar();
		if (currentChar == '\0')
			break;
		if (std::isdigit(currentChar)) {
			tokens.push_back(extractNumber());//its a number extract it
		}
		else if (std::isalpha(currentChar)) {
			tokens.push_back(extractWordOrKeyword());//if its a letter,it may be a keyword or variable
		}
		else if (currentChar == '"') {
			//if its a quote,its a string
			tokens.push_back(extractQuotedText());
		}
		else if (std::ispunct(currentChar)) {
			//its handle punctuator
			std::string symbol(1, moveToNextChar());
			tokens.emplace_back(TokenType::Symbol, symbol, currentIndex - 1);
		}
		else {
			//unknown character -just skip it
			moveToNextChar();
		}
		//currentIndex++;
	}
	//atlast add an endofline token
	tokens.emplace_back(TokenType::END_OF_LINE, "", currentIndex);
	return tokens;
}
