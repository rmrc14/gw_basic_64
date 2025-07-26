#pragma once
#include <string>

class ScreenRenderer {
public:
    static void moveCursor(int pos);  // cursor position in current line
    static void redrawBuffer(const std::string& buffer, int cursorPos);
};
