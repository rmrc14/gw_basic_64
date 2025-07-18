#pragma once

#include "ASTNode.h"
#include "Token.h"
#include <vector>

class Parser {
public:
    Parser() = default;          // default constructor
    AST parse(const std::vector<Token>& tokens);

private:
    const std::vector<Token>* tokensPtr = nullptr;
    size_t pos = 0;

    const Token& peek() const;
    Token get();
    bool match(TokenType t);

    AST parseProgram();
    AST parseStatement();
    AST parseExpression();
    AST parseTerm();
    AST parseFactor();
};
