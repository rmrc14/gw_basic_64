#include "DataManager.h"
#include <stdexcept>

void DataManager::addData(const std::vector<Value>& values) {
    data_.insert(data_.end(), values.begin(), values.end());
}

Value DataManager::readNext() {
    if (current_ >= data_.size()) {
        throw std::runtime_error("No more DATA values to READ");
    }
    return data_[current_++];
}

void DataManager::reset() {
    current_ = 0;
}
