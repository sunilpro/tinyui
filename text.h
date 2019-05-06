#ifndef TEXT_H
#define TEXT_H

#include "item.h"
#include "color.h"

class Text: public Item
{
    PROP(Text, std::string, text);
    PROP(Text, Color, color) = Color::black;
    PROP(Text, int, alignment) = NVG_ALIGN_LEFT|NVG_ALIGN_TOP;
    PROP(Text, std::string, fontName) = "sans";
    PROP(Text, int, fontSize) = 12.0f;

public:
    static Text* createIcon(float width, float hieght, const char* unicode, uint32_t color, int size) {
        auto icon = new Text(nullptr, 0, 0, width, hieght);
        icon->m_alignment = NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE;
        icon->m_fontName = "fas";
        icon->m_fontSize = size;
        icon->m_color = color;
        icon->m_text = unicode;
        return icon;
    }
    Text(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void performLayout() override;
    virtual void draw(NVGcontext *) override;

private:
    bool needsUpdate = true;
};

#endif // TEXT_H
