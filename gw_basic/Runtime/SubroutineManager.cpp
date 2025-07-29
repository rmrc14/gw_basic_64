#include "SubroutineManager.h"

void SubroutineManager::gosub(int returnLine) {
    returnStack.push(returnLine);
}

int SubroutineManager::ret() {
    if (returnStack.empty()) {
        throw std::runtime_error("RETURN without matching GOSUB");
    }
    int line = returnStack.top();
    returnStack.pop();
    return line;
}

bool SubroutineManager::hasReturn() const {
    return !returnStack.empty();
}

void SubroutineManager::clear() {
    while (!returnStack.empty()) {
        returnStack.pop();
    }
}
