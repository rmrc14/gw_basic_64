#include "StatementExecutor.h"
#include <iostream>

StatementExecutor::StatementExecutor(SymbolTable& table, ProgramMemory& mem)
    : table_(table), evaluator_(table), programMemory_(mem), flowControl_(table) {}

void StatementExecutor::setCurrentLine(int line) {
    currentLine_ = line;
    jumpToLine_ = -1;  // Reset previous jump
}

int StatementExecutor::getNextLine(int line) const {
    return (jumpToLine_ != -1) ? jumpToLine_ : programMemory_.getNextLineNumber(line);
}

void StatementExecutor::requestJump(int targetLine) {
    jumpToLine_ = targetLine;
}


void StatementExecutor::execute(ASTNode* node) {
    if (!node) return;

    switch (node->type()) {
    case ASTType::IfElseStmt:
        executeIf(node);
        break;

    case ASTType::Program: 
    {
        ProgramNode* prog = static_cast<ProgramNode*>(node);
        for (auto stmt : prog->stmts) {
            execute(stmt);
        }
        break;
    }

    case ASTType::PrintStmt:
        executePrint(static_cast<PrintNode*>(node));
        break;

    case ASTType::LetStmt:
        executeLet(static_cast<LetNode*>(node));
        break;

    /*case ASTType::IfElseStmt:
        executeIf(node);
        break;*/

    /*case ASTType::GotoStmt: {
        GotoNode* gotoNode = static_cast<GotoNode*>(node);
        requestJump(gotoNode->targetLine);
        break;
    }*/

    default:
        // Can add more like FOR, WHILE etc.
        break;
    }
}

void StatementExecutor::executePrint(PrintNode* printNode) {
    Value result = evaluateExpr(printNode->expr);

    switch (result.getType()) {
    case ValueType::INT:
        std::cout << result.asInt() << std::endl;
        break;
    case ValueType::FLOAT:
        std::cout << result.asFloat() << std::endl;
        break;
    case ValueType::STRING:
        std::cout << result.asString() << std::endl;
        break;
    default:
        std::cout << "Unknown value type" << std::endl;
    }
}

void StatementExecutor::executeLet(LetNode* letNode) {
    Value result = evaluateExpr(letNode->expr);
    table_.setVariable(letNode->name, result);
}

Value StatementExecutor::evaluateExpr(ASTNode* exprNode) {
    switch (exprNode->type()) {
    case ASTType::NumberExpr:
        return Value(std::stoi(static_cast<NumberNode*>(exprNode)->value));

    case ASTType::IdentExpr:
        return table_.getVariable(static_cast<IdentNode*>(exprNode)->name);

    case ASTType::StringExpr:
        return Value(static_cast<StringNode*>(exprNode)->value);

    case ASTType::BinOpExpr: {
        BinOpNode* bin = static_cast<BinOpNode*>(exprNode);
        Value leftVal = evaluateExpr(bin->left);
        Value rightVal = evaluateExpr(bin->right);

        if (bin->op == "+") {
            if (leftVal.getType() == ValueType::STRING || rightVal.getType() == ValueType::STRING)
                return Value(leftVal.asString() + rightVal.asString());
            return Value(leftVal.asInt() + rightVal.asInt());
        }
        else if (bin->op == "-") {
            return Value(leftVal.asInt() - rightVal.asInt());
        }
        else if (bin->op == "*") {
            return Value(leftVal.asInt() * rightVal.asInt());
        }
        else if (bin->op == "/") {
            if (rightVal.asInt() == 0) throw std::runtime_error("Division by zero");
            return Value(leftVal.asInt() / rightVal.asInt());
        }
        else {
            throw std::runtime_error("Unknown operator: " + bin->op);
        }
    }

    default:
        throw std::runtime_error("Unsupported expression type");
    }
}

void StatementExecutor::executeIf(ASTNode* node) {
    auto* ifElse = static_cast<IfElseNode*>(node);
    ASTNode* lhs = ifElse->left;
    std::string op = ifElse->op;
    ASTNode* rhs = ifElse->right;
    ASTNode* thenStmt = ifElse->thenStmt;
    ASTNode* elseStmt = ifElse->elseStmt;

    Value leftVal = evaluateExpr(lhs);
    Value rightVal = evaluateExpr(rhs);

    if (flowControl_.evaluateCondition(op, leftVal, rightVal)) {
        execute(thenStmt);
    }
    else if (elseStmt) {
        execute(elseStmt);
    }
}
