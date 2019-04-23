#include "rectangle.h"

bool operator!=(const NVGcolor& lhs, const NVGcolor& rhs) {
    return memcmp(lhs.rgba, rhs.rgba, sizeof(float) *4) != 0;
}

Rectangle::Rectangle(Item *parent, float x, float y, float width, float hieght)
    : Item (parent, x, y, width, hieght)
{

}

void Rectangle::draw(NVGcontext* vg) {

    //nvgTranslate(vg, m_parent->x(), m_parent->y());

    nvgBeginPath(vg);
    nvgRoundedRect(vg, x(), y(), width(), hieght(), radius());
    nvgFillColor(vg, color());
    nvgFill(vg);
    if (borderWidth()) {
        nvgStrokeColor(vg, borderColor());
        nvgStrokeWidth(vg, borderWidth());
        nvgStroke(vg);
    }

    Item::draw(vg);
}
