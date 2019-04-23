#include "listview.h"

ListView::ListView(Item *parent, float x, float y, float width, float hieght): Flickable (parent, x, y, width, hieght)
{
    mContentItem = add<Item>();
}

void ListView::setAdaptor(ListAdaptor *a) {
    mAdaptor = a;
    for (int i=0; i<a->count(); ++i) {
        auto item = a->itemAt(i);
        mContentItem->addChild(item);
        performLayout();

        printf("%d %f %f\n", item->position(), mContentItem->hieght(), hieght());

        if (mContentItem->children().back()->contains(0, hieght())) {
            //mContentItem->hieght = a->count() * (mContentItem->hieght()/i);
            break;
        }
    }
}

void ListView::onScrolled() {
    float currentY = -mContentItem->y();

    std::vector<Item *> toBeRemoved;
    for (auto child : mContentItem->children()) {
        if(!child->contains(0, currentY) && child->y() < currentY-100) {
            toBeRemoved.push_back(child);
        } else if(!child->contains(0, currentY + hieght() + 100) && child->y() > (currentY + hieght()+100)) {
            toBeRemoved.push_back(child);
        }
    }

    //printf("-> ListView::scrollEvent %f %d\n", currentY, toBeRemoved.size());

    for (auto child : toBeRemoved) {
        mContentItem->removeChild(child);
    }

    if(mContentItem->childCount() == 0) {
        int pos = int(currentY + hieght()/2.0f)/mAdaptor->hieghtForitemAt(0);
        auto item = mAdaptor->itemAt(pos);
        item->y = pos*60;
        mContentItem->addChild(0, item);
    }

    while(!mContentItem->children().front()->contains(0, currentY-100) && mContentItem->children().front()->y() > currentY-100) {
        auto first = static_cast<ListItem*>(mContentItem->children().front());
        auto item = mAdaptor->itemAt(first->position() - 1);
        item->y = first->y() - item->hieght() - spacing();
        mContentItem->addChild(0, item);
    }

    while(!mContentItem->children().back()->contains(0, currentY + hieght() + 100) && mContentItem->children().back()->y() < (currentY + hieght()+100)) {
        auto last = static_cast<ListItem*>(mContentItem->children().back());
        auto item = mAdaptor->itemAt(last->position() + 1);
        mContentItem->addChild(item);
        item->y = last->y() + last->hieght() + spacing();
    }
}

void ListView::performLayout() {
    float yOffset = mContentItem->children().front()->y() + padding();
    float maxWidth = 0;
    for (auto child : mContentItem->children()) {
        if (child->visible()) {
            child->x = padding();
            child->y = yOffset;
            yOffset += child->hieght() + spacing();
            if (child->width() > maxWidth)
                maxWidth = child->width();
        }
    }
    mContentItem->hieght = std::max(yOffset - spacing() + padding(), mAdaptor->count()*mAdaptor->hieghtForitemAt(0));
    mContentItem->width = maxWidth + 2*padding();
}

void ListView::draw(NVGcontext *ctx) {
    Flickable::draw(ctx);
}
