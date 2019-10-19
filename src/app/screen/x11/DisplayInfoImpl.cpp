#include "DisplayInfoImpl.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <blend2d.h>
#include <cstdint>
#include <cstring>
#include <vector>

class DisplayInfoImplConst {
public:
    static DisplayInfoImplConst& get()
    {
        static DisplayInfoImplConst i;
        return i;
    }

private:
    DisplayInfoImplConst()
    {
        mXDisplay = XOpenDisplay(nullptr);
    }
    ~DisplayInfoImplConst()
    {
        if (mXDisplay) {
            XCloseDisplay(mXDisplay);
        }
    }

public:
    Display* mXDisplay;
};

std::vector<DisplayInfo::Impl*> DisplayInfo::Impl::getDisplayInfos()
{
    Display* xDisplay = DisplayInfoImplConst::get().mXDisplay;

    auto displayCount = XScreenCount(xDisplay);

    std::vector<DisplayInfo::Impl*> result;
    result.resize(displayCount);

    int i = 0;
    for (auto& dii : result) {
        dii = new DisplayInfo::Impl;
        Screen* screen = ScreenOfDisplay(xDisplay, i);
        Window root = RootWindowOfScreen(screen);

        XWindowAttributes attributes = { 0 };
        XGetWindowAttributes(xDisplay, root, &attributes);
        dii->bounds.x = attributes.x;
        dii->bounds.y = attributes.y;
        dii->bounds.width = attributes.width;
        dii->bounds.height = attributes.height;
        dii->screen = root;
    }

    return result;
}

std::shared_ptr<BLImage> DisplayInfo::Impl::getScreenShot()
{
    Display* xDisplay = DisplayInfoImplConst::get().mXDisplay;

    int x = int(bounds.x);
    int y = int(bounds.y);
    int w = int(bounds.width);
    int h = int(bounds.height);
    XImage* img = XGetImage(xDisplay, screen, x, y, w, h, AllPlanes, ZPixmap);
    auto BitsPerPixel = img->bits_per_pixel;

    auto blImg = std::make_shared<BLImage>(w, h, BL_FORMAT_XRGB32);
    BLImageData imgData {};
    blImg->getData(&imgData);

    for (int i = 0; i < h; ++i) {
        uint8_t* dstRowPtr = (uint8_t*)imgData.pixelData + imgData.stride * i;
        uint8_t* srcRowPtr = (uint8_t*)img->data + img->bytes_per_line * i;
        memcpy(dstRowPtr, srcRowPtr, img->width * 4);
    }

    XDestroyImage(img);

    return blImg;
}
