
#pragma once
#include <string>
#include "SymbolTable.h"

class ExpressionEvaluator {
public:
    ExpressionEvaluator(SymbolTable& table);
    Value evaluate(const std::string& expr);

private:
    SymbolTable& table_;
    Value parseToken(const std::string& token);
};