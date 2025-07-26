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
    if (std::isdigit(token[0]))
        return Value(std::stoi(token));
    else
        return table_.getVariable(token);
}
