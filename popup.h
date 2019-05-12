#ifndef POPUP_H
#define POPUP_H

#include "item.h"

class Popup: protected Item
{
public:
    Popup(Item *parent, float x, float y, float width, float hieght);
    virtual void draw(NVGcontext* ctx) override;
    bool isVisible() { return visible(); }
    void show(Item *anchorItem, Item *content);
    void hide();
    Item *anchorItem() { return mAnchorItem; }
private:
    Item *mAnchorItem = nullptr;
};

#endif // POPUP_H
