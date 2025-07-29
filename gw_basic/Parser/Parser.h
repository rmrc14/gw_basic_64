#pragma once
#include "ASTNode.h"
#include "Token.h"
#include <vector>
#include<string>

class Parser {
public:
    Parser() {}
    ASTNode* parse(const std::vector<Token>& toks);

private:
    const std::vector<Token>* tokens = nullptr;
    int pos = 0;

    const Token& peek() const;
    Token get();
    bool match(TokenType, const std::string& value = "");
    //bool check(TokenType type, const std::string& value = "") const;

    ASTNode* parseProgram();
    ASTNode* parseStatement();
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();
    //std::string parseComparison();
    ASTNode* parseFor();
    ASTNode* parseIf();
    ASTNode* parseGoto();
    ASTNode* parseGosub();
    ASTNode* parseReturn();
    ASTNode* parseData();
    ASTNode* parseRead();
    ASTNode* parseStop();
    ASTNode* parseRem();
    ASTNode* parseInput();

};