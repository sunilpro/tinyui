#ifndef MOUSEAREA_H
#define MOUSEAREA_H

#include "item.h"

class MouseArea: public Item
{
public:
    MouseArea(Item *parent, float x = 0, float y = 0, float w = 0, float h = 0);
    MouseArea* clicked(const  std::function<bool(bool)>& cb) { mClicked = cb; return this; }
    MouseArea* hovered(const  std::function<bool(bool)>& cb) { mHovered = cb; return this; }

protected:
    virtual bool mouseButtonEvent(float mx, float my, int button, bool down, int modifiers) override;
    virtual bool mouseEnterEvent(float mx, float my, bool enter) override;

private:
    std::function<bool(bool)> mClicked;
    std::function<bool(bool)> mHovered;
};

#endif // MOUSEAREA_H
