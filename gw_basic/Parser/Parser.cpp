#include "parser.h"
#include <stdexcept>
#include <cstdlib>
#include <iostream>

// Custom node for PRINT A, B, C
class PrintListNode : public ASTNode {
public:
    std::vector<ASTNode*> exprs;

    PrintListNode(const std::vector<ASTNode*>& expressions)
        : exprs(expressions) {}

    ~PrintListNode() {
        for (int i = 0; i < (int)exprs.size(); i++) {
            delete exprs[i];
        }
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
    static Token eofToken(TokenType::END_OF_LINE, "", 0);

    if (pos < (int)tokens->size()) {
        return (*tokens)[pos];
    }

    return eofToken;
}


Token Parser::get() {
    if (pos < (int)tokens->size()) {
        Token current = (*tokens)[pos];
        pos = pos + 1;
        return current;
    }

    return peek();
}


bool Parser::match(TokenType type, const std::string& value) {
    const Token& current = peek();

    if (current.type != type) {
        return false;
    }

    if (!value.empty()) {
        if (current.value != value) {
            return false;
        }
    }

    get(); // consume the matched token
    return true;
}


ASTNode* Parser::parseProgram() {
    ProgramNode* program = new ProgramNode();

    while (peek().type != TokenType::END_OF_LINE) {
        // Skip line numbers
        if (peek().type == TokenType::Number) {
            get();
            continue;
        }

        ASTNode* stmt = parseStatement();
        if (stmt == nullptr) {
            break;
        }

        program->stmts.push_back(stmt);

        // Allow multiple statements on the same line separated by ':'
        while (match(TokenType::Separator, ":")) {
            stmt = parseStatement();
            if (stmt == nullptr) {
                break;
            }
            program->stmts.push_back(stmt);
        }
    }

    return program;
}

ASTNode* Parser::parseStatement() {
    if (match(TokenType::Keyword, "PRINT")) return parsePrint();
    if (match(TokenType::Keyword, "LET")) return parseLet();
    if (match(TokenType::Keyword, "INPUT")) return parseInput();
    if (match(TokenType::Keyword, "IF")) return parseIf();
    if (match(TokenType::Keyword, "FOR")) return parseFor();
    if (match(TokenType::Keyword, "WHILE")) return parseWhile();
    if (match(TokenType::Keyword, "DO")) return parseDo();
    if (match(TokenType::Keyword, "GOTO")) return parseGoto();
    if (match(TokenType::Keyword, "GOSUB")) return parseGosub();
    if (match(TokenType::Keyword, "RETURN")) return parseReturn();
    if (match(TokenType::Keyword, "STOP")) return parseStop();
    if (match(TokenType::Keyword, "END")) return new StopNode();  // END same as STOP
    if (match(TokenType::Keyword, "DATA")) return parseData();
    if (match(TokenType::Keyword, "READ")) return parseRead();
    if (match(TokenType::Keyword, "REM")) return parseRem();
    if (match(TokenType::Keyword, "ON")) return parseOnError();
    if (match(TokenType::Keyword, "FIELD")) return parseField();

    if (match(TokenType::Keyword, "NEXT")) {
        if (!match(TokenType::Identifier)) {
            throw std::runtime_error("NEXT requires variable name");
        }
        std::string var = (*tokens)[pos - 1].value;
        return new NextNode(var);
    }

    // Command keywords without parameters
    if (match(TokenType::Keyword, "SYSTEM")) return new CommandNode("SYSTEM", {});
    if (match(TokenType::Keyword, "CONT")) return new CommandNode("CONT", {});
    if (match(TokenType::Keyword, "EDIT")) return new CommandNode("EDIT", {});
    if (match(TokenType::Keyword, "CLEAR")) return new CommandNode("CLEAR", {});
    if (match(TokenType::Keyword, "DIR")) return new CommandNode("DIR", {});
    //if (match(TokenType::Keyword, "RESTORE")) return new RestoreNode();

    // Commands with one string argument
    if (match(TokenType::Keyword, "SAVE")) {
        if (!match(TokenType::String)) {
            throw std::runtime_error("SAVE requires a filename string");
        }
        std::string filename = (*tokens)[pos - 1].value;
        return new CommandNode("SAVE", { filename });
    }

    if (match(TokenType::Keyword, "LOAD")) {
        if (!match(TokenType::String)) {
            throw std::runtime_error("LOAD requires a filename string");
        }
        std::string filename = (*tokens)[pos - 1].value;
        return new CommandNode("LOAD", { filename });
    }

    // SCREEN command with a mode
    if (match(TokenType::Keyword, "SCREEN")) {
        std::string mode = get().value;
        return new CommandNode("SCREEN", { mode });
    }

    // File operations
   /* if (match(TokenType::Keyword, "OPEN")) return parseOpen();
    if (match(TokenType::Keyword, "CLOSE")) return parseClose();
    if (match(TokenType::Keyword, "WRITE")) return parseWrite();
    if (match(TokenType::Keyword, "GET")) return parseGet();
    if (match(TokenType::Keyword, "PUT")) return parsePut();
    if (match(TokenType::Keyword, "BLOAD")) return parseLoad();
    if (match(TokenType::Keyword, "BSAVE")) return parseSave();*/

    // Fallback for other known keyword-based commands
    if (peek().type == TokenType::Keyword) {
        return parseCommand();  // Generic CommandNode
    }

    // Implicit LET (e.g. X = 5)
    if (peek().type == TokenType::Identifier) {
        std::string varName = get().value;

        if (match(TokenType::Operator, "=")) {
            ASTNode* expr = parseExpression();
            return new LetNode(varName, expr);
        }
        else {
            throw std::runtime_error("Expected '=' after identifier: " + varName);
        }
    }

    // If END appears alone (like `END` as last statement), treat as no-op
    if (match(TokenType::Keyword, "END")) {
        return nullptr;
    }

    throw std::runtime_error("Unexpected token: " + peek().value);
}

ASTNode* Parser::parsePrint() {
    std::vector<ASTNode*> exprs;

    // Always try to parse at least one expression
    ASTNode* firstExpr = parseExpression();
    exprs.push_back(firstExpr);

    // Keep parsing more expressions if separated by ; or ,
    while (true) {
        if (match(TokenType::Separator, ";") || match(TokenType::Separator, ",")) {
            ASTNode* expr = parseExpression();
            exprs.push_back(expr);
        }
        else {
            break;
        }
    }

    // If only one expression, return a simple PrintNode
    if (exprs.size() == 1) {
        return new PrintNode(exprs[0]);
    }

    // If multiple expressions, use PrintListNode
    return new PrintListNode(exprs);
}


ASTNode* Parser::parseLet() {
    // Must have an identifier
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("LET statement must be followed by a variable name.");
    }

