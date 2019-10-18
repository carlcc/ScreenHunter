#pragma once

#include "screen/DisplayInfo.h"
#include <ApplicationServices/ApplicationServices.h>

class DisplayInfo::Impl {
public:
    static std::vector<DisplayInfo::Impl*> getDisplayInfos();

    std::shared_ptr<BLImage> getScreenShot();

    DisplayBounds bounds;
    CGDirectDisplayID id;
};
