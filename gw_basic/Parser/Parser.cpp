#include "parser.h"
#include <stdexcept>

ASTNode* Parser::parse(const std::vector<Token>& tokens) {
    tokensPtr = &tokens;
    pos = 0;
    return parseProgram();
}

const Token& Parser::peek() const {
    static Token eof{ TokenType::END_OF_LINE, "", 0 };
    if (pos < tokensPtr->size())
        return (*tokensPtr)[pos];
    return eof;
}

Token Parser::get() {
    if (pos < tokensPtr->size())
        return (*tokensPtr)[pos++];
    return peek();
}

bool Parser::match(TokenType t) {
    if (peek().type == t) {
        get();
        return true;
    }
    return false;
}

ASTNode* Parser::parseProgram() {
    auto* program = new ProgramNode();
    while (peek().type != TokenType::END_OF_LINE) {
        program->stmts.push_back(parseStatement());
    }
    return program;
}

ASTNode* Parser::parseStatement() {
    if (peek().type == TokenType::Keyword) {
        std::string kw = get().value;
        if (kw == "PRINT") {
            ASTNode* expr = parseExpression();
            return new PrintNode(expr);
        }
        if (kw == "LET") {
            if (peek().type != TokenType::Identifier)
                throw std::runtime_error("LET needs a variable name");
            std::string var = get().value;
            if (!match(TokenType::Symbol) || (*tokensPtr)[pos - 1].value != "=")
                throw std::runtime_error("LET missing '='");
            ASTNode* expr = parseExpression();
            return new LetNode(var, expr);
        }
    }
    return parseExpression();
}

ASTNode* Parser::parseExpression() {
    ASTNode* node = parseTerm();
    while (peek().type == TokenType::Symbol &&
        (peek().value == "+" || peek().value == "-")) {
        std::string op = get().value;
        ASTNode* rhs = parseTerm();
        node = new BinOpNode(op, node, rhs);
    }
    return node;
}

ASTNode* Parser::parseTerm() {
    ASTNode* node = parseFactor();
    while (peek().type == TokenType::Symbol &&
        (peek().value == "*" || peek().value == "/")) {
        std::string op = get().value;
        ASTNode* rhs = parseFactor();
        node = new BinOpNode(op, node, rhs);
    }
    return node;
}

ASTNode* Parser::parseFactor() {
    if (match(TokenType::Number)) {
        return new NumberNode((*tokensPtr)[pos - 1].value);
    }
    if (match(TokenType::Identifier)) {
        return new IdentNode((*tokensPtr)[pos - 1].value);
    }
    if (match(TokenType::Symbol) && (*tokensPtr)[pos - 1].value == "(") {
        ASTNode* expr = parseExpression();
        if (!match(TokenType::Symbol) || (*tokensPtr)[pos - 1].value != ")")
            throw std::runtime_error("Missing closing ')'");
        return expr;
    }
    throw std::runtime_error("Unexpected token '" + peek().value + "'");
}
