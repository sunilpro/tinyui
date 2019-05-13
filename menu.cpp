#include "menu.h"
#include "rectangle.h"

#define ENTRY_HEIGHT 36
#define DIVIDER_HEIGHT 24
#define PADDING 15

Menu::Menu(Item *parent): Item(parent, 0, 0, 200, 15) {

}

void Menu::addMenuItem(MenuItem item) {
    mMenuItems.push_back(item);
    hieght = hieght() + (item.title.length() > 0 ? ENTRY_HEIGHT : DIVIDER_HEIGHT);
}

bool Menu::focusEvent(bool) {
    return true;
}

bool Menu::mouseMotionEvent(float mx, float my, float, float, int, int) {
    mHoveredItem = nullptr;
    if(mx < PADDING || mx > width()-2*PADDING || my < PADDING)
        return true;
    float _y = y() + PADDING;
    for(const auto& entry: mMenuItems) {
        _y += entry.title.length() <= 0 ? DIVIDER_HEIGHT : ENTRY_HEIGHT;
        if (_y > my) {
            mHoveredItem = &entry;
            break;
        }
    }

    return true;
}

void Menu::draw(NVGcontext* ctx) {
    const float _x = PADDING;
    float _y = y() + PADDING;
    for(const auto& entry: mMenuItems) {
        if (entry.title.length() <= 0) {
            Rectangle::drawRect(ctx, 0, _y+4, width(), 1, 0, 0x00000011);
            _y += DIVIDER_HEIGHT;
            continue;
        }

        uint32_t textColor = 0x283744FF;
        if (mHoveredItem == &entry) {
            Rectangle::drawRect(ctx, 10, _y-5, width() - 20, 32, 5, 0x283744FF);
            textColor = 0xCCCCCCFF;
        }

        nvgFontFace(ctx, "fas");
        nvgFontSize(ctx, 12);
        nvgTextAlign(ctx, NVG_ALIGN_TOP | NVG_ALIGN_LEFT);
        nvgFillColor(ctx, Color(textColor).vgColor());
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

        _y += ENTRY_HEIGHT;
    }
    Item::draw(ctx);
}
