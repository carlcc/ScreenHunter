#include "screen/DisplayInfo.h"

#ifdef SCREEN_HUNTER_MACOS
#include "screen/mac/DisplayInfoImpl.h"
#endif

#ifdef SCREEN_HUNTER_X11
#include "screen/x11/DisplayInfoImpl.h"
#endif


DisplayInfo::~DisplayInfo()
{
    delete mImpl;
}

DisplayInfo::DisplayInfo()
    : mImpl(nullptr)
{
}

const DisplayBounds& DisplayInfo::displayBounds() const
{
    return mImpl->bounds;
}

std::shared_ptr<BLImage> DisplayInfo::getScreenShot()
{
    return mImpl->getScreenShot();
}

std::vector<std::shared_ptr<DisplayInfo>> DisplayInfo::getDisplayInfos()
{
    std::vector<std::shared_ptr<DisplayInfo>> result;
    std::vector<DisplayInfo::Impl*> impls = DisplayInfo::Impl::getDisplayInfos();

    result.resize(impls.size());
    size_t i = 0;
    for (auto& r : result) {
        r = std::make_shared<DisplayInfo>();
        r->mImpl = impls[i++];
    }
    return result;
}

