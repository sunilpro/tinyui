#include "flickable.h"
#include "nanovg.h"

Flickable::Flickable(Item *parent, float x, float y, float width, float hieght)
    : Item (parent, x, y, width, hieght) {

}

bool Flickable::mouseDragEvent(float mx, float my,  float rx, float ry,
                            int button, int modifiers) {
    if (!mChildren.empty() && mChildPreferredHeight > hieght()) {
        float scrollh = hieght() * std::min(1.0f, hieght() / mChildPreferredHeight);
        mScroll = std::max(0.0f, std::min(1.0f, mScroll + ry / (hieght() - 8 - scrollh)));
        onScrolled();
        return true;
    } else {
        return Item::mouseDragEvent(mx, my, rx, ry, button, modifiers);
    }
}

bool Flickable::scrollEvent(float mx, float my, float rx, float ry) {
    if (!mChildren.empty() && mChildPreferredHeight > hieght()) {
        float scrollAmount = ry * (hieght() / 20.0f);
        float scrollh = hieght() * std::min(1.0f, hieght() / mChildPreferredHeight);
        mScroll = std::max(0.0f, std::min(1.0f, mScroll - scrollAmount / (hieght() - 8 - scrollh)));
        onScrolled();
        return true;
    } else {
        return Item::scrollEvent(mx, my, rx, ry);
    }
}

void Flickable::draw(NVGcontext *ctx) {
    if (mChildren.empty())
        return;
    Item *child = mChildren[0];
    child->performLayout();
    child->y = -mScroll*(mChildPreferredHeight - hieght());
    mChildPreferredHeight = child->hieght();

    nvgSave(ctx);
    nvgTranslate(ctx, x(), y());
    nvgIntersectScissor(ctx, 0, 0, width(), hieght());
    if (child->visible())
        child->draw(ctx);
    nvgRestore(ctx);

    if (mChildPreferredHeight <= hieght() || !mMouseFocus)
        return;

    float scrollh = hieght() * std::min(1.0f, hieght() / mChildPreferredHeight);

    /*NVGpaint paint = nvgBoxGradient(
                ctx, x() + width() - 12 + 1, y() + 4 + 1, 8,
                hieght() - 8, 3, 4, nvgRGBA(0, 0, 0, 32), nvgRGBA(0, 0, 0, 92));
    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, x() + width() - 12, y() + 4, 8,
                   hieght() - 8, 3);
    nvgFillPaint(ctx, paint);
    nvgFill(ctx);*/

    auto paint = nvgBoxGradient(
                ctx, x() + width() - 12 - 1,
                y() + 4 + (hieght() - 8 - scrollh) * mScroll - 1, 8, scrollh,
                3, 4, nvgRGBA(220, 220, 220, 200), nvgRGBA(128, 128, 128, 200));

    nvgBeginPath(ctx);
    nvgRoundedRect(ctx, x() + width() - 12 + 1,
                   y() + 4 + 1 + (hieght() - 8 - scrollh) * mScroll, 8 - 2,
                   scrollh - 2, 2);
    nvgFillPaint(ctx, paint);
    nvgFill(ctx);
}
