#include "popup.h"
#include "color.h"
#include "nanovg.h"
#include "rectangle.h"

Popup::Popup(Item *parent, float x, float y, float width, float hieght):
    Item(parent, x, y, width, hieght)
{
    visible = false;
}

void Popup::show(Item *anchorItem, Item *content) {
    while(childCount() > 0)
        removeChild(childAt(0));

    mAnchorItem = anchorItem;
    float ax, ay;
    anchorItem->absolutePosition(&ax, &ay);
    x = ax-200+mAnchorItem->width();
    y = ay+anchorItem->hieght();
    parent()->bringChildToFront(this);

    addChild(content);
    hieght = content->hieght();
    //content->hieght.on_change().connect([=](float h) { hieght = h;});

    visible = true;
}

void Popup::hide() {
    printf("-> Popup::hide\n");
    visible = false;
    while(childCount() > 0)
        removeChild(childAt(0));
}

void Popup::draw(NVGcontext *vg) {
    const int radius = 4;
    const Color color(Color::white);

    nvgSave(vg);

    auto shadowPaint = nvgBoxGradient(vg, x(),y()+2, width(),hieght(), radius*2, 10, nvgRGBA(0,0,0,25), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, x()-10,y()-10, width()+20,hieght()+30);
    nvgRoundedRect(vg, x(),y(), width(),hieght(), radius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    Rectangle::drawRect(vg, x(), y(), width(), hieght(), radius, color);

    nvgStrokeColor(vg, Color(0xDFDFDFFF).vgColor());
    nvgStrokeWidth(vg, 1);
    nvgStroke(vg);

    nvgBeginPath(vg);
    float baseX = x() + width() - 30;
    nvgMoveTo(vg, baseX, y()+1);
    nvgLineTo(vg, baseX + 10, y() - 11);
    nvgLineTo(vg, baseX + 20, y()+1);
    nvgFill(vg);

    nvgRestore(vg);

    Item::draw(vg);
}
