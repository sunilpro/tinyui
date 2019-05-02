#ifndef TEXT_H
#define TEXT_H

#include "item.h"
#include "color.h"

class Text: public Item
{
public:
    std::string text;
    Color color;
    int fontSize;
public:
    Text(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void performLayout() override;
    virtual void draw(NVGcontext *) override;
private:
    bool needsUpdate = true;
};

#endif // TEXT_H
