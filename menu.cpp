#include "menu.h"
#include "rectangle.h"

Menu::Menu(Item *parent): Item(parent, 0, 0, 200, 15) {

}

void Menu::addMenuItem(MenuItem item) {
    mMenuItems.push_back(item);
    hieght = hieght() + (item.title.length() > 0 ? 36 : 28);
}

bool Menu::focusEvent(bool focused) {
    return true;
}

bool Menu::mouseEnterEvent(float mx, float my, bool enter) {
    return true;
}

void Menu::draw(NVGcontext* ctx) {
    const float _x = 15;
    float _y = y() + 15;
    for(const auto& entry: mMenuItems) {
        if (entry.title.length() <= 0) {
            Rectangle::drawRect(ctx, 0, _y+6, width(), 1, 0, 0x0000001F);
            _y += 28;
            continue;
        }
        nvgFontFace(ctx, "fas");
        nvgFontSize(ctx, 12);
        nvgTextAlign(ctx, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);
        nvgFillColor(ctx, Color(0xFF).vgColor());
        nvgText(ctx, _x, _y+5, entry.title.c_str(), nullptr);

        nvgText(ctx, width() - 28, _y + 5, entry.icon.c_str(), nullptr);

        // Badge
        if(entry.badge.length() > 0) {
            const float b_w = entry.badge.size()*10;
            const float b_x = width() - 28 - 18 - b_w;
            Rectangle::drawRect(ctx, b_x, _y, b_w + 10, 20, 10, entry.badgeColor);

            //nvgFontFace(ctx, "sans-bold");
            //nvgFontSize(ctx, 16);
            //nvgTextAlign(ctx, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);
            nvgFillColor(ctx, Color(0xFFFFFFCC).vgColor());
            nvgText(ctx, b_x + 7, _y + 5, entry.badge.c_str(), nullptr);
        }

        _y += 36;
    }
    Item::draw(ctx);
}
