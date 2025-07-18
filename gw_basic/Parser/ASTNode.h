#pragma once
#include <string>
#include <vector>

enum class ASTType {
    Program,
    PrintStmt,
    LetStmt,
    NumberExpr,
    IdentExpr,
    BinOpExpr
};

class ASTNode {
public:
    virtual ~ASTNode() {}
    virtual ASTType type() const = 0;
};

using ASTList = std::vector<ASTNode*>;

class ProgramNode : public ASTNode {
public:
    ASTList stmts;
    ~ProgramNode() {
        for (auto s : stmts) delete s;
    }
    ASTType type() const override { return ASTType::Program; }
};

class PrintNode : public ASTNode {
public:
    ASTNode* expr;
    PrintNode(ASTNode* e) : expr(e) {}
    ~PrintNode() { delete expr; }
    ASTType type() const override { return ASTType::PrintStmt; }
};

class LetNode : public ASTNode {
public:
    std::string name;
    ASTNode* expr;
    LetNode(const std::string& n, ASTNode* e)
        : name(n), expr(e) {}
    ~LetNode() { delete expr; }
    ASTType type() const override { return ASTType::LetStmt; }
};

class NumberNode : public ASTNode {
public:
    std::string value;
    NumberNode(const std::string& v) : value(v) {}
    ASTType type() const override { return ASTType::NumberExpr; }
};

class IdentNode : public ASTNode {
public:
    std::string name;
    IdentNode(const std::string& n) : name(n) {}
    ASTType type() const override { return ASTType::IdentExpr; }
};

class BinOpNode : public ASTNode {
public:
    std::string op;
    ASTNode* left;
    ASTNode* right;
    BinOpNode(const std::string& o, ASTNode* l, ASTNode* r)
        : op(o), left(l), right(r) {}
    ~BinOpNode() { delete left; delete right; }
    ASTType type() const override { return ASTType::BinOpExpr; }
};
