#ifndef COMMANDLINEEDITOR_H
#define COMMANDLINEEDITOR_H

#include <string>
#include "SpecialKeyHandler.h"

class CommandLineEditor {
public:
    CommandLineEditor();
    void setSpecialKeyHandler(SpecialKeyHandler* handler);
    bool getLineFromCli(std::string& line);

private:
    std::string buffer;
    int cursorPosition;
    SpecialKeyHandler* specialKeyHandler;
};

#endif

