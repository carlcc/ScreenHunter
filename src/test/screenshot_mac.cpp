#include <ApplicationServices/ApplicationServices.h>
#include <CoreFoundation/CFData.h>
#include <blend2d.h>
#include <vector>

struct DisplayBounds {
    double x;
    double y;
    double w;
    double h;
};

struct DisplayInfo {
    DisplayBounds bounds;
    CGDirectDisplayID id;
};

class MonitorService {
public:
    static std::vector<DisplayInfo> getDisplayInfos()
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

        std::vector<DisplayInfo> result;
        result.resize(directDisplayIds.size());
        for (size_t i = 0; i < directDisplayIds.size(); ++i) {
            CGRect bounds = CGDisplayBounds(directDisplayIds[i]);
            DisplayInfo& di = result[i];
            di.bounds.x = bounds.origin.x;
            di.bounds.y = bounds.origin.y;
            di.bounds.w = bounds.size.width;
            di.bounds.h = bounds.size.height;
            di.id = directDisplayIds[i];
        }

        return result;
    }

    static std::shared_ptr<BLImage> getScreenShot(const DisplayInfo& info)
    {
        auto cgImage = CGDisplayCreateImage(info.id);

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

        //        BLImageCodec codec;
        //        codec.findByName("BMP");
        //        blImage.writeToFile("3.bmp", codec);

        //        CFStringRef file = CFSTR("lala.jpg");
        //        CFStringRef type = CFSTR("public.jpeg");
        //        CFURLRef urlRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false);
        //
        //        CGImageDestinationRef idst = CGImageDestinationCreateWithURL(urlRef, type, 1, NULL);
        //        CGImageDestinationAddImage(idst, cgImage, NULL);
        //        CGImageDestinationFinalize(idst);
    }
};
int main(int argc, char** argv)
{
    auto infos = MonitorService::getDisplayInfos();
    auto img = MonitorService::getScreenShot(infos[0]);
    BLImageCodec codec;
    codec.findByName("BMP");
    img->writeToFile("4.bmp", codec);

    return 0;

    uint32_t displayCount;
    CGDirectDisplayID activeDisplays[128];
    auto ret = CGGetActiveDisplayList(128, activeDisplays, &displayCount);

    CGImageRef screenShot = CGWindowListCreateImage(CGRectInfinite,
        kCGWindowListOptionOnScreenOnly, kCGNullWindowID, kCGWindowImageDefault);

    CFStringRef file = CFSTR("lala.jpg");
    CFStringRef type = CFSTR("public.jpeg");
    //    CFURLRef urlRef = CFURLCreateWithString(NULL, file, NULL);
    CFURLRef urlRef = CFURLCreateWithFileSystemPath(kCFAllocatorDefault, file, kCFURLPOSIXPathStyle, false);

    CGImageDestinationRef idst = CGImageDestinationCreateWithURL(urlRef, type, 1, NULL);
    CGImageDestinationAddImage(idst, screenShot, NULL);
    CGImageDestinationFinalize(idst);

    return 0;
}