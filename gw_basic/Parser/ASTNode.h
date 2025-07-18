#pragma once
#include <memory>
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
    virtual ~ASTNode() = default;
    virtual ASTType type() const = 0;
};

using AST = std::shared_ptr<ASTNode>;

class ProgramNode : public ASTNode {
public:
    std::vector<AST> stmts;
    ASTType type() const override { return ASTType::Program; }
};

class PrintNode : public ASTNode {
public:
    AST expr;
    explicit PrintNode(AST e) : expr(std::move(e)) {}
    ASTType type() const override { return ASTType::PrintStmt; }
};

class LetNode : public ASTNode {
public:
    std::string name;
    AST expr;
    LetNode(std::string n, AST e)
        : name(std::move(n)), expr(std::move(e)) {}
    ASTType type() const override { return ASTType::LetStmt; }
};

class NumberNode : public ASTNode {
public:
    std::string value;
    explicit NumberNode(std::string v) : value(std::move(v)) {}
    ASTType type() const override { return ASTType::NumberExpr; }
};

class IdentNode : public ASTNode {
public:
    std::string name;
    explicit IdentNode(std::string n) : name(std::move(n)) {}
    ASTType type() const override { return ASTType::IdentExpr; }
};

class BinOpNode : public ASTNode {
public:
    std::string op;
    AST left, right;
    BinOpNode(std::string o, AST l, AST r)
        : op(std::move(o)), left(std::move(l)), right(std::move(r)) {}
    ASTType type() const override { return ASTType::BinOpExpr; }
};
