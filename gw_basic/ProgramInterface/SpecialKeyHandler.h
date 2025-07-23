#pragma once

#include <string>
#include "SystemInterface.h"
#include <vector>


class SpecialKeyHandler {
public:
    SpecialKeyHandler() = default;

    // Instead of executing, just inject command into buffer
    bool handleSpecialKey(const check_SKey& key, std::string& inputBuffer);
};

///////////////////////////////////////
// SystemInterface.h



