#pragma once

#include "AppWindow.h"
#include <SDL2/SDL.h>
#include <blend2d.h>
#include <memory>

class AppWindow::Impl {
public:
    Impl();
    ~Impl();

    bool init(const char* title, int w, int h, AppWindow* _this);

    void onPaint();

    void setTitle(const char* title);

    const char* title() const;

    void resize(int w, int h);

    int width() const;

    int height() const;

    std::shared_ptr<SDL_Window> mSDLWindow;
    std::shared_ptr<SDL_Renderer> mRenderer;
    std::shared_ptr<SDL_Texture> mTexture;

    std::shared_ptr<BLImage> mTextureBuf;
    int mWidth;
    int mHeight;

    bool mNeedRepaint { true };
};