//#include <Windows.h>
//#include <strsafe.h>
//#include <iostream>
//
//void GetDesktopResolution(int& horizontal, int& vertical)
//{
//	RECT desktop;
//	// Get a handle to the desktop window
//	const HWND hDesktop = GetDesktopWindow();
//
//	// Get the size of screen to the variable desktop
//	GetWindowRect(hDesktop, &desktop);
//	// The top left corner will have coordinates (0,0)
//	// and the bottom right corner will have coordinates
//	// (horizontal, vertical)
//	horizontal = desktop.right;
//	vertical = desktop.bottom;
//}
//
//int getNumDisplays()
//{
//    // GetSystemMetrics(SM_CMONITORS) counts only visible display monitors.
//    // This is different from EnumDisplayMonitors, which enumerates both visible display monitors and invisible pseudo-monitors that are associated with mirroring drivers.
//    // An invisible pseudo-monitor is associated with a pseudo-device used to mirror application drawing for remoting or other purposes.
//
//    return GetSystemMetrics(SM_CMONITORS);
//}
//
//int main()
//{
//	std::cout << getNumDisplays() << std::endl;
//	return 0;
//}

#include <Windows.h>
#include <cstdio>
#include <string>
#include <memory>
#include <string>
#include <map>

typedef std::shared_ptr<std::remove_pointer<HDC>::type> device_hdc;
typedef std::map<HMONITOR, device_hdc> device_hdc_map;

BOOL CALLBACK MyInfoEnumProc(
    HMONITOR hMonitor,
    HDC hdcMonitor,
    LPRECT lprcMonitor,
    LPARAM dwData)
{
    MONITORINFOEX mi;
    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(mi);

    GetMonitorInfo(hMonitor, &mi);
    printf("DisplayDevice: %s\n", mi.szDevice);

    return TRUE;
}

#include <iostream>
#include <vector>

struct DisplayBounds {
    double x;
    double y;
    double width;
    double height;
};

class DisplayInfoA {
public:
    DisplayBounds bounds;
	decltype(MONITORINFOEX::szDevice) szDevice;
};

BOOL CALLBACK MyInfoEnumProc2(
    HMONITOR hMonitor,
    HDC hdcMonitor,
    LPRECT lprcMonitor,
    LPARAM dwData)
{
    std::vector<DisplayInfoA>& displayInfos = *(std::vector<DisplayInfoA>*)dwData;

    tagMONITORINFO mi;
	MONITORINFOEX miex;

    ZeroMemory(&mi, sizeof(mi));
    mi.cbSize = sizeof(mi);
	miex.cbSize = sizeof(miex);
	GetMonitorInfo(hMonitor, &miex);
    GetMonitorInfo(hMonitor, &mi);
    DisplayInfoA di {};
    di.bounds.x = mi.rcMonitor.left;
    di.bounds.y = mi.rcMonitor.top;
    di.bounds.width = mi.rcMonitor.right - mi.rcMonitor.left;
    di.bounds.height = mi.rcMonitor.bottom - mi.rcMonitor.top;
	
	if (GetMonitorInfo(hMonitor, &miex))
	{
		memcpy(di.szDevice, miex.szDevice, sizeof(miex.szDevice));
	}
	else {
		memset(di.szDevice, 0, sizeof(di.szDevice));
	}

	/*if (GetMonitorInfo(hMonitor, &miex))
	{
		HDC dc = CreateDC(NULL, miex.szDevice, NULL, NULL);
		if (dc)
			di.hMonitor = device_hdc(dc, &::DeleteDC);
	}*/
	

    displayInfos.push_back(di);
    //    printf("DisplayDevice: %s\n", mi.szDevice);
	
    return TRUE;
}

void foo(const DisplayInfoA& info);
std::vector<DisplayInfoA> getDisplayInfos()
{
    std::vector<DisplayInfoA> infos;

	auto hdc = GetDC(NULL);
    EnumDisplayMonitors(hdc, nullptr, MyInfoEnumProc2, (LPARAM)&infos);
	ReleaseDC(nullptr, hdc);

    return infos;
}
void foo(const DisplayInfoA& info)
{
//    unsigned int cx = GetSystemMetrics(SM_CXSCREEN), cy = GetSystemMetrics(SM_CYSCREEN);
	HDC dc = CreateDC(NULL, info.szDevice, NULL, NULL);
    HDC hScreenDC = dc;
    uint32_t cx = uint32_t(info.bounds.width);
    uint32_t cy = uint32_t(info.bounds.height);
    HDC hMemDC = CreateCompatibleDC(hScreenDC);
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, cx, cy);
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
    BitBlt(hMemDC, 0, 0, cx, cy, hScreenDC, 0, 0, SRCCOPY);
    SelectObject(hMemDC, hOldBitmap);
