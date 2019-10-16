#pragma once

#include "editor/IDragHandler.h"
#include "editor/paint/IPaintStep.h"

class IElementPainter : public IPaintStep, public IDragHandler {
};
