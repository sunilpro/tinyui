#include "mousearea.h"

MouseArea::MouseArea(Item *parent, float x, float y, float w, float h):
    Item (parent, x, y, w, h) {
}

bool MouseArea::focusEvent(bool focused) {
    Item::focusEvent(focused);
    return mFocused ? mFocused(focused) : false;
}

bool MouseArea::mouseButtonEvent(float mx, float my, int button, bool down, int modifiers) {
    Item::mouseButtonEvent(mx, my, button, down, modifiers);
    return mClicked ? mClicked(down) : false;
}

bool MouseArea::mouseEnterEvent(float, float, bool enter) {
    return mHovered ? mHovered(enter) : false;
}
