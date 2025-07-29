#pragma once
#include <stack>

class SubroutineManager {
public:
    void push(int returnLine);
    int pop();
    bool isEmpty() const;

private:
    std::stack<int> returnStack;
};
