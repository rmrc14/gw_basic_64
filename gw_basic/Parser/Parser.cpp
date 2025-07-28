#include "parser.h"
#include <stdexcept>
#include<cstdlib>
#include<iostream>

ASTNode* Parser::parse(const std::vector<Token>& toks) {
    tokens = &toks;
    pos = 0;
    return parseProgram();
}

const Token& Parser::peek() const {
    static Token eof = Token(TokenType::END_OF_LINE, "", 0);
    if (pos < (int)tokens->size()) {
        return (*tokens)[pos];
    }
    return eof;
}

Token Parser::get() {
    if (pos < tokens->size()) {
        Token t = (*tokens)[pos];
        pos = pos + 1;
        return t;
    }
    else {
        return peek();
    }
}

bool Parser::match(TokenType t, const std::string& value) {
    if (peek().type != t) {
        return false;
    }
    if (value.empty() == false && peek().value != value) {
        return false;
    }
    get();
    return true;
}

//ASTNode* Parser::parseProgram() {
//    ProgramNode* program = new ProgramNode();
//    while (peek().type != TokenType::END_OF_LINE) {
//       /*ASTNode* stmt=parseStatement();
//        program->stmts.push_back(stmt);*/
//        ASTNode* stmt = parseStatement();
//        if (stmt != nullptr) {
//            program->stmts.push_back(stmt);
//        }
//    }
//    return program;
//}

//ASTNode* Parser::parseProgram() {
//    ProgramNode* program = new ProgramNode();
//    while (peek().type != TokenType::END_OF_LINE) {
//        ASTNode* stmt = parseStatement();
//        if (!stmt) break;  // `END` or label-only line
//        program->stmts.push_back(stmt);
//    }
//    return program;
//}

ASTNode* Parser::parseProgram() {
    ProgramNode* program = new ProgramNode();
    while (peek().type != TokenType::END_OF_LINE) {
        ASTNode* stmt = parseStatement();
        if (!stmt) break;
        program->stmts.push_back(stmt);

        // handle multiple statements on same line:
        while (match(TokenType::Separator, ":")) {
            ASTNode* nextStmt = parseStatement();
            if (!nextStmt) break;
            program->stmts.push_back(nextStmt);
        }
    }
    return program;
}




ASTNode* Parser::parseStatement() {
    // Drop line labels like "100 GOTO 130"
    //if (peek().type == TokenType::Number) {
    //    if (pos + 1 < (int)tokens->size() &&
    //        (*tokens)[pos + 1].type == TokenType::Keyword) {
    //        get(); // consume the label number
    //    }
    //}
    //------------------------------------------
    if (peek().type == TokenType::Number) {
        // If next token doesn't start a stmt, skip the label
        if (pos + 1 < (int)tokens->size() &&
            ((*tokens)[pos + 1].type == TokenType::END_OF_LINE ||
                (*tokens)[pos + 1].value == "")) {
            get();
            return nullptr;
        }
        get(); // remove label before normal parsing
    }
    //---------------------------------------------------
    // FOR
    if (match(TokenType::Keyword, "FOR")) {
        return parseFor();
    }

    // IF ... THEN ... [ELSE ...]
    if (match(TokenType::Keyword, "IF")) {
        return parseIf();
    }
    //------------------------------------------------------
    // GOTO
    if (peek().type == TokenType::Keyword && peek().value == "GOTO") {
        get(); // consume the "GOTO" keyword
        std::cerr << "[DEBUG] GOTO detected; calling parseGoto(), pos=" << pos << "\n";
        return parseGoto();
    }
    //--------------------------------------------------------
    //GOSUB
    if (match(TokenType::Keyword, "GOSUB")) {
        return parseGosub();
    }
    if (match(TokenType::Keyword, "RETURN")) {
        return parseReturn();
    }
    if (match(TokenType::Keyword, "DATA")) {
        return parseData();
    }
    if (match(TokenType::Keyword, "READ")) {
        return parseRead();
    }
    if (match(TokenType::Keyword, "STOP")) {
        return parseStop();
    }

    // INPUT
    if (match(TokenType::Keyword, "INPUT")) {
        return parseInput();
    }

    // PRINT
    if (match(TokenType::Keyword, "PRINT")) {
        ASTNode* expr = parseExpression();
        return new PrintNode(expr);
    }

    // LET
    if (match(TokenType::Keyword, "LET")) {
        if (!match(TokenType::Identifier)) {
            throw std::runtime_error("LET requires variable name");
        }
        std::string name = (*tokens)[pos - 1].value;
        if (!match(TokenType::Operator, "=")) {
            throw std::runtime_error("LET needs '='");
        }
        ASTNode* expr = parseExpression();
        return new LetNode(name, expr);
    }

    // END
    if (match(TokenType::Keyword, "END")) {
        return nullptr;
    }

    // Unexpected ELSE
    /*if (peek().type == TokenType::Keyword && peek().value == "ELSE") {
        throw std::runtime_error("Unexpected token 'ELSE'");
    }*/

    // Default: treat as expression or syntax error
    throw std::runtime_error("Unexpected token '" + peek().value + "'");
    //return parseExpression();
}
//std::string Parser::parseComparison() {
//    if (peek().type == TokenType::Operator || (peek().type == TokenType::Operator &&
//        (peek().value == "=" || peek().value == "<" || peek().value == ">" || peek().value == "<>" || peek().value == ">=" || peek().value == "<="))) {
//        std::string op = peek().value;
//        get();
//        return op;
//        
//    }
//    throw std::runtime_error("=,<>,<,>,<=,>=)");
//}

