#pragma once

#include "editor/paint/PaintHistory.h"
#include "window/AppWindow.h"
#include <memory>

class Control;
class ColorPicker;
class ToolPicker;
class SizePicker;
class IElementPainter;
class ClippingPainter;

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
    void setButtonsPosition();

private:
    enum EditorState {
        ES_CLIPPING_IDLE,
        ES_CLIPPING,
        ES_PAINTING_IDLE,
        ES_PAINTING
    };
    std::vector<std::shared_ptr<Control>> mControls;
    ColorPicker* mColorPicker;
    ToolPicker* mToolPicker;
    SizePicker* mSizePicker;
    std::shared_ptr<IElementPainter> mCurrentPainter { nullptr };
    PaintHistory mPaintHistory;
    std::shared_ptr<ClippingPainter> mClippingPainter;
    EditorState mEditorState { ES_CLIPPING_IDLE };
    bool mPaintControls { true };

    std::shared_ptr<BLImage> mImageToEdit;
    friend class App;
};