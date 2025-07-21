#include "Lexer.h"

#include<cctype>//for character checks
#include<vector>//for storing keywords as simple list
#include<algorithm>
#include<iostream>



//basic keywords stored as a simple list
const std::vector<std::string> Keywords = {
	"PRINT", "INPUT", "LET", "IF", "THEN", "ELSE", "GOTO", "GOSUB", "RETURN",
	"FOR", "TO", "STEP", "NEXT", "WHILE", "WEND", "DO", "LOOP", "STOP", "END",
	"NEW", "RUN", "LIST", "LOAD", "SAVE", "SYSTEM", "EDIT", "CONT", "DELETE",
	"DIR", "OPEN", "CLOSE", "FIELD", "WRITE", "LPRINT", "DATA", "READ",
	"RESTORE", "GET", "PUT", "BLOAD", "BSAVE", "ON", "ERROR", "RESUME", "CLEAR",
	"ERL", "ERR", "ABS", "ASC", "ATN", "COS", "SIN", "TAN", "LOG", "EXP", "SQR",
	"RND", "LEN", "LEFT$", "RIGHT$", "MID$", "TIME$", "DATE$", "INSTR", "VAL",
	"PLOT", "LINE", "CIRCLE", "DRAW", "PAINT", "SOUND", "PLAY", "SCREEN",
	"AND", "OR", "NOT", "XOR", "EQV", "IMP", "DEFINT", "DEFSNG", "DEFDBL", "DEFSTR", "DEF"

};
//Lexer::Lexer(string input ):currentIndex(0){}
Lexer::Lexer() :currentIndex(0) {}
//Main function:Scans and splits input into tokens
std::vector<Token> Lexer::tokenize(const std::string& line) {
	inputLine = line;
	currentIndex = 0;
	std::vector<Token>tokens;

	skipSpaces();
	//Line number validation
	if (std::isdigit(getCurrentChar())) {
		unsigned int lineStart = currentIndex;
		Token num = extractNumber();
		if (getCurrentChar() == ' ') {
			tokens.push_back(num);//treated as line number
			skipSpaces();
		}
		else {
			currentIndex = lineStart;//rollback
		}
	}
	while (currentIndex < inputLine.size()) {
		skipSpaces();//skip any spaces
		char c = getCurrentChar();
		if (c == '\0')
			break;//end of line
		if (std::isdigit(c))
		{
			tokens.push_back(extractNumber());
		}

		else if (std::isalpha(c)) {
			tokens.push_back(extractWordOrKeyword());//handles identifiers/keyword
		}
		else if (c == '\"') {
			tokens.push_back(extractQuotedText());//handles string
		}
		else if (std::ispunct(c)) {
			tokens.push_back(extractOperatororSymbol());
		}
		else {
			//Invalid character
			std::string invalid(1, c);
			tokens.emplace_back(TokenType::Invalid, invalid, currentIndex);
			moveToNextChar();//skip any unknown character
		}
	}
	//Add an END_OF_LINE token to mark end
	tokens.emplace_back(TokenType::END_OF_LINE, "", currentIndex);
	return tokens;
}



//skip white spaces (space,tab,etc)
void Lexer::skipSpaces() {
	while (std::isspace(getCurrentChar())) {
		moveToNextChar();
	}
}
//gets the current character
char Lexer::getCurrentChar()const {
	return(currentIndex < inputLine.size()) ? inputLine[currentIndex] : '\0';
}
//Moves to the next character and returns it
char Lexer::moveToNextChar() {
	return(currentIndex < inputLine.size()) ? inputLine[currentIndex++] : '\0';

}
//extract a anumber token(like 10,20)
Token Lexer::extractNumber() {
	unsigned int start = currentIndex;
	std::string num;
	bool hasPoint = false;//to check float
	while (std::isdigit(getCurrentChar()) ||
		getCurrentChar() == '.' ||
		getCurrentChar() == 'E' ||
		getCurrentChar() == '+' ||
		getCurrentChar() == '-') {
		char ch = getCurrentChar();
		if (ch == '.') {
			if (hasPoint)
				break;//only one dot allowed
			hasPoint = true;
		}
		num += moveToNextChar();
	}
	return Token(TokenType::Number, num, start);
}
//Extracts either a keyword(like print) or identifier(like A$)
Token Lexer::extractWordOrKeyword() {
	unsigned int start = currentIndex;
	std::string word;
	//collect alphanumeric characters and $(eg A$,VAR1)
	while (std::isalnum(getCurrentChar()) || getCurrentChar() == '$') {
		word += moveToNextChar();
	}
	//Convert to uppercase for keyword comparison
	std::string upper = toUpper(word);
	//handles REM comments 
	if (upper == "REM") {
		return extractCommentAfterRem();

	}
	//check if it is a keyword
	if (isKeyword(upper)) {
		return Token(TokenType::Keyword, word, start);
	}
	//otherwise,its a identifier(variable/function name)
	return Token(TokenType::Identifier, word, start);
}
//extract string literals
Token Lexer::extractQuotedText()
{
	unsigned int start = currentIndex;
	moveToNextChar();//skip opening quote
	std::string text;
	//keep collecting characters until closing quote or end of line
	while (getCurrentChar() != '\0') {
		if (getCurrentChar() == '\\') {
			moveToNextChar();//escape

			text += moveToNextChar();//escape sequence like \"
		}
		else if (getCurrentChar() == '\"') {
			moveToNextChar();//skip closing quotes
			break;
		}
		else {
			text += moveToNextChar();
		}
	}
	return Token(TokenType::String, text, start);
}

//extract symbols ,operators or seperators like :,=,+,-,>=
Token Lexer::extractOperatororSymbol() {
	unsigned int start = currentIndex;
	char c = moveToNextChar();
	char next = getCurrentChar();
	//Two character operator check like >=,<=,<>
	if ((c == '<' && next == '>') || (c == '<' && next == '=') || (c == '>' && next == '=')) {
		std::string op;
		op += c;
		op += moveToNextChar();//consume second char
		return Token(TokenType::Operator, op, start);
	}
	std::string single(1, c);
	if (single == "," || single == ":" || single == ";") {
		return Token(TokenType::Seperator, single, start);
	}
	else if (std::string("+-=*/^<>()").find(c) != std::string::npos) {
		return Token(TokenType::Operator, single, start);
	}
	else {
		return Token(TokenType::Symbol, single, start);
	}
}
//special case:Extracts entire comment after REM
Token Lexer::extractCommentAfterRem() {
	unsigned int start = currentIndex - 3;//REM already read
	std::string comment = "REM";
	//grab everything until end of line
	while (getCurrentChar() != '\0') {
		comment += moveToNextChar();
	}
	return Token(TokenType::Comment, comment, start);
}
//checks whether a word is in the keyword list

bool Lexer::isKeyword(const std::string& word) {
	return std::find(Keywords.begin(), Keywords.end(), word) != Keywords.end();
}
//converts a string to uppercase (for case-insensitive keyword check) 
std::string Lexer::toUpper(const std::string& s) {
	std::string result = s;
	std::transform(result.begin(), result.end(), result.begin(), ::toupper);
	return result;
}


