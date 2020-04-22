#include "editor/paint/PaintHistory.h"
#include "editor/paint/PaintStep.h"

PaintHistory::PaintHistory()
    : mHistory()
    , mHistorySize(0)
{
}

PaintHistory::~PaintHistory() = default;

bool PaintHistory::undo()
{
    if (mHistorySize > 0) {
        mHistorySize--;
        return true;
    }
    return false;
}

bool PaintHistory::redo()
{
    if (mHistory.size() > mHistorySize) {
        mHistorySize++;
        return true;
    }
    return false;
}

void PaintHistory::push(const std::shared_ptr<PaintStep>& step)
{
    mHistorySize += 1;
    if (mHistory.size() >= mHistorySize) {
        if (mHistory.size() > mHistorySize) {
            mHistory.resize(mHistorySize);
        }
        mHistory[mHistorySize - 1] = step;
    } else {
        mHistory.push_back(step);
    }
}

void PaintHistory::paint(Painter& painter)
{
    for (size_t i = 0; i < mHistorySize; ++i) {
        mHistory[i]->paint(painter);
    }
}