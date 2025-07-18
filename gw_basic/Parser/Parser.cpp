#include "parser.h"
#include <stdexcept>

AST Parser::parse(const std::vector<Token>& tokens) {
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

AST Parser::parseProgram() {
    auto program = std::make_shared<ProgramNode>();
    while (peek().type != TokenType::END_OF_LINE) {
        program->stmts.push_back(parseStatement());
    }
    return program;
}

AST Parser::parseStatement() {
    if (peek().type == TokenType::Keyword) {
        std::string kw = get().value;
        if (kw == "PRINT") {
            AST expr = parseExpression();
            return std::make_shared<PrintNode>(expr);
        }
        if (kw == "LET") {
            if (peek().type != TokenType::Identifier)
                throw std::runtime_error("LET needs a variable name");
            std::string var = get().value;
            if (!match(TokenType::Symbol) || (*tokensPtr)[pos - 1].value != "=")
                throw std::runtime_error("LET missing '='");
            AST expr = parseExpression();
            return std::make_shared<LetNode>(var, expr);
        }
    }
    return parseExpression();
}

AST Parser::parseExpression() {
    AST node = parseTerm();
    while (peek().type == TokenType::Symbol &&
        (peek().value == "+" || peek().value == "-")) {
        std::string op = get().value;
        AST rhs = parseTerm();
        node = std::make_shared<BinOpNode>(op, node, rhs);
    }
    return node;
}
AST Parser::parseTerm() {
    AST node = parseFactor();
    while (peek().type == TokenType::Symbol &&
        (peek().value == "*" || peek().value == "/")) {
        std::string op = get().value;
        AST rhs = parseFactor();
        node = std::make_shared<BinOpNode>(op, node, rhs);
    }
    return node;
}
AST Parser::parseFactor() {
    if (match(TokenType::Number)) {
        return std::make_shared<NumberNode>((*tokensPtr)[pos - 1].value);
    }
    if (match(TokenType::Identifier)) {
        return std::make_shared<IdentNode>((*tokensPtr)[pos - 1].value);
    }
    if (match(TokenType::Symbol) && (*tokensPtr)[pos - 1].value == "(") {
        AST expr = parseExpression();
        if (!match(TokenType::Symbol) || (*tokensPtr)[pos - 1].value != ")")
            throw std::runtime_error("Missing closing ')'");
        return expr;
    }
    throw std::runtime_error("Unexpected token '" + peek().value + "'");
}
