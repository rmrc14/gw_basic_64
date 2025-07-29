#include "SubroutineManager.h"
#include <stdexcept>

void SubroutineManager::pushReturnAddress(int line) {
    returnStack_.push(line);
}

int SubroutineManager::popReturnAddress() {
    if (returnStack_.empty()) {
        throw std::runtime_error("RETURN without GOSUB");
    }
    int line = returnStack_.top();
    returnStack_.pop();
    return line;
}

bool SubroutineManager::hasReturnAddress() const {
    return !returnStack_.empty();
}
