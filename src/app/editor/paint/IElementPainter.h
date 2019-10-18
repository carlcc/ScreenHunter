#pragma once

#include "editor/IDragHandler.h"
#include "editor/paint/PaintStep.h"

class IElementPainter : public PaintStep, public IDragHandler {
};
