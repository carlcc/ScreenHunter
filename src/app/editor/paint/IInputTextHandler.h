#pragma once

#include "event/KeyboardEvent.h"
#include <string>

class IInputTextHandler {
public:
    virtual void onText(const std::string& text) = 0;
    virtual void onKey(bool isDown, ScanCode scanCode) = 0;
};