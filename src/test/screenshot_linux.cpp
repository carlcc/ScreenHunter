#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <blend2d.h>
#include <cstdint>
#include <cstring>
#include <vector>

void foo()
{
    Display* display = XOpenDisplay(nullptr);
    Window root = DefaultRootWindow(display);

    XWindowAttributes attributes = { 0 };
    XGetWindowAttributes(display, root, &attributes);

    auto Width = attributes.width;
    auto Height = attributes.height;

    XImage* img = XGetImage(display, root, 0, 0, Width, Height, AllPlanes, ZPixmap);
    auto BitsPerPixel = img->bits_per_pixel;

    BLImage blImg(Width, Height, BL_FORMAT_XRGB32);
    BLImageData imgData;
    blImg.getData(&imgData);

    for (int h = 0; h < Height; ++h) {
        uint8_t* dstRowPtr = (uint8_t*)imgData.pixelData + imgData.stride * h;
        uint8_t* srcRowPtr = (uint8_t*)img->data + img->bytes_per_line * h;
        memcpy(dstRowPtr, srcRowPtr, img->width * 4);
    }

    XDestroyImage(img);
    XCloseDisplay(display);

    BLImageCodec codec;
    codec.findByName("BMP");
    blImg.writeToFile("3.bmp", codec);
}

int main()
{
    foo();
}