    std::string varName = (*tokens)[pos - 1].value;

    // Expect "=" operator after variable
    if (!match(TokenType::Operator, "=")) {
        throw std::runtime_error("LET must have '=' after variable name.");
    }

    // Parse the expression assigned to the variable
    ASTNode* expr = parseExpression();

    return new LetNode(varName, expr);
}

//
//ASTNode* Parser::parseInput() {
//    if (!match(TokenType::Identifier)) {
//        throw std::runtime_error("INPUT requires a variable");
//    }
//    return new InputNode((*tokens)[pos - 1].value);
//}
ASTNode* Parser::parseInput() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("INPUT requires a variable name.");
    }

    std::string var = (*tokens)[pos - 1].value;

    return new InputNode(var);
}


ASTNode* Parser::parseIf() {
    // Parse the condition
    ASTNode* condition = parseExpression();

    // Expect THEN
    if (!match(TokenType::Keyword, "THEN")) {
        throw std::runtime_error("IF statement must include THEN.");
    }

    // Parse the THEN statement
    ASTNode* thenStmt = parseStatement();

    // Check for optional ELSE
    ASTNode* elseStmt = nullptr;
    if (match(TokenType::Keyword, "ELSE")) {
        elseStmt = parseStatement();
    }

    // Ensure condition is a binary comparison (like A > B)
    BinOpNode* bin = dynamic_cast<BinOpNode*>(condition);
    if (bin == nullptr) {
        throw std::runtime_error("IF condition must be a binary comparison (e.g., A > B).");
    }

    return new IfElseNode(bin->left, bin->op, bin->right, thenStmt, elseStmt);
}


