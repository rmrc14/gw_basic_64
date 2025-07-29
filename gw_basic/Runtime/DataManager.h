#pragma once
#include <vector>
#include "TypeSystem.h"

class DataManager {
    std::vector<Value> data_;
    size_t current_ = 0;

public:
    void addData(const std::vector<Value>& values);
    Value readNext();
    void reset();
};
