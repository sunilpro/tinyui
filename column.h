#ifndef COLUMN_H
#define COLUMN_H

#include "item.h"

class Column: public Item
{
public:
    PROPERTY(float, spacing) = 0;
    PROPERTY(float, padding) = 0;
public:
    Column(Item *parent, float x = 0, float y = 0);
    virtual void performLayout() override;
};

#endif // COLUMN_H
