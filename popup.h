#ifndef POPUP_H
#define POPUP_H

#include "item.h"

class Popup: public Item
{
public:
    Popup(Item *parent, float x, float y, float width, float hieght);
    virtual void draw(NVGcontext* ctx) override;

    void show(Item *anchorItem);
    Item *anchorItem() { return mAnchorItem; }
private:
    Item *mAnchorItem = nullptr;
};

#endif // POPUP_H
