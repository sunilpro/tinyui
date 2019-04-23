#ifndef FLICKABLE_H
#define FLICKABLE_H

#include "item.h"

class Flickable: public Item
{
public:
    Flickable(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual bool mouseDragEvent(float mx, float my, float rx, float ry, int button, int modifiers) override;
    virtual bool scrollEvent(float, float, float, float) override;
    virtual void draw(NVGcontext *ctx) override;

protected:
    virtual void onScrolled() { }
    float mChildPreferredHeight;
    float mScroll = 0;
};

#endif // FLICKABLE_H
