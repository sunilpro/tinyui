#include "rectangle.h"

bool operator!=(const NVGcolor& lhs, const NVGcolor& rhs) {
    return memcmp(lhs.rgba, rhs.rgba, sizeof(float) *4) != 0;
}

Rectangle::Rectangle(Item *parent, float x, float y, float width, float hieght)
    : Item (parent, x, y, width, hieght)
{

}

void Rectangle::draw(NVGcontext* vg) {
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x(), y(), width(), hieght(), m_radius);
    nvgFillColor(vg, m_color.vgColor());
    nvgFill(vg);
    if (borderWidth()) {
        nvgStrokeColor(vg, m_borderColor.vgColor());
        nvgStrokeWidth(vg, m_borderWidth);
        nvgStroke(vg);
    }

    Item::draw(vg);
}
