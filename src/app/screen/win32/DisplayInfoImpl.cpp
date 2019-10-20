#include "screen/win32/DisplayInfoImpl.h"
#include <algorithm>
#include <blend2d.h>

void deleteHDC(HDC hdc)
{
    ::ReleaseDC(nullptr, hdc);
    ::DeleteDC(hdc);
}

std::vector<DisplayInfo::Impl*> DisplayInfo::Impl::getDisplayInfos()
{
    std::vector<DisplayInfo::Impl*> result;

    auto hdc = GetDC(nullptr);
    struct EnumDisplayHelper {
        static BOOL CALLBACK callback(HMONITOR hMonitor, HDC hdcMonitor, LPRECT lprcMonitor, LPARAM dwData)
        {
            auto& displayInfos = *(std::vector<DisplayInfo::Impl*>*)dwData;

            MONITORINFOEX mi;

            ZeroMemory(&mi, sizeof(mi));
            mi.cbSize = sizeof(mi);

            if (GetMonitorInfo(hMonitor, &mi)) {
                auto* dii = new DisplayInfo::Impl;
                dii->bounds.x = mi.rcMonitor.left;
                dii->bounds.y = mi.rcMonitor.top;
                dii->bounds.width = mi.rcMonitor.right - mi.rcMonitor.left;
                dii->bounds.height = mi.rcMonitor.bottom - mi.rcMonitor.top;
                memcpy(dii->szDevice, mi.szDevice, sizeof(mi.szDevice));
                displayInfos.push_back(dii);
            }

            return TRUE;
        }
    };
    EnumDisplayMonitors(hdc, nullptr, EnumDisplayHelper::callback, (LPARAM)&result);
    deleteHDC(hdc);

    return result;
}

std::shared_ptr<BLImage> DisplayInfo::Impl::getScreenShot()
{
    HDC hScreenDC = CreateDC(nullptr, szDevice, nullptr, nullptr);
    std::shared_ptr<std::remove_pointer<HDC>::type> spScreenDC(hScreenDC, deleteHDC);

    auto cx = uint32_t(bounds.width);
    auto cy = uint32_t(bounds.height);

    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, cx, cy);
    std::shared_ptr<std::remove_pointer<HDC>::type> spMemDc(hMemDC, deleteHDC);
    std::shared_ptr<std::remove_pointer<HBITMAP>::type> spBitmap(hBitmap, ::DeleteObject);

    auto hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, cx, cy, hScreenDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);

    const size_t kHeaderSize = sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD);
    BYTE pHeader[kHeaderSize];
    auto pbmi = reinterpret_cast<LPBITMAPINFO>(pHeader);
    memset(pHeader, 0, kHeaderSize);

    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biBitCount = 0;
    if (!GetDIBits(hMemDC, hBitmap, 0, 0, nullptr, pbmi, DIB_RGB_COLORS)) {
        return nullptr;
    }

    auto blImage = std::make_shared<BLImage>(pbmi->bmiHeader.biWidth, pbmi->bmiHeader.biHeight, BL_FORMAT_XRGB32);
    BLImageData blImageData {};
    blImage->getData(&blImageData);

    std::unique_ptr<uint8_t> tmpData(new uint8_t[pbmi->bmiHeader.biSizeImage]);
    if (!GetDIBits(hMemDC, hBitmap, 0, abs(pbmi->bmiHeader.biHeight), tmpData.get(), pbmi, DIB_RGB_COLORS)) {
        return nullptr;
    }
    for (int i = 0; i < pbmi->bmiHeader.biHeight; ++i) {
        auto* dstRowPtr = (uint8_t*)blImageData.pixelData + blImageData.stride * i;
        auto* srcRowPtr = (uint8_t*)tmpData.get() + pbmi->bmiHeader.biWidth * 4 * (pbmi->bmiHeader.biHeight - i - 1);
        memcpy(dstRowPtr, srcRowPtr, pbmi->bmiHeader.biWidth * 4);
    }

    return blImage;
}