//    ::ReleaseDC(NULL, hScreenDC);

    size_t headerSize = sizeof(BITMAPINFOHEADER) + 3 * sizeof(RGBQUAD);
    BYTE* pHeader = new BYTE[headerSize];
    LPBITMAPINFO pbmi = (LPBITMAPINFO)pHeader;
    memset(pHeader, 0, headerSize);
    pbmi->bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    pbmi->bmiHeader.biBitCount = 0;
    if (!GetDIBits(hMemDC, hBitmap, 0, 0, NULL, pbmi, DIB_RGB_COLORS)) {
        std::cout << "+++ " << std::endl;
        return;
    }

    BITMAPFILEHEADER bmf;
    if (pbmi->bmiHeader.biSizeImage <= 0)
        pbmi->bmiHeader.biSizeImage = pbmi->bmiHeader.biWidth * abs(pbmi->bmiHeader.biHeight) * (pbmi->bmiHeader.biBitCount + 7) / 8;
    BYTE* pData = new BYTE[pbmi->bmiHeader.biSizeImage];
    bmf.bfType = 0x4D42;
    bmf.bfReserved1 = bmf.bfReserved2 = 0;
    bmf.bfSize = sizeof(BITMAPFILEHEADER) + headerSize + pbmi->bmiHeader.biSizeImage;
    bmf.bfOffBits = sizeof(BITMAPFILEHEADER) + headerSize;
    if (!GetDIBits(hMemDC, hBitmap, 0, abs(pbmi->bmiHeader.biHeight), pData, pbmi, DIB_RGB_COLORS)) {
        delete pData;
        std::cout << "++999+ " << std::endl;
        return;
    }
    static int scrrenIndex = 0;
    FILE* hFile = fopen((std::to_string(scrrenIndex++) + ".bmp").c_str(), "wb");
    fwrite(&bmf, sizeof(BITMAPFILEHEADER), 1, hFile);
    fwrite(pbmi, headerSize, 1, hFile);
    fwrite(pData, pbmi->bmiHeader.biSizeImage, 1, hFile);
    fclose(hFile);

    DeleteObject(hBitmap);
    DeleteDC(hMemDC);

    delete[] pData;
}

void getScreenShot(const DisplayInfoA& info)
{
    // get the device context of the screen
	HDC dc = CreateDC(NULL, info.szDevice, NULL, NULL);

	HDC hScreenDC = dc;
    // and a device context to put it in
    HDC hMemoryDC = CreateCompatibleDC(hScreenDC);

    int width = GetDeviceCaps(hScreenDC, HORZRES);
    int height = GetDeviceCaps(hScreenDC, VERTRES);

    // maybe worth checking these are positive values
    HBITMAP hBitmap = CreateCompatibleBitmap(hScreenDC, width, height);

    // get a new bitmap
    HBITMAP hOldBitmap = (HBITMAP)SelectObject(hMemoryDC, hBitmap);

    BitBlt(hMemoryDC, 0, 0, width, height, hScreenDC, 0, 0, SRCCOPY);
    hBitmap = (HBITMAP)SelectObject(hMemoryDC, hOldBitmap);

    // clean up
    DeleteDC(hMemoryDC);
}

int main(int argc, char* argv[])
{
    auto infos = getDisplayInfos();
    for (auto& dia : infos) {
        std::cout << dia.bounds.x << ", " << dia.bounds.y << ", " << dia.bounds.width << ", " << dia.bounds.height << std::endl;
          foo(dia);
    }
    return 0;
    getDisplayInfos();
    return 0;
    printf("\n\n\\EnumDisplayDevices\n\n\n");

    DISPLAY_DEVICE dd;
    ZeroMemory(&dd, sizeof(dd));
    dd.cb = sizeof(dd);
    for (int i = 0; EnumDisplayDevices(NULL, i, &dd, 0); i++) {
        printf("\n\nDevice %d:", i);
        printf("\n    DeviceName:   '%s'", dd.DeviceName);
        printf("\n    DeviceString: '%s'", dd.DeviceString);
        printf("\n    StateFlags:   %s%s%s%s",
            ((dd.StateFlags & DISPLAY_DEVICE_ATTACHED_TO_DESKTOP) ? "desktop " : ""),
            ((dd.StateFlags & DISPLAY_DEVICE_PRIMARY_DEVICE) ? "primary " : ""),
            ((dd.StateFlags & DISPLAY_DEVICE_VGA_COMPATIBLE) ? "vga " : ""),
            ((dd.StateFlags & DISPLAY_DEVICE_MULTI_DRIVER) ? "multi " : ""),
            ((dd.StateFlags & DISPLAY_DEVICE_MIRRORING_DRIVER) ? "mirror " : ""));

        // Get more info about the device
        DISPLAY_DEVICE dd2;
        ZeroMemory(&dd2, sizeof(dd2));
        dd2.cb = sizeof(dd2);
        EnumDisplayDevices(dd.DeviceName, 0, &dd2, 0);
        printf("\n    DeviceID: '%s'", dd2.DeviceID);
        printf("\n    Monitor Name: '%s'", dd2.DeviceString);
    }

    printf("\n\n\nEnumDisplayMonitors\n\n\n");

    EnumDisplayMonitors(NULL, NULL, MyInfoEnumProc, 0);

    return 0;
}