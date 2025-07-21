#ifndef TOKEN_H
#define TOKEN_H
#include<string>
enum class TokenType { //different types of token in GWBASICS	
	Number,
	String,
	Identifier,
	Keyword,
	Symbol,
	Comment,
	END_OF_LINE,
	Operator,//to classify math or logical operator
	Seperator,//for seperating comma,colon etc.
	Invalid
};
struct Token {
	TokenType type; //what kind of token it is
	std::string value;//the exact word or number from code
	size_t position;//Index where the token starts in the line
	//std::string toString() const;
	Token(TokenType t, const std::string& v, size_t p) {
		type = t;
		value = v;
		position = p;

	}
};

#endif 