ASTNode* Parser::parseFor() {
    // Expect a variable after FOR
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("FOR requires a loop variable.");
    }

    std::string varName = (*tokens)[pos - 1].value;

    // Expect "=" sign
    if (!match(TokenType::Operator, "=")) {
        throw std::runtime_error("FOR missing '=' after loop variable.");
    }

    // Parse the start expression
    ASTNode* startExpr = parseExpression();

    // Expect TO keyword
    if (!match(TokenType::Keyword, "TO")) {
        throw std::runtime_error("FOR loop must have a TO value.");
    }

    // Parse the end expression
    ASTNode* endExpr = parseExpression();

    // Parse optional STEP value
    ASTNode* stepExpr = nullptr;
    if (match(TokenType::Keyword, "STEP")) {
        stepExpr = parseExpression();
    }
    else {
        stepExpr = new NumberNode("1");
    }

    // Require colon before body
    if (!match(TokenType::Separator, ":")) {
        throw std::runtime_error("FOR must use ':' before loop body.");
    }

    // Parse loop body statement
    ASTNode* bodyStmt = parseStatement();

    // Optional: parse NEXT var
    if (match(TokenType::Keyword, "NEXT")) {
        if (!match(TokenType::Identifier)) {
            throw std::runtime_error("NEXT must be followed by loop variable.");
        }
        std::string nextVar = (*tokens)[pos - 1].value;
        if (nextVar != varName) {
            throw std::runtime_error("NEXT variable must match FOR variable.");
        }
    }

    return new ForNode(varName, startExpr, endExpr, stepExpr, bodyStmt);
}


ASTNode* Parser::parseWhile() {
    // Parse condition
    ASTNode* condition = parseExpression();

    // Prepare body node
    ProgramNode* body = new ProgramNode();

    // Parse body statements until we see WEND
    while (true) {
        const Token& tok = peek();
        if (tok.type == TokenType::Keyword && tok.value == "WEND") {
            break;
        }

        if (match(TokenType::Separator, ":")) {
            continue;
        }

        ASTNode* stmt = parseStatement();
        if (stmt != nullptr) {
            body->stmts.push_back(stmt);
        }
    }

    // Consume WEND
    if (!match(TokenType::Keyword, "WEND")) {
        throw std::runtime_error("WHILE loop must end with WEND.");
    }

    return new WhileNode(condition, body);
}

ASTNode* Parser::parseDo() {
    // Prepare loop body
    ProgramNode* body = new ProgramNode();

    // Collect statements until we see LOOP
    while (true) {
        const Token& tok = peek();

        if (tok.type == TokenType::Keyword && tok.value == "LOOP") {
            break;
        }

        if (tok.type == TokenType::END_OF_LINE) {
            throw std::runtime_error("DO loop missing LOOP.");
        }

        if (match(TokenType::Separator, ":")) {
            continue;
        }

        ASTNode* stmt = parseStatement();
        if (stmt != nullptr) {
            body->stmts.push_back(stmt);
        }
    }

    // Consume LOOP
    get();

    // Optional WHILE or UNTIL condition
    ASTNode* condition = nullptr;
    bool isUntil = false;

    if (match(TokenType::Keyword, "WHILE")) {
        condition = parseExpression();
    }
    else if (match(TokenType::Keyword, "UNTIL")) {
        isUntil = true;
        condition = parseExpression();
    }

    return new DoLoopNode(body, condition, isUntil);
}


ASTNode* Parser::parseGoto() {
    if (!match(TokenType::Number)) {
        throw std::runtime_error("GOTO needs a line number.");
    }
    int line = std::atoi((*tokens)[pos - 1].value.c_str());
    return new GotoNode(line);
}

ASTNode* Parser::parseGosub() {
    if (!match(TokenType::Number)) {
        throw std::runtime_error("GOSUB needs a line number.");
    }
    int line = std::atoi((*tokens)[pos - 1].value.c_str());
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
        Token t = get(); // Get next value
        values.push_back(t.value);
    } while (match(TokenType::Separator, ",")); // Support comma-separated list

    return new DataNode(values);
}

ASTNode* Parser::parseRead() {
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error("READ requires a variable name.");
    }
    std::string varName = (*tokens)[pos - 1].value;
    return new ReadNode(varName);
}


ASTNode* Parser::parseRem() {
    get(); // Consume REM
    std::string comment = "";

    while (peek().type != TokenType::END_OF_LINE) {
        if (!comment.empty()) {
            comment += " ";
        }
        comment += get().value;
    }

    return new RemNode(comment);
}


ASTNode* Parser::parseOnError() {
    if (!match(TokenType::Keyword, "ERROR")) {
        throw std::runtime_error("ON ERROR syntax error.");
    }

    if (!match(TokenType::Keyword, "GOTO")) {
        throw std::runtime_error("ON ERROR missing GOTO.");
    }

    if (!match(TokenType::Number)) {
        throw std::runtime_error("ON ERROR requires a line number.");
    }

    int line = std::atoi((*tokens)[pos - 1].value.c_str());
    return new OnErrorNode(line);
}

