#ifndef TEXT_H
#define TEXT_H

#include "item.h"

class Text: public Item
{
public:
    PROPERTY(std::string, text);
public:
    Text(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void performLayout() override;
    virtual void draw(NVGcontext *) override;
};

#endif // TEXT_H
