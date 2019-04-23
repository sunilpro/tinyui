#ifndef LISTVIEW_H
#define LISTVIEW_H

#include "flickable.h"

class ListItem: public Item {
public:
    PROPERTY(int, position);
    ListItem(int position, float x = 0, float y = 0, float width = 0, float hieght = 0):
        Item(nullptr, x, y, width, hieght), position(position) {

    }
};

class ListAdaptor {
public:
    virtual int count() = 0;
    virtual ListItem *itemAt(int position) = 0;
    virtual float hieghtForitemAt(int position) = 0;
    virtual ~ListAdaptor() { }
};

class ListView: public Flickable
{

public:
    PROPERTY(float, spacing) = 0;
    PROPERTY(float, padding) = 0;

    ListAdaptor *adaptor() { return  mAdaptor; }
    void setAdaptor(ListAdaptor *);

    ListView(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);
    virtual void performLayout() override;
    virtual void draw(NVGcontext*) override;

protected:
    virtual void onScrolled() override;

private:
    ListAdaptor *mAdaptor = nullptr;
    Item *mContentItem;
};

#endif // LISTVIEW_H
