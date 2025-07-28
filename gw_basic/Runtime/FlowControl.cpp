#include "FlowControl.h"
#include <stdexcept>

FlowControl::FlowControl(SymbolTable& table)
    : table_(table) {}

bool FlowControl::evaluateCondition(const std::string& op, Value left, Value right) {
    int l = left.asInt();
    int r = right.asInt();

    if (op == "=") return l == r;
    if (op == "<") return l < r;
    if (op == ">") return l > r;
    if (op == "<=") return l <= r;
    if (op == ">=") return l >= r;
    if (op == "<>") return l != r;

    throw std::runtime_error("Invalid comparison operator: " + op);

