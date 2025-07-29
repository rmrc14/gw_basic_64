#include "parser.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>

// Custom node for PRINT A, B, C
class PrintListNode : public ASTNode {
public:
    std::vector<ASTNode*> exprs;
    PrintListNode(const std::vector<ASTNode*>& e) : exprs(e) {}
    ~PrintListNode() {
        for (auto e : exprs) delete e;
    }
    ASTType type() const override {
        return ASTType::PrintStmt;
    }
};

ASTNode* Parser::parse(const std::vector<Token>& toks) {
    tokens = &toks;
    pos = 0;
    return parseProgram();
}

const Token& Parser::peek() const {
    static Token eof = Token(TokenType::END_OF_LINE, "", 0);
    if (pos < static_cast<int>(tokens->size()))
        return (*tokens)[pos];
    return eof;
}

Token Parser::get() {
    if (pos < static_cast<int>(tokens->size())) {
        return (*tokens)[pos++];
    }
    return peek();
}

bool Parser::match(TokenType type, const std::string& value) {
    if (peek().type != type) return false;
    if (!value.empty() && peek().value != value) return false;
    get();
    return true;
}

ASTNode* Parser::parseProgram() {
    auto* prog = new ProgramNode();

    while (peek().type != TokenType::END_OF_LINE) {
        // Skip line numbers
        if (peek().type == TokenType::Number) {
            get();
            continue;
        }

        ASTNode* stmt = parseStatement();
        if (!stmt) break;
        prog->stmts.push_back(stmt);

        // Handle multiple statements separated by :
        while (match(TokenType::Separator, ":")) {
            stmt = parseStatement();
            if (!stmt) break;
            prog->stmts.push_back(stmt);
        }
    }

    return prog;
}

ASTNode* Parser::parseStatement() {
    if (match(TokenType::Keyword, "PRINT")) return parsePrint();
    if (match(TokenType::Keyword, "LET")) return parseLet();
    if (match(TokenType::Keyword, "INPUT")) return parseInput();
    if (match(TokenType::Keyword, "IF")) return parseIf();
    if (match(TokenType::Keyword, "FOR")) return parseFor();
    if (match(TokenType::Keyword, "NEXT")) {
        if (!match(TokenType::Identifier)) throw std::runtime_error("NEXT needs variable");
        return new NextNode((*tokens)[pos - 1].value);
    }
    if (match(TokenType::Keyword, "SAVE")) {
        std::string filename = get().value;
        return new CommandNode("SAVE", { filename });
    }
    if (match(TokenType::Keyword, "WHILE")) return parseWhile();
    if (match(TokenType::Keyword, "DO")) return parseDo();
    if (match(TokenType::Keyword, "GOTO")) return parseGoto();
    if (match(TokenType::Keyword, "GOSUB")) return parseGosub();
    if (match(TokenType::Keyword, "RETURN")) return parseReturn();
    if (match(TokenType::Keyword, "STOP")) return parseStop();
    if (match(TokenType::Keyword, "END")) return new StopNode(); // END same as STOP
    if (match(TokenType::Keyword, "DATA")) return parseData();
    if (match(TokenType::Keyword, "READ")) return parseRead();
    if (match(TokenType::Keyword, "REM")) return parseRem();
    if (match(TokenType::Keyword, "ON")) return parseOnError();
    if (match(TokenType::Keyword, "FIELD")) return parseField();
    if (peek().type == TokenType::Keyword) return parseCommand();
    if (match(TokenType::Keyword, "SYSTEM")) return new CommandNode("SYSTEM", {});
    if (match(TokenType::Keyword, "CONT")) return new CommandNode("CONT", {});
    if (match(TokenType::Keyword, "EDIT")) return new CommandNode("EDIT", {});
    if (match(TokenType::Keyword, "CLEAR")) return new CommandNode("CLEAR", {});
    if (match(TokenType::Keyword, "DIR")) return new CommandNode("DIR", {});
    if (match(TokenType::Keyword, "SCREEN")) {
        std::string mode = get().value;
        return new CommandNode("SCREEN", { mode });
    }


    // Implicit assignment (X = 5)
    if (peek().type == TokenType::Identifier) {
        std::string name = get().value;

        if (match(TokenType::Operator, "=")) {
            ASTNode* expr = parseExpression();
            return new LetNode(name, expr);
        }
        else {
            // not an assignment — put back identifier?
            throw std::runtime_error("Expected '=' after identifier: " + name);
        }
    }

    if (match(TokenType::Keyword, "END")) return nullptr;

    throw std::runtime_error("Unexpected token: " + peek().value);
}

