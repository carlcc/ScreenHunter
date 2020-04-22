#pragma once

#include "screen/DisplayInfo.h"
#include <X11/X.h>
#include <X11/Xlib.h>

class DisplayInfo::Impl {
public:
    static std::vector<DisplayInfo::Impl*> getDisplayInfos();

    std::shared_ptr<BLImage> getScreenShot();

    DisplayBounds bounds;
    Window screen;
};