//ASTNode* Parser::parseExpression() {
//    ASTNode* lhs = parseTerm();
//    while (true) {
//        if (peek().type == TokenType::Symbol &&
//            (peek().value == "+" || peek().value == "-")) {
//                    std::string op = get().value;
//                    ASTNode* rhs = parseTerm();
//                    lhs = new BinOpNode(op, lhs, rhs);
//                }else {
//                     break;
//        }
//        }
//        return lhs;
//    }

ASTNode* Parser::parseExpression() {
    ASTNode* node = parseTerm();
    while (match(TokenType::Operator, "+") || match(TokenType::Operator, "-")) {
        std::string op = (*tokens)[pos - 1].value;
        ASTNode* rhs = parseTerm();
        node = new BinOpNode(op, node, rhs);
    }
    return node;
}

//ASTNode* Parser::parseTerm() {
//    ASTNode* lhs = parseFactor();
//    while (true) {
//        if (peek().type == TokenType::Symbol &&
//            (peek().value == "*" || peek().value == "/")) {
//            std::string op = get().value;
//            ASTNode* rhs = parseFactor();
//            lhs = new BinOpNode(op, lhs, rhs);
//        }
//        else {
//            break;
//        }
//    }
//    return lhs;
//}

ASTNode* Parser::parseTerm() {
    ASTNode* node = parseFactor();
    while (match(TokenType::Operator, "*") || match(TokenType::Operator, "/")) {
        std::string op = (*tokens)[pos - 1].value;
        ASTNode* rhs = parseFactor();
        node = new BinOpNode(op, node, rhs);
    }
    return node;
}
//ASTNode* Parser::parseFactor() {
//    if (match(TokenType::Number)) {
//        std::string v = (*tokensPtr)[pos - 1].value;
//        return new NumberNode(v);
//    }
//
//    if (match(TokenType::String)) {
//        std::string text = (*tokensPtr)[pos - 1].value;
//        return new StringNode(text);  
//    }
//
//    if (match(TokenType::Identifier)) {
//        std::string n = (*tokensPtr)[pos - 1].value;
//        return new IdentNode(n);
//    }
//
//    if (match(TokenType::Symbol) && (*tokensPtr)[pos - 1].value == "(") {
//        ASTNode* expr = parseExpression();
//        if (!match(TokenType::Symbol) || (*tokensPtr)[pos - 1].value != ")") {
//            throw std::runtime_error("Missing ')'");
//        }
//        return expr;
//    }
//
//    throw std::runtime_error(std::string("Unexpected token '") + peek().value + "'");
//}

