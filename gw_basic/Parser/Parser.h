#pragma once
#include "ASTNode.h"
#include "Token.h"
#include <vector>

class Parser {
public:
    Parser() {}
    ASTNode* parse(const std::vector<Token>& tokens);

private:
    const std::vector<Token>* tokensPtr = nullptr;
    size_t pos = 0;

    const Token& peek() const;
    Token get();
    bool match(TokenType);

    ASTNode* parseProgram();
    ASTNode* parseStatement();
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
};
