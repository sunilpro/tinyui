#ifndef ANCHORS_H
#define ANCHORS_H

#include "object.h"

class Item;
class Anchors;

struct AnchorLine {
    enum Anchor {
        horizontalCenter, verticalCenter, left, right, top, bottom
    };
    AnchorLine(Anchors *anchor, Anchor type):anchor(anchor), type(type) {}
    Anchors *anchor;
    Anchor type;
};

bool operator!=(const AnchorLine& lhs, const AnchorLine& rhs);

class Anchors: public Object
{
public:
    PROPERTY(Item*, item);
    PROPERTY(Item*, centerIn) = nullptr;
    PROPERTY(Item*, fill) = nullptr;
    PROPERTY(AnchorLine, horizontalCenter) = AnchorLine(this, AnchorLine::horizontalCenter);
    PROPERTY(AnchorLine, verticalCenter) = AnchorLine(this, AnchorLine::verticalCenter);
    PROPERTY(AnchorLine, left) = AnchorLine(this, AnchorLine::left);
    PROPERTY(AnchorLine, right) = AnchorLine(this, AnchorLine::right);
    PROPERTY(AnchorLine, top) = AnchorLine(this, AnchorLine::top);
    PROPERTY(AnchorLine, bottom) = AnchorLine(this, AnchorLine::bottom);
    PROPERTY(float, margins) = 0;
    PROPERTY(float, leftMargin) = 0;
    PROPERTY(float, rightMargin) = 0;
    PROPERTY(float, topMargin) = 0;
    PROPERTY(float, bottomMargin) = 0;
public:
    Anchors(Item *parent);
    void performLayout();
    void bind(Property<AnchorLine> &anchorLine, Item* target, Property<AnchorLine> &targetAnchorLine);

protected:
    void updateHGeometry();
    void updateVGeometry();
    bool updateHLayout = false;
    bool updateVLayout = false;
};

#endif // ANCHORS_H
