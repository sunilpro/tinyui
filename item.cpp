#include "nanovg.h"
#include "item.h"
#include "anchors.h"
#include "screen.h"

extern Screen *gScreen;

Item::Item(Item *parent, float x, float y, float width, float hieght)
    : parent(nullptr), visible(true), x(x), y(y),
      width(width), hieght(hieght), mMouseFocus(false) {
    if (parent)
        parent->addChild(this);

}

Item::~Item() {
    for (auto child : mChildren) {
        if (child)
            child->decRef();
    }
}

void Item::addChild(int index, Item * widget) {
    assert(index <= childCount());
    mChildren.insert(mChildren.begin() + index, widget);
    widget->incRef();
    widget->parent = this;
}

void Item::addChild(Item * widget) {
    addChild(childCount(), widget);
}

void Item::removeChild(const Item *widget) {
    mChildren.erase(std::remove(mChildren.begin(), mChildren.end(), widget), mChildren.end());
    widget->decRef();
}

void Item::removeChild(int index) {
    Item *widget = mChildren[index];
    mChildren.erase(mChildren.begin() + index);
    widget->decRef();
}

int Item::childIndex(Item* widget) const {
    auto it = std::find(mChildren.begin(), mChildren.end(), widget);
    if (it == mChildren.end())
        return -1;
    return (int) (it - mChildren.begin());
}

Item *Item::findItem(float x, float y) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        Item *child = *it;
        if (child->visible() && child->contains(x - this->x(), y - this->y()))
            return child->findItem(x - this->x(), y - this->y());
    }
    return contains(x, y) ? this : nullptr;
}

void Item::requestFocus() {
    gScreen->updateFocus(this);
}

#define GLFW_MOUSE_BUTTON_1         0
bool Item::mouseButtonEvent(float mx, float my, int button, bool down, int modifiers) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        Item *child = *it;
        if (child->visible() && child->contains(mx - x(), my - y()) &&
                child->mouseButtonEvent(mx - x(), my - y(), button, down, modifiers))
            return true;
    }
    if (button == GLFW_MOUSE_BUTTON_1 && down && !focused())
        requestFocus();
    return false;
}

bool Item::mouseMotionEvent(float mx, float my, float rx, float ry, int button, int modifiers) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        Item *child = *it;
        if (!child->visible())
            continue;
        bool contained = child->contains(mx - x(), my - y()), prevContained = child->contains(mx - x() - rx, my - y() - ry);
        if (contained != prevContained)
            child->mouseEnterEvent(mx, my, contained);
        if ((contained || prevContained) &&
                child->mouseMotionEvent(mx - x(), my - y(), rx, ry, button, modifiers))
            return true;
    }
    return false;
}

bool Item::mouseDragEvent(float, float, float, float, int, int) {
    return false;
}

bool Item::mouseEnterEvent(float, float, bool enter) {
    mMouseFocus = enter;
    return false;
}

bool Item::focusEvent(bool focused) {
    this->focused = focused;
    return false;
}

bool Item::keyboardEvent(int, int, int, int) {
    return false;
}

bool Item::keyboardCharacterEvent(unsigned int) {
    return false;
}

bool Item::scrollEvent(float mx, float my, float rx, float ry) {
    for (auto it = mChildren.rbegin(); it != mChildren.rend(); ++it) {
        Item *child = *it;
        if (!child->visible())
            continue;
        if (child->contains(mx - x(), my - y()) && child->scrollEvent(mx - x(), my - y(), rx, ry))
            return true;
    }
    return false;
}

void Item::performLayout() {
    auto _anchors = anchors();
    if (_anchors)
        _anchors->performLayout();
}

void Item::draw(NVGcontext *ctx) {
#if 0
    nvgStrokeWidth(ctx, 1.0f);
    nvgBeginPath(ctx);
    nvgRect(ctx, x() - 0.5f, y() - 0.5f, width() + 1, hieght() + 1);
    nvgStrokeColor(ctx, nvgRGBA(255, 0, 0, 255));
    nvgStroke(ctx);
#endif

    performLayout();

    if (mChildren.empty())
        return;

    nvgSave(ctx);
    nvgTranslate(ctx, x(), y());
    for (auto child : mChildren) {
        if (child->visible()) {
            nvgSave(ctx);
            //nvgIntersectScissor(ctx, child->x(), child->y(), child->width(), child->hieght());
            child->draw(ctx);
            nvgRestore(ctx);
        }
    }
    nvgRestore(ctx);
}
