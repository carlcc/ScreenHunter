//#include "SDL2/SDL.h"
//
//void GetRGBA8888Data(void*& pixels, int& width, int& height)
//{
//    // for debug
//    width = 700;
//    height = 590;
//    pixels = calloc(width * height * 4, 1); // 一像素占4字节
//}
//
//int main(int argc, char* argv[])
//{
//    SDL_Window* pWindow = nullptr;
//    SDL_Renderer* pRenderer = nullptr;
//    SDL_Texture* pTexture;
//
//    SDL_Rect srcRect;
//    SDL_Rect dstRect;
//    //初始化SDL
//    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) < 0)
//        return 0;
//    //创建窗口
//    pWindow = SDL_CreateWindow("example04: for mediaplayer", 100, 100, 700, 590, 0);
//    if (nullptr == pWindow)
//        return 0;
//    //创建Renderer
//    pRenderer = SDL_CreateRenderer(pWindow, -1, 0);
//    if (nullptr == pRenderer)
//        return 0;
//    //设置Renderer画笔颜色，透明度(此时透明度不会生效，因为没有设置BlendMode)
//    SDL_SetRenderDrawColor(pRenderer, 0, 255, 0, 255);
//
//    uint8_t * pixels;
//    int width, height, depth, pitch;
//
//    GetRGBA8888Data((void*&)pixels, width, height);
//    pitch = width * 4; // 每行图像所占字节数
//    depth = 4 * 8; // 每像素所占位数(R8位G8位B8位A8位)
//
//    int rmask, gmask, bmask, amask;
//    rmask = 0xFF000000;
//    gmask = 0x00FF0000;
//    bmask = 0x0000FF00;
//    amask = 0x000000FF;
//    //创建一个RGB Surface
////    SDL_Surface* pTmpSurface = SDL_CreateRGBSurfaceFrom(pixels, width, height, depth, pitch, rmask, gmask, bmask, amask);
////    if (nullptr == pTmpSurface)
////        return 0;
//    //创建Texture
////    pTexture = SDL_CreateTextureFromSurface(pRenderer, pTmpSurface);
//    pTexture = SDL_CreateTexture(pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_STATIC, width, height);
//    if (nullptr == pTexture)
//        return 0;
//
////    free(pixels);
//    dstRect.x = srcRect.x = 0;
//    dstRect.y = srcRect.y = 0;
//    dstRect.w = srcRect.w = width;
//    dstRect.h = srcRect.h = height;
//
//    bool isRunning = true;
//    while (isRunning) {
//        SDL_Event event;
//        while (SDL_PollEvent(&event)) {
//            switch (event.type) {
//            case SDL_QUIT:
//                isRunning = false;
//                break;
//            }
//        }
//
//        for (int i = 0; i < 100; ++i) {
//            pixels[i] += 1;
//        }
//        SDL_UpdateTexture(pTexture, nullptr, pixels, pitch);
//
//        //清除Renderer
//        SDL_RenderClear(pRenderer);
//        // Texture复制到Renderer
//        SDL_RenderCopy(pRenderer, pTexture, &srcRect, &dstRect);
//        //更新Renderer显示
//        SDL_RenderPresent(pRenderer);
//    }
//
//    //清理
//    SDL_DestroyWindow(pWindow);
//    SDL_DestroyRenderer(pRenderer);
//    SDL_DestroyTexture(pTexture);
//    SDL_Quit();
//    return 0;
//}

#include "editor/EditorWindow.h"
#include "screen/DisplayInfo.h"
#include "window/App.h"
#include "window/AppWindowManager.h"
#include <chrono>
#include <iostream>

int64_t steadyTimeMillis()
{
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}

int main(int argc, char** argv)
{
    auto startTime = steadyTimeMillis();
    auto screenInfos = DisplayInfo::getDisplayInfos();
    auto endTime = steadyTimeMillis();
    std::cout << "Capture takes: " << endTime - startTime << std::endl;

    if (screenInfos.empty()) {
        std::cout << "Not screen was captured" << std::endl;
        return -1;
    }
    BLImageCodec codec;
    codec.findByName("BMP");

    App app(argc, argv);
    std::vector<std::shared_ptr<BLImage>> images;
    std::vector<std::shared_ptr<AppWindow>> windows;
    images.reserve(screenInfos.size());
    windows.reserve(screenInfos.size());

    for (auto& si : screenInfos) {
        images.push_back(si->getScreenShot());
    }

    for (size_t i = 0; i < screenInfos.size(); ++i) {
        auto& si = screenInfos[i];
        auto& bounds = si->displayBounds();

        int x = int(bounds.x);
        int y = int(bounds.y);
        int w = int(bounds.width);
        int h = int(bounds.height);
        auto win = AppWindowManager::get().createWindow<EditorWindow>("Main", x, y, w, h);
        win->setImage(images[i]);
        windows.push_back(std::static_pointer_cast<AppWindow>(win));
    }

    return app.exec();
}