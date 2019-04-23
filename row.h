#ifndef ROW_H
#define ROW_H

#include "item.h"
class Row: public Item
{
public:
    PROPERTY(float, spacing) = 0;
    PROPERTY(float, padding) = 0;
public:
    Row(Item *parent, float x = 0, float y = 0);
    virtual void performLayout() override;
};

#endif // ROW_H
