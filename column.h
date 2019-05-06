#ifndef COLUMN_H
#define COLUMN_H

#include "item.h"

class Column: public Item
{
public:
    PROP(Column, float, spacing) = 0;
    PROP(Column, float, padding) = 0;
public:
    Column(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void performLayout() override;
};

#endif // COLUMN_H
