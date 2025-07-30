#include "ExpressionEvaluator.h"
#include <sstream>
#include <stdexcept>
#include <cctype>

ExpressionEvaluator::ExpressionEvaluator(SymbolTable& table)
    : table_(table) {}

Value ExpressionEvaluator::evaluate(const std::string& expr) {
    std::istringstream iss(expr);
    std::string token;
    int result = 0;
    char op = '+';  // Start with addition

    while (iss >> token) {
        Value val = parseToken(token);
        int num = val.asInt();

        switch (op) {
        case '+': result += num; break;
        case '-': result -= num; break;
        case '*': result *= num; break;
        case '/':
            if (num == 0) throw std::runtime_error("Division by zero");
            result /= num;
            break;

        default:
            throw std::runtime_error("Unknown operator");
        }
        iss >> op;
    }

    return Value(result);
}
Value ExpressionEvaluator::parseToken(const std::string& token) {
    if (token.size() >= 2 && token.front() == '"' && token.back() == '"') {
        // It's a string literal: remove quotes
        return Value(token.substr(1, token.size() - 2));
    }
    else if (std::isdigit(token[0]) ||
        (token[0] == '-' && token.size() > 1 && std::isdigit(token[1]))) {
        // It's an integer number
        return Value(std::stoi(token));
    }
    else {
        // It's a variable
        return table_.getVariable(token);
    }
}
