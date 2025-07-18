#pragma once
#include "ASTNode.h"
#include "Token.h"
#include <vector>

class Parser {
public:
    explicit Parser(const std::vector<Token>& tks)
        : tokens(tks), pos(0) {}

    AST parseProgram();

private:
    const std::vector<Token>& tokens;
    size_t pos;

    const Token& peek() const;
    Token get();
    bool match(TokenType);

    AST parseStatement();
    AST parseExpression();
    AST parseTerm();
    AST parseFactor();
};
