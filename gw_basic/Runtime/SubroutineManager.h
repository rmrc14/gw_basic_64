#ifndef SUBROUTINE_MANAGER_H
#define SUBROUTINE_MANAGER_H

#include <stack>
#include <stdexcept>

class SubroutineManager {
public:
    // Call a subroutine, push return address
    void gosub(int returnLine);

    // Return from subroutine, pop return address
    int ret();

    // Check if there is a return address to pop
    bool hasReturn() const;

    // Clear call stack
    void clear();

private:
    std::stack<int> returnStack;
};

#endif // SUBROUTINE_MANAGER_H
