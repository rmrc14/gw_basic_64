
#pragma once
#include <variant>
#include <string>

enum class ValueType { INT, FLOAT, STRING };

class Value {
public:
    Value();                     // Default constructor
    Value(int i);
    Value(float f);
    Value(const std::string& s);

    ValueType getType() const;

    int asInt() const;
    float asFloat() const;
    std::string asString() const;

private:
    ValueType type_;
    std::variant<int, float, std::string> data_;
};