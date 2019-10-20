#pragma once

#include "editor/paint/PaintHistory.h"
#include "window/AppWindow.h"
#include <memory>

class Control;
class ColorPicker;
class ToolPicker;
class SizePicker;
class IElementPainter;

class EditorWindow : public AppWindow {
public:
    EditorWindow();
    ~EditorWindow();

    void setImage(const std::shared_ptr<BLImage>& img);

protected:
    void paint() override;
    void onWindowEvent(const WindowEvent& we) override;
    void onMouseButtonEvent(const MouseButtonEvent& mbe) override;
    void onKeyboardEvent(const KeyboardEvent& ke) override;
    void onMouseMoveEvent(const MouseMoveEvent& mme) override;
    void onTextInputEvent(const TextInputEvent& tie) override;

private:
    std::vector<std::shared_ptr<Control>> mControls;
    ColorPicker* mColorPicker;
    ToolPicker* mToolPicker;
    SizePicker* mSizePicker;
    std::shared_ptr<IElementPainter> mCurrentPainter { nullptr };
    PaintHistory mPaintHistory;
    bool mIsPainting { false };
    bool mPaintControls { true };

    std::shared_ptr<BLImage> mImageToEdit;
    friend class App;
};