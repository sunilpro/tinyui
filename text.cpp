#include "text.h"

#include "nanovg.h"

Text::Text(Item *parent, float x, float y, float width, float hieght): Item (parent, x, y, width, hieght)
{

}

void Text::performLayout() {

}

void Text::draw(NVGcontext *ctx) {
    Item::draw(ctx);
    nvgFontFace(ctx, "sans");

    nvgFontSize(ctx, 18.0f);
    nvgTextAlign(ctx, NVG_ALIGN_LEFT|NVG_ALIGN_TOP);
    nvgFillColor(ctx, nvgRGBA(255, 255, 255, 255));
    nvgTextBox(ctx, x(), y(), width(), text().c_str(), nullptr);
}
