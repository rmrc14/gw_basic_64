#pragma once
#include "ASTNode.h"
#include "SymbolTable.h"
#include "TypeSystem.h"
#include "ExpressionEvaluator.h"

class FlowControl {
public:
    FlowControl(SymbolTable& table);

    bool evaluateCondition(const std::string& op, Value left, Value right);

private:
    SymbolTable& table_;
};