ASTNode* Parser::parseField() {
    if (!match(TokenType::Number)) {
        throw std::runtime_error("FIELD requires file number.");
    }

    int fileNum = std::atoi((*tokens)[pos - 1].value.c_str());
    std::vector<std::pair<int, std::string>> fields;

    while (match(TokenType::Separator, ",")) {
        if (!match(TokenType::Number)) {
            throw std::runtime_error("FIELD requires width.");
        }
        int width = std::atoi((*tokens)[pos - 1].value.c_str());

        if (!match(TokenType::Keyword, "AS")) {
            throw std::runtime_error("FIELD missing 'AS'.");
        }

        if (!match(TokenType::Identifier)) {
            throw std::runtime_error("FIELD requires variable name.");
        }

        std::string varName = (*tokens)[pos - 1].value;
        fields.push_back(std::make_pair(width, varName));
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
    // First, parse the left-hand side factor (like a number, variable, or parenthesis)
    ASTNode* left = parseFactor();

    // Check for '*' or '/' operators and process them
    while (true) {
        // If the next token is '*' or '/', consume it
        if (match(TokenType::Operator, "*") || match(TokenType::Operator, "/")) {
            // Store the operator (either "*" or "/")
            std::string op = (*tokens)[pos - 1].value;

            // Parse the right-hand side factor
            ASTNode* right = parseFactor();

            // Build a binary operation node and make it the new left-hand side
            left = new BinOpNode(op, left, right);
        }
        else {
            // If it's not '*' or '/', stop the loop
            break;
        }
    }

    // Return the full term expression tree
    return left;
}


ASTNode* Parser::parseFactor() {
    // If it's a number like 42 or 3.14
    if (match(TokenType::Number)) {
        std::string value = (*tokens)[pos - 1].value;
        return new NumberNode(value);
    }

    // If it's a string like "HELLO"
    if (match(TokenType::String)) {
        std::string value = (*tokens)[pos - 1].value;
        return new StringNode(value);
    }

    // If it's a variable name (identifier)
    if (match(TokenType::Identifier)) {
        std::string name = (*tokens)[pos - 1].value;
        return new IdentNode(name);
    }

    // If it starts with '(' then it's a grouped expression like (A + B)
    if (match(TokenType::Operator, "(")) {
        ASTNode* expr = parseExpression();

        // Make sure we close the parenthesis
        if (!match(TokenType::Operator, ")")) {
            throw std::runtime_error("Missing closing ')'");
        }

        return expr;
    }

    // If it's a math function like SIN(X), LOG(Y), etc.
    if (peek().type == TokenType::Keyword) {
        std::string keyword = peek().value;

        if (keyword == "SIN" || keyword == "COS" || keyword == "SQR" ||
            keyword == "LOG" || keyword == "EXP" || keyword == "ABS" ||
            keyword == "VAL" || keyword == "LEN" || keyword == "RND") {
            return parseMathFunc();
        }
    }

    // If none of the above matched, it's an error
    std::string unexpected = peek().value;
    throw std::runtime_error("Unexpected token in expression: " + unexpected);
}

ASTNode* Parser::parseMathFunc() {
    // Get the function name (e.g., SIN, LOG, etc.)
    std::string funcName = get().value;

    // Expect opening parenthesis
    if (!match(TokenType::Operator, "(")) {
        throw std::runtime_error(funcName + " requires '('");
    }

    // Parse the function argument inside the parenthesis
    ASTNode* argument = parseExpression();

    // Expect closing parenthesis
    if (!match(TokenType::Operator, ")")) {
        throw std::runtime_error(funcName + " missing ')'");
    }

    // Create a MathFuncNode with function name and its argument
    return new MathFuncNode(funcName, argument);
}


ASTNode* Parser::parseDefType() {
    // Get the DEF type keyword (e.g., DEFINT, DEFSNG)
    std::string typeName = get().value;

    // Expect a variable range after the type keyword
    if (!match(TokenType::Identifier)) {
        throw std::runtime_error(typeName + " requires variable range");
    }

    // Capture the variable range string (e.g., A-Z)
    std::string varRange = (*tokens)[pos - 1].value;

    // Return a new DefTypeNode containing the type and range
    return new DefTypeNode(typeName, varRange);
}