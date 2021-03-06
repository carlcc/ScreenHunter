#pragma once
#include <memory>
#include <vector>

class Painter;
class PaintStep;

class PaintHistory {
public:
    PaintHistory();
    ~PaintHistory();

    bool undo();

    bool redo();

    void push(const std::shared_ptr<PaintStep>& step);

    void paint(Painter& painter);

    void clear()
    {
        mHistory.clear();
        mHistorySize = 0;
    }

    size_t totalSize() const
    {
        return mHistory.size();
    }

    size_t effectiveSize() const
    {
        return mHistorySize;
    }

private:
    std::vector<std::shared_ptr<PaintStep>> mHistory;
    size_t mHistorySize;
};
