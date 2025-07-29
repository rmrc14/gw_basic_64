#include "SubroutineManager.h"
#include <stdexcept>

void SubroutineManager::push(int returnLine) {
    returnStack.push(returnLine);
}

int SubroutineManager::pop() {
    if (returnStack.empty()) {
        throw std::runtime_error("RETURN without GOSUB");
    }
    int val = returnStack.top();
    returnStack.pop();
    return val;
}

bool SubroutineManager::isEmpty() const {
    return returnStack.empty();
}
