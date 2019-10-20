#pragma once

#include "screen/DisplayInfo.h"
#include <Windows.h>

class DisplayInfo::Impl {
public:
    static std::vector<DisplayInfo::Impl*> getDisplayInfos();

    std::shared_ptr<BLImage> getScreenShot();

    DisplayBounds bounds;
    decltype(MONITORINFOEX::szDevice) szDevice;
};
