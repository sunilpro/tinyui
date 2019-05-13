#ifndef POPUP_H
#define POPUP_H

#include "item.h"
#include "color.h"

class Popup: protected Item
{
public:
    Popup(Item *parent, float x, float y, float width, float hieght);
    virtual void draw(NVGcontext* ctx) override;
    bool isVisible() { return visible(); }
    void show(Item *anchorItem, Item *content, Color backgroundColor = Color::white);
    void hide();
    Item *anchorItem() { return mAnchorItem; }
private:
    Item *mAnchorItem = nullptr;
    Color mBackgroundColor = Color::white;
};

#endif // POPUP_H
