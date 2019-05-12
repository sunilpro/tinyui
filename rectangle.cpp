#include "rectangle.h"

bool operator!=(const NVGcolor& lhs, const NVGcolor& rhs) {
    return memcmp(lhs.rgba, rhs.rgba, sizeof(float) *4) != 0;
}

Rectangle::Rectangle(Item *parent, float x, float y, float width, float hieght)
    : Item (parent, x, y, width, hieght)
{

}

void Rectangle::drawRect(NVGcontext* ctx, float x, float y, float width, float hieght, float radius, Color color) {
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, x, y, width, hieght, radius);
    nvgFillColor(ctx, color.vgColor());
    nvgFill(ctx);
}

void Rectangle::draw(NVGcontext* vg) {
    drawRect(vg, x(), y(), width(), hieght(), m_radius, m_color);
    if (borderWidth()) {
        nvgStrokeColor(vg, m_borderColor.vgColor());
        nvgStrokeWidth(vg, m_borderWidth);
        nvgStroke(vg);
    }

    Item::draw(vg);
}
