#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "item.h"
#include "color.h"

bool operator!=(const NVGcolor& lhs, const NVGcolor& rhs);

class Rectangle: public Item
{
    PROP(Rectangle, Color, color) = Color::white;
    PROP(Rectangle, Color, borderColor) = Color::black;
    PROP(Rectangle, float, radius) = 0;
    PROP(Rectangle, int, borderWidth) = 0;

public:
    Rectangle(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void draw(NVGcontext* ctx) override;

    static void drawRect(NVGcontext* ctx, float x, float y, float width, float hieght, float radius, Color);
};


#endif // RECTANGLE_H
