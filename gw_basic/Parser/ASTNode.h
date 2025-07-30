#pragma once
#include <string>
#include <vector>

enum class ASTType {
    Program,
    PrintStmt,
    LetStmt,
    IfElseStmt,
    ForStmt,
    NextStmt,
    WhileStmt,
    DoLoopStmt,
    GotoStmt,
    InputStmt,
    DataStmt,
    ReadStmt,
    GosubStmt,
    ReturnStmt,
    StopStmt,
    RemStmt,
    OnErrorStmt,
    FieldStmt,
    CommandStmt,
    NumberExpr,
    IdentExpr,
    BinOpExpr,
    StringExpr,
    MathFuncExpr,
    DefTypeStmt,
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
    ASTType type() const override {
        return ASTType::Program;
    }
};

class DataNode : public ASTNode {
public:
    std::vector<std::string> values;
    DataNode(const std::vector<std::string>& vals) : values(vals) {}
    ASTType type() const override {
        return ASTType::DataStmt;
    }
};

class ReadNode : public ASTNode {
public:
    std::string var;
    ReadNode(const std::string& n) : var(n) {}
    ASTType type() const override {
        return ASTType::ReadStmt;
    }
};

class GosubNode : public ASTNode {
public:
    int line;
    GosubNode(int ln) : line(ln) {}
    ASTType type() const override {
        return ASTType::GosubStmt;
    }
};

class ReturnNode : public ASTNode {
public:
    ASTType type() const override {
        return ASTType::ReturnStmt;
    }
};

class StopNode : public ASTNode {
public:
    ASTType type() const override {
        return ASTType::StopStmt;
    }
};

class RemNode : public ASTNode {
public:
    std::string comment;
    RemNode(const std::string& c) : comment(c) {}
    ASTType type() const override {
        return ASTType::RemStmt;
    }
};

class PrintNode : public ASTNode {
public:
    ASTNode* expr;
    PrintNode(ASTNode* e) : expr(e) {}
    ~PrintNode() { delete expr; }
    ASTType type() const override {
        return ASTType::PrintStmt;
    }
};

class LetNode : public ASTNode {
public:
    std::string name;
    ASTNode* expr;
    LetNode(const std::string& n, ASTNode* e) : name(n), expr(e) {}
    ~LetNode() { delete expr; }
    ASTType type() const override {
        return ASTType::LetStmt;
    }
};

class NumberNode : public ASTNode {
public:
    std::string value;
    NumberNode(const std::string& v) : value(v) {}
    ASTType type() const override {
        return ASTType::NumberExpr;
    }
};

class IdentNode : public ASTNode {
public:
    std::string name;
    IdentNode(const std::string& n) : name(n) {}
    ASTType type() const override {
        return ASTType::IdentExpr;
    }
};

class BinOpNode : public ASTNode {
public:
    std::string op;
    ASTNode* left;
    ASTNode* right;
    BinOpNode(const std::string& o, ASTNode* l, ASTNode* r) : op(o), left(l), right(r) {}
    ~BinOpNode() { delete left; delete right; }
    ASTType type() const override {
        return ASTType::BinOpExpr;
    }
};

class MathFuncNode : public ASTNode {
public:
    std::string func;
    ASTNode* argument;
    MathFuncNode(const std::string& f, ASTNode* arg) : func(f), argument(arg) {}
    ~MathFuncNode() { delete argument; }
    ASTType type() const override {
        return ASTType::MathFuncExpr;
    }
};

class DefTypeNode : public ASTNode {
public:
    std::string typ, range;
    DefTypeNode(const std::string& t, const std::string& r) : typ(t), range(r) {}
    ASTType type() const override {
        return ASTType::DefTypeStmt;
    }
};

class ForNode : public ASTNode {
public:
    std::string var;
    ASTNode* start, * end, * step, * body;
    ForNode(const std::string& v, ASTNode* s, ASTNode* e, ASTNode* st, ASTNode* b)
        : var(v), start(s), end(e), step(st), body(b) {}
    ~ForNode() { delete start; delete end; delete step; delete body; }
    ASTType type() const override {
        return ASTType::ForStmt;
    }
};

class NextNode : public ASTNode {
public:
    std::string var;
    NextNode(const std::string& v) : var(v) {}
    ASTType type() const override {
        return ASTType::NextStmt;
    }
};

class WhileNode : public ASTNode {
public:
    ASTNode* cond, * body;
    WhileNode(ASTNode* c, ASTNode* b) : cond(c), body(b) {}
    ~WhileNode() { delete cond; delete body; }
    ASTType type() const override {
        return ASTType::WhileStmt;
    }
};

class DoLoopNode : public ASTNode {
public:
    ASTNode* body, * cond;
    bool untilStyle;
    DoLoopNode(ASTNode* b, ASTNode* c, bool u) : body(b), cond(c), untilStyle(u) {}
    ~DoLoopNode() { delete body; if (cond) delete cond; }
    ASTType type() const override {
        return ASTType::DoLoopStmt;
    }
};

class GotoNode : public ASTNode {
public:
    int line;
    GotoNode(int n) : line(n) {}
    ASTType type() const override {
        return ASTType::GotoStmt;
    }
};

class IfElseNode : public ASTNode {
public:
    ASTNode* left, * right, * thenStmt, * elseStmt;
    std::string op;
    IfElseNode(ASTNode* l, const std::string& o, ASTNode* r, ASTNode* t, ASTNode* e)
        : left(l), op(o), right(r), thenStmt(t), elseStmt(e) {}
    ~IfElseNode() {
        delete left; delete right;
        delete thenStmt;
        if (elseStmt) delete elseStmt;
    }
    ASTType type() const override {
        return ASTType::IfElseStmt;
    }
};

class InputNode : public ASTNode {
public:
    std::string name;
    InputNode(const std::string& n) : name(n) {}
    ASTType type() const override {
        return ASTType::InputStmt;
    }
};

class StringNode : public ASTNode {
public:
    std::string value;
    StringNode(const std::string& v) : value(v) {}
    ASTType type() const override {
        return ASTType::StringExpr;
    }
};

class OnErrorNode : public ASTNode {
public:
    int line;
    OnErrorNode(int ln) : line(ln) {}
    ASTType type() const override {
        return ASTType::OnErrorStmt;
    }
};

class FieldNode : public ASTNode {
public:
    int fileNum;
    std::vector<std::pair<int, std::string>> fields;
    FieldNode(int f, const std::vector<std::pair<int, std::string>>& fs)
        : fileNum(f), fields(fs) {}
    ASTType type() const override {
        return ASTType::FieldStmt;
    }
};

class CommandNode : public ASTNode {
public:
    std::string cmd;
    std::vector<std::string> args;
    CommandNode(const std::string& c, const std::vector<std::string>& a)
        : cmd(c), args(a) {}
    ASTType type() const override {
        return ASTType::CommandStmt;
    }
};