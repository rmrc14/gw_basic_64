#include "SymbolTable.h"
#include <stdexcept>

void SymbolTable::setVariable(const std::string& name, const Value& value) {
    variables_[name] = value;
}

Value SymbolTable::getVariable(const std::string& name) const {
    auto it = variables_.find(name);
    if (it == variables_.end()) {
        throw std::runtime_error("Variable '" + name + "' not defined");
    }
    return it->second;
}