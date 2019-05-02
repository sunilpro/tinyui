#include "text.h"

#include "nanovg.h"
#include "screen.h"

extern Screen *gScreen;

Text::Text(Item *parent, float x, float y, float w, float h): Item (parent, x, y, w, h),
    color(Color::black()), fontSize(12.0f)
{

}

void Text::performLayout() {
    if(!needsUpdate)
        return;
    needsUpdate = false;

    auto ctx = gScreen->vg();
    nvgFontFace(ctx, "sans");
    nvgFontSize(ctx, fontSize);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(ctx, color.vgColor());

    float bounds[4];
    nvgTextBounds(ctx, x(), y(), text.c_str(), nullptr, bounds);
    width = bounds[2] - bounds[0] + 4;
    hieght = bounds[3] - bounds[1];
}

void Text::draw(NVGcontext *ctx) {
    Item::draw(ctx);
    nvgFontFace(ctx, "sans");
    nvgFontSize(ctx, fontSize);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(ctx, color.vgColor());

    nvgTextBox(ctx, x(), y(), width(), text.c_str(), nullptr);
}
