#pragma once
#include <stack>

class SubroutineManager {
public:
    void pushReturnAddress(int line);
    int popReturnAddress();
    bool hasReturnAddress() const;

private:
    std::stack<int> returnStack_;
};
