#include "parser.h"
#include <stdexcept>
#include"Lexer.h"
#include"Token.h"
#include"ASTNode.h"

const Token& Parser::peek() const {
    static Token eof{ TokenType::END_OF_LINE, "", 0 };
    return pos < tokens.size() ? tokens[pos] : eof;
}

Token Parser::get() {
    return (pos < tokens.size()) ? tokens[pos++] : peek();
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
            auto expr = parseExpression();
            return std::make_shared<PrintNode>(expr);
        }
        if (kw == "LET") {
            if (peek().type != TokenType::Identifier)
                throw std::runtime_error("LET requires a variable name");
            std::string var = get().value;
            if (!match(TokenType::Symbol) || tokens[pos - 1].value != "=")
                throw std::runtime_error("LET missing '='");
            auto expr = parseExpression();
            return std::make_shared<LetNode>(var, expr);
        }
    }

    // Fallback expression-as-statement
    return parseExpression();
}

AST Parser::parseExpression() {
    auto node = parseTerm();
    while (peek().type == TokenType::Symbol &&
        (peek().value == "+" || peek().value == "-")) {
        std::string op = get().value;
        auto rhs = parseTerm();
        node = std::make_shared<BinOpNode>(op, node, rhs);
    }
    return node;
}

AST Parser::parseTerm() {
    auto node = parseFactor();
    while (peek().type == TokenType::Symbol &&
        (peek().value == "*" || peek().value == "/")) {
        std::string op = get().value;
        auto rhs = parseFactor();
        node = std::make_shared<BinOpNode>(op, node, rhs);
    }
    return node;
}

AST Parser::parseFactor() {
    if (match(TokenType::Number)) {
        return std::make_shared<NumberNode>(tokens[pos - 1].value);
    }
    if (match(TokenType::Identifier)) {
        return std::make_shared<IdentNode>(tokens[pos - 1].value);
    }
    if (match(TokenType::Symbol) && tokens[pos - 1].value == "(") {
        auto expr = parseExpression();
        if (!match(TokenType::Symbol) || tokens[pos - 1].value != ")")
            throw std::runtime_error("Missing ')'");
        return expr;
    }
    throw std::runtime_error("Unexpected token '" + peek().value + "'");
}