ASTNode* Parser::parseFactor() {
    if (match(TokenType::Number)) {
        return new NumberNode((*tokens)[pos - 1].value);
    }
    if (match(TokenType::String)) {
        return new StringNode((*tokens)[pos - 1].value);
    }
    if (match(TokenType::Identifier)) {
        return new IdentNode((*tokens)[pos - 1].value);
    }

    if (match(TokenType::Operator, "(")) {
        ASTNode* expr = parseExpression();

        /*std::cerr << "[DEBUG] Expecting ')' but peek is: type="
            << static_cast<int>(peek().type)
            << ", value='" << peek().value << "'\n";*/

        if (!match(TokenType::Operator, ")")) {
            throw std::runtime_error("Missing ')'");
        }
        return expr;
    }

    throw std::runtime_error("Unexpected token '" + peek().value + "'");
}
ASTNode* Parser::parseFor() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("FOR missing variable name");
    }
    std::string var = (*tokens)[pos - 1].value;

    if (!match(TokenType::Operator, "=")) {
        throw std::runtime_error("FOR missing '=' after variable");
    }
    ASTNode* start = parseExpression();

    if (!match(TokenType::Keyword, "TO")) {
        throw std::runtime_error("FOR missing TO");
    }
    ASTNode* end = parseExpression();

    ASTNode* step;
    if (match(TokenType::Keyword, "STEP")) {
        step = parseExpression();
    }
    else {
        step = new NumberNode("1");
    }

    if (!match(TokenType::Separator, ":")) {
        throw std::runtime_error("FOR missing ':' before body");
    }
    ASTNode* body = parseStatement();

    if (match(TokenType::Keyword, "NEXT")) {
        if (!match(TokenType::Identifier) || (*tokens)[pos - 1].value != var) {
            throw std::runtime_error("NEXT variable does not match FOR variable");
        }
    }
    return new ForNode(var, start, end, step, body);
}
ASTNode* Parser::parseIf() {
    ASTNode* left = parseExpression();

    if (match(TokenType::Operator) == false) {
        throw std::runtime_error("IF needs comparison operator");
    }
    std::string op = (*tokens)[pos - 1].value;

    ASTNode* right = parseExpression();

    if (match(TokenType::Keyword, "THEN") == false) {
        throw std::runtime_error("IF missing THEN");
    }

    if (peek().type == TokenType::END_OF_LINE) {
        throw std::runtime_error("IF missing statement after THEN");
    }
    ASTNode* thenStmt = parseStatement();
    //ASTNode* thenStmt = nullptr;
    //if (peek().type != TokenType::Keyword     // not another keyword
    //    && peek().type != TokenType::END_OF_LINE // not end of line
    //    ) {
    //    thenStmt = parseStatement();
    //}
   /* if (match(TokenType::Keyword, "ELSE")) {
        if (peek().type == TokenType::Keyword && peek().value == "IF") {
            get();
            elseStmt = parseIf();
        }
        else {
            elseStmt = parseStatement();
        }
    }*/
    ASTNode* elseStmt = nullptr;
    if (match(TokenType::Keyword, "ELSE")) {
        // ELSE followed by a valid statement
        if (peek().type == TokenType::END_OF_LINE) {
            throw std::runtime_error("IF missing statement after ELSE");
        }
        elseStmt = parseStatement();
    }

    return new IfElseNode(left, op, right, thenStmt, elseStmt);
}
//ASTNode* Parser::parseIf() {
//    get();  
//    ASTNode* left = parseExpression();
//
//    const Token& cmp = peek();
//    if (cmp.type != TokenType::Symbol ||
//        !(cmp.value == ">" || cmp.value == "<" || cmp.value == "=" ||
//            cmp.value == "<=" || cmp.value == ">=" || cmp.value == "<>")) {
//        throw std::runtime_error("IF missing comparison operator");
//    }
//    std::string op = get().value;
//
//    ASTNode* right = parseExpression();
//
// 
//    if (!match(TokenType::Keyword) || (*tokensPtr)[pos - 1].value != "THEN") {
//        throw std::runtime_error("IF missing THEN");
//    }
//    ASTNode* thenStmt = parseStatement();
//
//    ASTNode* elseStmt = nullptr;
//    if (peek().type == TokenType::Keyword && peek().value == "ELSE") {
//        get();
//        if (peek().type == TokenType::Keyword && peek().value == "IF") {
//            elseStmt = parseIf();
//        }
//        else {
//            elseStmt = parseStatement();
//        }
//    }
//    return new IfElseNode(left, op, right, thenStmt, elseStmt);
//}
//ASTNode* Parser::parseGoto() {
//   // std::cerr << "[DEBUG] in parseGoto(), pos=" << pos << "\n";
//    if (!match(TokenType::Number)) {
//        throw std::runtime_error("GOTO requires a line number");
//    }
//    int line = std::atoi((*tokens)[pos - 1].value.c_str());
//   // std::cerr << "[DEBUG] goto line = " << line << ", new pos=" << pos << "\n";
//    while (match(TokenType::Separator, ":"));
//    return new GotoNode(line);
//}
ASTNode* Parser::parseGoto() {
    if (!match(TokenType::Number)) {
        throw std::runtime_error("GOTO needs line");
    }
    int line = std::atoi((*tokens)[pos - 1].value.c_str());
    return new GotoNode(line);
}

ASTNode* Parser::parseData() {
    std::vector<std::string> vals;
    do {
        if (match(TokenType::Number) || match(TokenType::String)) {
            vals.push_back((*tokens)[pos - 1].value);
        }
    } while (match(TokenType::Separator, ","));
    return new DataNode(vals);
}

ASTNode* Parser::parseRead() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("READ needs variable");
    }
    return new ReadNode((*tokens)[pos - 1].value);
}

ASTNode* Parser::parseGosub() {
    if (!match(TokenType::Number)) {
        throw std::runtime_error("GOSUB needs line");
    }
    int ln = std::stoi((*tokens)[pos - 1].value);
    return new GosubNode(ln);
}

ASTNode* Parser::parseReturn() {
    return new ReturnNode();
}

ASTNode* Parser::parseStop() {
    return new StopNode();
}

ASTNode* Parser::parseRem() {
    get(); // consume REM
    std::string rest = peek().value; // everything else
    pos = tokens->size(); // skip to EOL
    return new RemNode(rest);
}


ASTNode* Parser::parseInput() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("INPUT requires a variable name");
    }
    std::string name = (*tokens)[pos - 1].value;
    return new InputNode(name);
}



