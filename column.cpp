#include "column.h"

Column::Column(Item *parent, float x, float y)
    : Item (parent, x, y)
{

}

void Column::performLayout() {
    float yOffset = padding();
    float maxWidth = 0;
    for (auto child : mChildren) {
        if (child->visible()) {
            child->x = padding();
            child->y = yOffset;
            yOffset += child->hieght() + spacing();
            if (child->width() > maxWidth)
                maxWidth = child->width();
        }
    }
    if(hieght() == 0.0f && yOffset > spacing()) {
        hieght = yOffset - spacing() + padding();
    }
    if(width() == 0.0f)
        width = maxWidth + 2*padding();

    Item::performLayout();
}
