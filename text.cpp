#include "text.h"

#include "nanovg.h"
#include "screen.h"

extern Screen *gScreen;

Text::Text(Item *parent, float x, float y, float w, float h): Item (parent, x, y, w, h) {

}

void Text::performLayout() {
    if(!needsUpdate && width != 0)
        return;
    needsUpdate = false;

    auto ctx = gScreen->vg();
    nvgFontFace(ctx, m_fontName.c_str());
    nvgFontSize(ctx, m_fontSize);
    nvgTextAlign(ctx, m_alignment);
    nvgFillColor(ctx, m_color.vgColor());

    float bounds[4];
    nvgTextBounds(ctx, x(), y(), m_text.c_str(), nullptr, bounds);
    if (width == 0)
        width = bounds[2] - bounds[0] + 4;
    if (hieght == 0)
        hieght = bounds[3] - bounds[1];
}

void Text::draw(NVGcontext *ctx) {
    Item::draw(ctx);
    nvgFontFace(ctx, m_fontName.c_str());
    nvgFontSize(ctx, m_fontSize);
    nvgTextAlign(ctx, m_alignment);
    nvgFillColor(ctx, m_color.vgColor());
    auto _x = x();
    auto _y = y();
    if(m_alignment & NVG_ALIGN_CENTER)
        _x += width()/2;
    if(m_alignment & NVG_ALIGN_MIDDLE)
        _y += hieght()/2;

    nvgText(ctx, _x, _y, m_text.c_str(), nullptr);
}
