#include "row.h"

Row::Row(Item *parent, float x, float y, float w, float h): Item(parent, x, y, w, h) {

}

void Row::performLayout() {
    float xOffset = padding();
    float maxHieght = 0;
    for (auto child : mChildren) {
        if (child->visible()) {
            child->x = xOffset;
            child->y = padding();
            xOffset += child->width() + spacing();
            if (child->hieght() > maxHieght)
                maxHieght = child->hieght();
        }
    }
    if(width() == 0.0f && xOffset > spacing()) {
        width = xOffset - spacing() + padding();
    }
    if(hieght() == 0.0f)
        hieght = maxHieght + 2*padding();

    Item::performLayout();
}
