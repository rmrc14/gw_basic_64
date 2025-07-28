#pragma once
#include <vector>
#include "TypeSystem.h"  // Value

class DataManager {
public:
    void load(const std::vector<Value>& newValues);
    Value readNext();
    void restore(); // Optional: resets read pointer

private:
    std::vector<Value> values_;
    size_t index_ = 0;
};
