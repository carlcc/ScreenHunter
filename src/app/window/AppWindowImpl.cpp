#include "window/AppWindowImpl.h"
#include <iostream>

AppWindow::Impl::Impl()
    : mSDLWindow(nullptr)
    , mRenderer(nullptr)
    , mTexture(nullptr)
{
}

AppWindow::Impl::~Impl()
{
}

bool AppWindow::Impl::init(const char* title, int x, int y, int w, int h, AppWindow* _this)
{
    auto* window = SDL_CreateWindow(title, x, y, w, h, 0);
    if (window == nullptr) {
        return false;
    }

    SDL_SetWindowData(window, "app_win", _this);
    mSDLWindow = std::shared_ptr<SDL_Window>(window, [](auto* p) {
        SDL_DestroyWindow(p);
    });

    auto* renderer = SDL_CreateRenderer(window, -1, 0);
    if (nullptr == renderer)
        return false;
    mRenderer = std::shared_ptr<SDL_Renderer>(renderer, [](auto* r) {
        SDL_DestroyRenderer(r);
    });
    int rmask, gmask, bmask, amask;
    rmask = 0xFF000000;
    gmask = 0x00FF0000;
    bmask = 0x0000FF00;
    amask = 0x000000FF;

    auto* img = new BLImage(w, h, BL_FORMAT_PRGB32);
    mTextureBuf = std::shared_ptr<BLImage>(img);
    BLImageData imgInfo {};
    mTextureBuf->getData(&imgInfo);

    SDL_Surface* pTmpSurface = SDL_CreateRGBSurfaceFrom(imgInfo.pixelData, w, h, 4 * 8, imgInfo.stride, rmask, gmask, bmask, amask);
    std::cout << SDL_GetError() << std::endl;
    if (nullptr == pTmpSurface)
        return false;

    auto* texture = SDL_CreateTextureFromSurface(renderer, pTmpSurface);

    SDL_FreeSurface(pTmpSurface);

//    auto* texture = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_ARGB32, SDL_TEXTUREACCESS_STATIC, mWidth, mHeight);

    mTexture = std::shared_ptr<SDL_Texture>(texture, [](auto* t) {
        SDL_DestroyTexture(t);
    });

    return true;
}

void AppWindow::Impl::onPaint()
{
    BLImageData imgInfo {};
    mTextureBuf->getData(&imgInfo);

    SDL_UpdateTexture(mTexture.get(), nullptr, imgInfo.pixelData, imgInfo.stride);

    //清除Renderer
    SDL_RenderClear(mRenderer.get());
    // Texture复制到Renderer
    SDL_RenderCopy(mRenderer.get(), mTexture.get(), nullptr, nullptr);
    //更新Renderer显示
    SDL_RenderPresent(mRenderer.get());

    mNeedRepaint = false;
}

void AppWindow::Impl::setTitle(const char* title)
{
    SDL_SetWindowTitle(mSDLWindow.get(), title);
}

const char* AppWindow::Impl::title() const
{
    return SDL_GetWindowTitle(mSDLWindow.get());
}

void AppWindow::Impl::resize(int w, int h)
{
    SDL_SetWindowSize(mSDLWindow.get(), w, h);
}

int AppWindow::Impl::width() const
{
    int x, y;
    SDL_GetWindowSize(mSDLWindow.get(), &x, &y);
    return x;
}

int AppWindow::Impl::height() const
{
    int x, y;
    SDL_GetWindowSize(mSDLWindow.get(), &x, &y);
    return y;
}
