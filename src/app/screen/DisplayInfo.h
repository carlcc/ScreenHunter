#pragma once
#include <memory>
#include <vector>

class BLImage;

struct DisplayBounds {
    double x;
    double y;
    double width;
    double height;
};

class DisplayInfo {
public:
    DisplayInfo();
    ~DisplayInfo();

    DisplayInfo(const DisplayInfo&) = delete;
    DisplayInfo(DisplayInfo&&) = delete;
    DisplayInfo& operator=(const DisplayInfo&) = delete;
    DisplayInfo& operator=(DisplayInfo&&) = delete;

    const DisplayBounds& displayBounds() const;

    std::shared_ptr<BLImage> getScreenShot();

    static std::vector<std::shared_ptr<DisplayInfo>> getDisplayInfos();

private:

    class Impl;
    Impl* mImpl;
};