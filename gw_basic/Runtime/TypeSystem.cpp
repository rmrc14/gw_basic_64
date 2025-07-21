#include "TypeSystem.h"
#include <stdexcept>

Value::Value() : type_(ValueType::INT), data_(0) {}

Value::Value(int i) : type_(ValueType::INT), data_(i) {}

Value::Value(float f) : type_(ValueType::FLOAT), data_(f) {}

Value::Value(const std::string& s) : type_(ValueType::STRING), data_(s) {}

ValueType Value::getType() const {
    return type_;
}

int Value::asInt() const {
    if (type_ == ValueType::INT) return std::get<int>(data_);
    throw std::runtime_error("Value is not INT");
}

float Value::asFloat() const {
    if (type_ == ValueType::FLOAT) return std::get<float>(data_);
    throw std::runtime_error("Value is not FLOAT");
}

std::string Value::asString() const {
    if (type_ == ValueType::STRING) return std::get<std::string>(data_);
    throw std::runtime_error("Value is not STRING");
}