ASTNode* Parser::parsePrint() {
    std::vector<ASTNode*> exprs;
    exprs.push_back(parseExpression());

    while (match(TokenType::Separator, ";") || match(TokenType::Separator, ",")) {
        exprs.push_back(parseExpression());
    }

    if (exprs.size() == 1) {
        return new PrintNode(exprs[0]);
    }
    else {
        return new PrintListNode(exprs);
    }
}

ASTNode* Parser::parseLet() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("LET needs a variable name");
    }

    std::string name = (*tokens)[pos - 1].value;

    if (!match(TokenType::Operator, "=")) {
        throw std::runtime_error("LET requires '=' after variable");
    }

    ASTNode* expr = parseExpression();
    return new LetNode(name, expr);
}
//
//ASTNode* Parser::parseInput() {
//    if (!match(TokenType::Identifier)) {
//        throw std::runtime_error("INPUT requires a variable");
//    }
//    return new InputNode((*tokens)[pos - 1].value);
//}
ASTNode* Parser::parseInput() {
    std::string var = get().value;
    return new InputNode(var);
}

ASTNode* Parser::parseIf() {
    ASTNode* condition = parseExpression();

    if (!match(TokenType::Keyword, "THEN"))
        throw std::runtime_error("IF missing THEN");

    ASTNode* thenStmt = parseStatement();
    ASTNode* elseStmt = nullptr;

    if (match(TokenType::Keyword, "ELSE")) {
        elseStmt = parseStatement();
    }

    // Convert condition to BinOpExpr if possible, or wrap in IfElseNode directly
    if (auto* bin = dynamic_cast<BinOpNode*>(condition)) {
        return new IfElseNode(bin->left, bin->op, bin->right, thenStmt, elseStmt);
    }
    else {
        throw std::runtime_error("IF condition must be binary comparison");
    }
}


ASTNode* Parser::parseFor() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("FOR needs variable");
    }
    std::string var = (*tokens)[pos - 1].value;

    if (!match(TokenType::Operator, "=")) {
        throw std::runtime_error("FOR missing '='");
    }

    ASTNode* start = parseExpression();

    if (!match(TokenType::Keyword, "TO")) {
        throw std::runtime_error("FOR missing TO");
    }

    ASTNode* end = parseExpression();

    ASTNode* step = nullptr;
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
            throw std::runtime_error("NEXT variable mismatch");
        }
    }

    return new ForNode(var, start, end, step, body);
}

ASTNode* Parser::parseWhile() {
    ASTNode* condition = parseExpression();

    ProgramNode* body = new ProgramNode();

    while (!(peek().type == TokenType::Keyword && peek().value == "WEND")) {
        if (match(TokenType::Separator, ":")) continue;
        ASTNode* stmt = parseStatement();
        if (stmt) body->stmts.push_back(stmt);
    }

    if (!match(TokenType::Keyword, "WEND"))
        throw std::runtime_error("WHILE missing WEND");

    return new WhileNode(condition, body);
}

ASTNode* Parser::parseDo() {
    ProgramNode* body = new ProgramNode();

    // Parse body until we hit 'LOOP'
    while (!(peek().type == TokenType::Keyword && peek().value == "LOOP")) {
        if (peek().type == TokenType::END_OF_LINE)
            throw std::runtime_error("DO missing LOOP");

        if (match(TokenType::Separator, ":")) continue;

        ASTNode* stmt = parseStatement();
        if (stmt) body->stmts.push_back(stmt);
    }

    get(); // consume LOOP

    ASTNode* cond = nullptr;
    bool until = false;

    // Now parse 'WHILE' or 'UNTIL' if present
    if (match(TokenType::Keyword, "WHILE")) {
        cond = parseExpression();
    }
    else if (match(TokenType::Keyword, "UNTIL")) {
        until = true;
        cond = parseExpression();
    }

    return new DoLoopNode(body, cond, until);
}


ASTNode* Parser::parseGoto() {
    if (!match(TokenType::Number)) {
        throw std::runtime_error("GOTO needs line number");
    }
    return new GotoNode(std::atoi((*tokens)[pos - 1].value.c_str()));
}

ASTNode* Parser::parseGosub() {
    int line = std::stoi(get().value);
    return new GosubNode(line);
}

ASTNode* Parser::parseReturn() {
    return new ReturnNode();
}

ASTNode* Parser::parseStop() {
    return new StopNode();
}

ASTNode* Parser::parseData() {
    std::vector<std::string> values;
    do {
        Token t = get();
        values.push_back(t.value);
    } while (match(TokenType::Separator, ","));
    return new DataNode(values);
}


ASTNode* Parser::parseRead() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("READ requires variable name");
    }
    return new ReadNode((*tokens)[pos - 1].value);
}

ASTNode* Parser::parseRem() {
    get(); // consume REM
    std::string comment;
    while (peek().type != TokenType::END_OF_LINE) {
        if (!comment.empty()) comment += " ";
        comment += get().value;
    }
    return new RemNode(comment);
}

