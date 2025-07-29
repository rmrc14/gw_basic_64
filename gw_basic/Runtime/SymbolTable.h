
#pragma once
#include <unordered_map>
#include <string>
#include "TypeSystem.h"

class SymbolTable {
public:
    void setVariable(const std::string& name, const Value& value);
    Value getVariable(const std::string& name) const;

private:
    std::unordered_map<std::string, Value> variables_;
};
