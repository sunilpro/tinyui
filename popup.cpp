#include "popup.h"
#include "color.h"
#include "nanovg.h"
#include "rectangle.h"

Popup::Popup(Item *parent, float x, float y, float width, float hieght):
    Item(parent, x, y, width, hieght)
{
    visible = false;
}

void Popup::show(Item *anchorItem, Item *content, Color backgroundColor) {
    while(childCount() > 0)
        removeChild(childAt(0));

    mBackgroundColor = backgroundColor;
    mAnchorItem = anchorItem;

    addChild(content);
    content->performLayout();
    width = content->width();
    hieght = content->hieght();
    float ax, ay;
    anchorItem->absolutePosition(&ax, &ay);
    x = ax - width() + mAnchorItem->width() - 2;
    y = ay + anchorItem->hieght();
    //content->hieght.on_change().connect([=](float h) { hieght = h;});

    parent()->bringChildToFront(this);
    visible = true;
}

void Popup::hide() {
    visible = false;
    //while(childCount() > 0)
    //    removeChild(childAt(0));
}

void Popup::draw(NVGcontext *vg) {
    const int radius = 4;

    Rectangle::drawRect(vg, x(), y(), width(), hieght(), radius, mBackgroundColor);

    Item::draw(vg);

    nvgSave(vg);

    auto shadowPaint = nvgBoxGradient(vg, x(),y()+2, width(),hieght(), radius*2, 10, nvgRGBA(0,0,0,25), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, x()-10,y()-10, width()+20,hieght()+30);
    nvgRoundedRect(vg, x(),y(), width(),hieght(), radius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    Rectangle::drawRect(vg, x(), y(), width(), hieght(), radius, 0x0);

    nvgStrokeColor(vg, Color(0xDFDFDFFF).vgColor());
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);

    nvgBeginPath(vg);
    float baseX = x() + width() - 30;
    nvgMoveTo(vg, baseX, y()+1);
    nvgLineTo(vg, baseX + 10, y() - 11);
    nvgLineTo(vg, baseX + 20, y()+1);
    nvgFillColor(vg, mBackgroundColor.vgColor());
    nvgFill(vg);

    nvgRestore(vg);

}
