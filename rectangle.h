#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "item.h"
#include "color.h"

bool operator!=(const NVGcolor& lhs, const NVGcolor& rhs);

class Rectangle: public Item
{
    PROP(Rectangle, Color, color) = Color::white;

public:
    PROPERTY(float, radius);
    PROPERTY(int, borderWidth) = 0;
    PROPERTY(NVGcolor, borderColor) = nvgRGBA(0, 0, 0, 255);

public:
    Rectangle(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void draw(NVGcontext* ctx) override;

};


#endif // RECTANGLE_H
