#include "anchors.h"
#include "item.h"

bool operator!=(const AnchorLine& lhs, const AnchorLine& rhs) {
    return lhs.anchor != rhs.anchor || lhs.type != rhs.type;
}

Anchors::Anchors(Item *parent): item(parent)
{
    const auto &targetChanges = [=](Item *_centerIn) {
        if(_centerIn) {
            _centerIn->x.on_change().connect([=](float){ updateHGeometry(); });
            _centerIn->y.on_change().connect([=](float){ updateVGeometry(); });
            _centerIn->width.on_change().connect([=](float){ updateHGeometry(); });
            _centerIn->hieght.on_change().connect([=](float){ updateVGeometry(); });
        }
    };
    centerIn.on_change().connect(targetChanges);
    fill.on_change().connect(targetChanges);
    verticalCenter.on_change().connect([=](const AnchorLine&){ updateVGeometry(); });
    horizontalCenter.on_change().connect([=](const AnchorLine&){ updateHGeometry(); });
}

void Anchors::updateHGeometry() {
    updateHLayout = true;
    //printf("updateHGeometry [%f, %f]!\n", item()->width(), item()->hieght());
    if (const auto &_centerIn = centerIn()) {
        if (centerIn == item()->parent) { // to parent
            //printf("to parent\n");
            item()->x = (_centerIn->width() - item()->width())/2;
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->x = _centerIn->x() + (_centerIn->width() - item()->width())/2;
        }
        return;
    } else if (const auto &_fill = fill()) {
        auto old_width = item()->width;
        if (fill == item()->parent) { // to parent
            //printf("to parent\n");
            item()->x = 0;
            item()->width = _fill->width();
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->x = _fill->x();
            item()->width = _fill->width();
        }
        updateHLayout = old_width == item()->width();
        return;
    }
    const auto &_leftAnchor = left().anchor;
    const auto &_rightAnchor = right().anchor;
    const auto &_horizontalCenter = horizontalCenter().anchor;
    if (_leftAnchor != this) { // bound
        //printf("updateHGeometry left bound [%f, %f] [%f, %f]!\n", item()->width(), item()->hieght(), item()->x(), item()->parent()->x());
        if (_leftAnchor->item == item()->parent) { // to parent
            //printf("to parent\n");
            item()->x = left().type == AnchorLine::right ? item()->parent()->width() : 0;
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->x = _leftAnchor->item()->x() + (left().type == AnchorLine::right ? _leftAnchor->item()->width() : 0);
        }
        if (_rightAnchor != this) { // bound
            //printf("updateHGeometry right bound [%f, %f]!\n", item()->x(), item()->parent()->x());
            auto old_width = item()->width;
            if (_rightAnchor->item == item()->parent) { // to parent
                //printf("to parent\n");
                item()->width = _rightAnchor->item()->width() - item()->x();
            } else { // to sibling
                //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
                item()->width = _rightAnchor->item()->x() + ( _rightAnchor->item()->width() - item()->x());
            }
            updateHLayout = old_width == item()->width();
        }
    } else if (_rightAnchor != this) { // bound
        //printf("updateHGeometry right-only bound [%f, %f]!\n", item()->x(), item()->parent()->x());
        if (_rightAnchor->item == item()->parent) { // to parent
            //printf("to parent\n");
            item()->x = (right().type == AnchorLine::right ? item()->parent()->width() : 0) - item()->width();
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->x = _rightAnchor->item()->x() - item()->width() + (right().type == AnchorLine::right ? _rightAnchor->item()->width() : 0);
        }
    } else if (_horizontalCenter != this) {  // bound
        //printf("updateHGeometry horizontalCenter bound [%f, %f]!\n", _horizontalCenter->parent()->width(), _horizontalCenter->parent()->hieght());
        if (_horizontalCenter->item == item()->parent) { // to parent
            //printf("to parent\n");
            item()->x = (item()->parent()->width() - item()->width())/2;
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _horizontalCenter->parent()->x(), _horizontalCenter->parent()->width());
            item()->x = _horizontalCenter->item()->x() + (_horizontalCenter->item()->width() - item()->width())/2;
        }
    }
}
void Anchors::updateVGeometry() {
    updateVLayout = true;
    //printf("updateVGeometry [%f, %f]!\n", item()->width(), item()->hieght());
    if (const auto &_centerIn = centerIn()) {
        if (centerIn == item()->parent) { // to parent
            //printf("to parent\n");
            item()->y = (_centerIn->hieght() - item()->hieght())/2;
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->x = _centerIn->x() + (_centerIn->width() - item()->width())/2;
            item()->y = _centerIn->y() + (_centerIn->hieght() - item()->hieght())/2;
        }
        return;
    } else if (const auto &_fill = fill()) {
        auto old_hieght = item()->hieght;
        if (fill == item()->parent) { // to parent
            //printf("to parent\n");
            item()->y = 0;
            item()->hieght = _fill->hieght();
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->y = _fill->y();
            item()->hieght = _fill->hieght();
        }
        updateVLayout = old_hieght == item()->hieght;
        return;
    }

    const auto &_topAnchor = top().anchor;
    const auto &_bottomAnchor = bottom().anchor;
    const auto &_verticalCenter = verticalCenter().anchor;
    if (_topAnchor != this) { // bound
        //printf("updateVGeometry left bound [%f, %f] [%f, %f]!\n", item()->width(), item()->hieght(), item()->y(), item()->y());
        if (_topAnchor->item == item()->parent) { // to parent
            //printf("to parent\n");
            item()->y = top().type == AnchorLine::bottom ? item()->parent()->hieght() : 0;
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->y = _topAnchor->item()->y() + (top().type == AnchorLine::bottom ? _topAnchor->item()->hieght() : 0);
        }
        if (_bottomAnchor != this) { // bound
            //printf("updateHGeometry right bound [%f, %f]!\n", x(), _anchors->parent()->x());
            auto old_hieght = item()->hieght;
            if (_bottomAnchor->item == item()->parent) { // to parent
                //printf("to parent\n");
                item()->hieght = _bottomAnchor->item()->hieght() - item()->y();
            } else { // to sibling
                //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
                item()->hieght = _bottomAnchor->item()->y() + (_bottomAnchor->item()->hieght() - item()->y());
            }

            updateVLayout = old_hieght == item()->hieght;
        }
    } else if (_bottomAnchor != this) { // bound
        //printf("updateVGeometry bottom-only bound [%f, %f]!\n", item()->x(), item()->x());
        if (_bottomAnchor->item == item()->parent) { // to parent
            //printf("to parent\n");
            item()->y = (bottom().type == AnchorLine::bottom ? item()->parent()->hieght() : 0) - item()->hieght();
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _leftAnchor->parent()->x(), _leftAnchor->parent()->width());
            item()->y = _bottomAnchor->item()->y() - item()->hieght() + (bottom().type == AnchorLine::bottom ? _bottomAnchor->item()->hieght() : 0);
        }
    } else if (_verticalCenter != this) {  // bound
        //printf("updateVGeometry _verticalCenter bound [%f, %f]!\n", _verticalCenter->parent()->width(), _verticalCenter->parent()->hieght());
        if (_verticalCenter->item == item()->parent) { // to parent
            //printf("to parent\n");
            item()->y = (item()->parent()->hieght() - item()->hieght())/2;
        } else { // to sibling
            //printf("to sibling [%f, %f]\n", _verticalCenter->parent()->x(), _horizontalCenter->parent()->width());
            item()->y = _verticalCenter->item()->y() + (_verticalCenter->item()->hieght() - item()->hieght())/2;
        }
    }
}

void Anchors::performLayout() {
    if (!updateHLayout)
        updateHGeometry();
    if (!updateVLayout)
        updateVGeometry();
}

void Anchors::bind(Property<AnchorLine> &anchorLine, Item* target, Property<AnchorLine> &targetAnchorLine) {
    anchorLine.connect_from(targetAnchorLine);
    auto isParent = item()->parent == target;
    switch(anchorLine().type) {
    case AnchorLine::left:
    case AnchorLine::right:
    case AnchorLine::horizontalCenter: {
        auto cb = [=](float) { horizontalCenter.touch(); };
        if(!isParent)
            target->x.on_change().connect(cb);
        target->width.on_change().connect(cb);
        break;
    }
    case AnchorLine::top:
    case AnchorLine::bottom:
    case AnchorLine::verticalCenter: {
        auto cb = [=](float) { verticalCenter.touch(); };
        if(!isParent)
            target->y.on_change().connect(cb);
        target->hieght.on_change().connect(cb);
        break;
    }
    }
}