ASTNode* Parser::parseOnError() {
    if (!match(TokenType::Keyword, "ERROR"))
        throw std::runtime_error("ON ERROR syntax error");
    if (!match(TokenType::Keyword, "GOTO"))
        throw std::runtime_error("ON ERROR missing GOTO");
    if (!match(TokenType::Number))
        throw std::runtime_error("ON ERROR requires line number");

    int line = std::atoi((*tokens)[pos - 1].value.c_str());
    return new OnErrorNode(line);
}

ASTNode* Parser::parseField() {
    if (!match(TokenType::Number))
        throw std::runtime_error("FIELD requires file number");

    int fileNum = std::atoi((*tokens)[pos - 1].value.c_str());
    std::vector<std::pair<int, std::string>> fields;

    while (match(TokenType::Separator, ",")) {
        if (!match(TokenType::Number))
            throw std::runtime_error("FIELD requires width");
        int width = std::atoi((*tokens)[pos - 1].value.c_str());

        if (!match(TokenType::Keyword, "AS"))
            throw std::runtime_error("FIELD missing 'AS'");

        if (!match(TokenType::Identifier))
            throw std::runtime_error("FIELD requires variable");

        fields.emplace_back(width, (*tokens)[pos - 1].value);
    }

    return new FieldNode(fileNum, fields);
}

ASTNode* Parser::parseCommand() {
    std::string cmd = get().value;
    std::vector<std::string> args;

    while (match(TokenType::Identifier) || match(TokenType::String) || match(TokenType::Number)) {
        args.push_back((*tokens)[pos - 1].value);
        if (!match(TokenType::Separator, ",")) break;
    }

    return new CommandNode(cmd, args);
}

ASTNode* Parser::parseExpression() {
    ASTNode* node = parseTerm();
    while (peek().type == TokenType::Operator &&
        (peek().value == "+" || peek().value == "-" ||
            peek().value == "<" || peek().value == ">" ||
            peek().value == "=" || peek().value == "<=" ||
            peek().value == ">=" || peek().value == "<>")) {
        std::string op = get().value;
        ASTNode* rhs = parseTerm();
        node = new BinOpNode(op, node, rhs);
    }
    return node;

    if (peek().type == TokenType::Identifier) {
        std::string id = get().value;

        // Check for math function call
        if (match(TokenType::Separator, "(")) {
            ASTNode* arg = parseExpression();
            match(TokenType::Separator, ")");
            return new MathFuncNode(id, arg);  // You must define MathFuncNode
        }

        return new IdentNode(id);
    }

}

ASTNode* Parser::parseTerm() {
    ASTNode* node = parseFactor();
    while (match(TokenType::Operator, "*") || match(TokenType::Operator, "/")) {
        std::string op = (*tokens)[pos - 1].value;
        ASTNode* rhs = parseFactor();
        node = new BinOpNode(op, node, rhs);
    }
    return node;
}

ASTNode* Parser::parseFactor() {
    if (match(TokenType::Number))
        return new NumberNode((*tokens)[pos - 1].value);
    if (match(TokenType::String))
        return new StringNode((*tokens)[pos - 1].value);
    if (match(TokenType::Identifier))
        return new IdentNode((*tokens)[pos - 1].value);

    if (match(TokenType::Operator, "(")) {
        ASTNode* expr = parseExpression();
        if (!match(TokenType::Operator, ")"))
            throw std::runtime_error("Missing ')'");
        return expr;
    }

    if (peek().type == TokenType::Keyword &&
        (peek().value == "SIN" || peek().value == "COS" ||
            peek().value == "SQR" || peek().value == "LOG" ||
            peek().value == "EXP") || peek().value == "COS" || peek().value == "ABS" || peek().value == "VAL"
        || peek().value == "LEN" || peek().value == "RND") {
        return parseMathFunc();
    }
    // "SQR", "SIN", "COS", "LOG", "ABS", "ATN", "RND", "VAL", "LEN"
    throw std::runtime_error("Unexpected token in expression: " + peek().value);
}

ASTNode* Parser::parseMathFunc() {
    std::string fn = get().value;

    if (!match(TokenType::Operator, "("))
        throw std::runtime_error(fn + " requires '('");

    ASTNode* arg = parseExpression();

    if (!match(TokenType::Operator, ")"))
        throw std::runtime_error(fn + " missing ')'");

    return new MathFuncNode(fn, arg);
}

ASTNode* Parser::parseDefType() {
    std::string dtype = get().value;
    if (!match(TokenType::Identifier))
        throw std::runtime_error(dtype + " requires variable range");

    std::string range = (*tokens)[pos - 1].value;
    return new DefTypeNode(dtype, range);
}