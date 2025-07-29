#pragma once
#include "ASTNode.h"
#include "Token.h"
#include <vector>
#include <string>

class Parser {
public:
    Parser() {}
    ASTNode* parse(const std::vector<Token>& toks);

private:
    const std::vector<Token>* tokens = nullptr;
    int pos = 0;

    const Token& peek() const;
    Token get();
    bool match(TokenType type, const std::string& value = "");

    ASTNode* parseProgram();
    ASTNode* parseStatement();
    ASTNode* parseExpression();
    ASTNode* parseTerm();
    ASTNode* parseFactor();

    // Statements
    ASTNode* parsePrint();
    ASTNode* parseLet();
    ASTNode* parseIf();
    ASTNode* parseFor();
    ASTNode* parseWhile();
    ASTNode* parseDo();
    ASTNode* parseGoto();
    ASTNode* parseGosub();
    ASTNode* parseReturn();
    ASTNode* parseStop();
    ASTNode* parseInput();
    ASTNode* parseData();
    ASTNode* parseRead();
    ASTNode* parseRem();
    ASTNode* parseOnError();
    ASTNode* parseField();
    ASTNode* parseCommand();
    ASTNode* parseMathFunc();
    ASTNode* parseDefType();
};