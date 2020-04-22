#include "DisplayInfoImpl.h"
#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CFData.h>
#include <CoreGraphics/CoreGraphics.h>
#include <blend2d.h>

std::vector<DisplayInfo::Impl*> DisplayInfo::Impl::getDisplayInfos()
{
    uint32_t displayCount;
    auto ret = CGGetActiveDisplayList(0, nullptr, &displayCount);
    if (kCGErrorSuccess != ret) {
        abort();
    }
    std::vector<CGDirectDisplayID> directDisplayIds;
    directDisplayIds.resize(displayCount);
    ret = CGGetActiveDisplayList(displayCount, &directDisplayIds[0], &displayCount);
    if (kCGErrorSuccess != ret) {
        abort();
    }

    std::vector<DisplayInfo::Impl*> result;
    result.resize(directDisplayIds.size());
    for (size_t i = 0; i < directDisplayIds.size(); ++i) {
        CGRect bounds = CGDisplayBounds(directDisplayIds[i]);
        auto* di = new DisplayInfo::Impl;
        result[i] = di;
        di->bounds.x = bounds.origin.x;
        di->bounds.y = bounds.origin.y;
        di->bounds.width = bounds.size.width;
        di->bounds.height = bounds.size.height;
        di->id = directDisplayIds[i];
    }

    return result;
}

std::shared_ptr<BLImage> DisplayInfo::Impl::getScreenShot()
{
    auto cgImage = CGDisplayCreateImage(id);
    CFDataRef cgDataRef = CGDataProviderCopyData(CGImageGetDataProvider(cgImage));
    auto cgDataLen = CFDataGetLength(cgDataRef);
    auto* cgData = new uint8_t[cgDataLen];
    auto cgImageBytesPerRow = CGImageGetBytesPerRow(cgImage);
    auto imgWidth = CGImageGetWidth(cgImage);
    auto imgHeight = CGImageGetHeight(cgImage);
    CFDataGetBytes(cgDataRef, CFRangeMake(0, cgDataLen), cgData);

    CGImageRelease(cgImage);
    CFRelease(cgDataRef);

    auto blImage = std::make_shared<BLImage>(imgWidth, imgHeight, BL_FORMAT_XRGB32);
    BLImageData blImageData {};
    blImage->getData(&blImageData);
    auto* blData = (uint8_t*)blImageData.pixelData;

    for (uint32_t h = 0; h < imgHeight; ++h) {
        auto blRowStart = blData + blImageData.stride * h;
        auto cgImageRowStart = cgData + cgImageBytesPerRow * h;
        memcpy(blRowStart, cgImageRowStart, 4 * imgWidth);
    }
    delete[] cgData;

    return blImage;
}
