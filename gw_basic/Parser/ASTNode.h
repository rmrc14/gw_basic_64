#pragma once
#include <string>
#include <vector>

enum class ASTType {
    Program,
    PrintStmt,
    LetStmt,
    IfElseStmt,
    //IfStmt,
    ForStmt,
    GotoStmt,
    InputStmt,
    DataStmt,
    ReadStmt,
    GosubStmt,
    ReturnStmt,
    StopStmt,
    RemStmt,
    NumberExpr,
    IdentExpr,
    BinOpExpr,
    StringExpr,
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
        for (auto s : stmts) {
            delete s;
        }
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
    ~PrintNode() {
        delete expr;
    }
    ASTType type() const override {
        return ASTType::PrintStmt;
    }
};
//========
//class IfNode :public ASTNode {
//public:
//    ASTNode* lhs;
//    std::string op;
//    ASTNode* rhs;
//    ASTNode* thenStmt;
//
//    IfNode(ASTNode* l,const std::string& o,ASTNode* r,ASTNode* t)
//        :lhs(l),op(o),rhs(r),thenStmt(t){}
//    ~IfNode() {
//        delete lhs;
//        delete rhs;
//        delete thenStmt;
//    }
//
//    ASTType type() const override {
//        return ASTType::IfStmt;
//}
//};


class LetNode : public ASTNode {
public:
    std::string name;
    ASTNode* expr;
    LetNode(const std::string& n, ASTNode* e)
        : name(n), expr(e) {}
    ~LetNode() {
        delete expr;
    }
    ASTType type() const override {
        return ASTType::LetStmt;
    }
};
//
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
    BinOpNode(const std::string& o, ASTNode* l, ASTNode* r)
        : op(o), left(l), right(r) {}
    ~BinOpNode() {
        delete left; delete right;
    }
    ASTType type() const override {
        return ASTType::BinOpExpr;
    }
};


class ForNode :public ASTNode {
public:
    std::string var;
    ASTNode* start;
    ASTNode* end;
    ASTNode* step;
    ASTNode* body;
    ForNode(const std::string& v, ASTNode* s, ASTNode* e, ASTNode* st, ASTNode* b) {
        var = v;
        start = s;
        end = e;
        step = st;
        body = b;
    }

    ~ForNode() {
        delete start;
        delete end;
        delete step;
        delete body;
    }

    ASTType type() const override {
        return ASTType::ForStmt;
    }
};

class GotoNode :public ASTNode {
public:
    int line;
    GotoNode(int n) {
        line = n;
    }
    ASTType type() const override {
        return ASTType::GotoStmt;
    }
};
class IfElseNode :public ASTNode {
public:
    ASTNode* left;
    std::string op;
    ASTNode* right;
    ASTNode* thenStmt;
    ASTNode* elseStmt;

    IfElseNode(ASTNode* l, const std::string& o, ASTNode* r, ASTNode* t, ASTNode* e) {
        left = l;
        op = o;
        right = r;
        thenStmt = t;
        elseStmt = e;
    }

    ~IfElseNode() {
        delete left;
        delete right;
        delete thenStmt;
        if (elseStmt) {
            delete elseStmt;
        }
    }
    ASTType type() const override {
        return ASTType::IfElseStmt;
    }

};

class InputNode :public ASTNode {
public:
    std::string name;
    InputNode(const std::string& n) {
        name = n;
    }
    ASTType type() const override {
        return ASTType::InputStmt;
    }
};

class StringNode : public ASTNode {
public:
    std::string value;
    StringNode(const std::string& v) {
        value = v;
    }
    ASTType type() const override {
        return ASTType::StringExpr;
    }
};