#include "DataManager.h"
#include <stdexcept>

void DataManager::load(const std::vector<Value>& newValues) {
    values_.insert(values_.end(), newValues.begin(), newValues.end());
}

Value DataManager::readNext() {
    if (index_ >= values_.size()) {
        throw std::runtime_error("READ past end of DATA");
    }
    return values_[index_++];
}

void DataManager::restore() {
    index_ = 0;
}
