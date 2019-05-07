#include "cute.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "rectangle.h"
#include "row.h"
#include "column.h"
#include "flickable.h"
#include "listview.h"
#include "text.h"
#include "screen.h"

class MyAdaptor: public ListAdaptor {
public:
    virtual int count() {
        return 100;
    }

    virtual float hieghtForitemAt(int) {
        return 60;
    }

    virtual ListItem *itemAt(int position) {
        auto listItem = new ListItem(position, 0, 0, 200, 60);
        auto rect = listItem->add<Rectangle>(0, 0, 200, 60);
        listItem->add<Text>(0, 0, 200, 60)->set_text(std::string("Item ") + std::to_string(position));
        if (position % 2 == 0)
            rect->set_color(Color(255-position%3, position*2, 255-position%4));
        else
            rect->set_color(Color(position%3, position*2, position%4));
        return listItem;
    }

    virtual ~MyAdaptor() {

    }
};

void setupUI(Screen *screen) {
    Item *root = screen;
    static Flickable *flickable = nullptr;
    {
        Column *content;
        {
            flickable = root->add<Flickable>(100, 100, 240, 200);
            flickable->anchors = new Anchors(flickable);
            //content->anchors()->fill.connect_from(redRect);
        }
        {
            content = flickable->add<Column>(0, 0);
            content->set_spacing(5);
            content->set_padding(5);
            content->anchors = new Anchors(content);
            //rect1->anchors()->bind(rect1->anchors()->bottom, redRect, redRect->anchors()->bottom);
            //rect1->anchors()->bind(rect1->anchors()->horizontalCenter, redRect, redRect->anchors()->horizontalCenter);

            content->add<Rectangle>(0, 0, 100, 100)->set_color(Color(255, 0, 0, 255));
            content->add<Rectangle>(0, 0, 200, 100)->set_color(Color(255, 255, 0, 255));
            content->add<Rectangle>(0, 0, 100, 200)->set_color(Color(255, 0, 155, 255));
            content->add<Rectangle>(0, 0, 100, 10)->set_color(Color(0, 0, 255, 255));
            content->add<Rectangle>(0, 0, 10, 100)->set_color(Color(0, 255, 0, 255));
        }
        {
            auto rect1 = root->add<Rectangle>(100, 600, 200, 100);
            rect1->set_radius(10);
            rect1->set_color(Color(255, 0, 255, 255));
            rect1->anchors = new Anchors(rect1);
            rect1->anchors()->bind(rect1->anchors()->left, flickable, flickable->anchors()->left);
            //rect1->anchors()->bind(rect1->anchors()->right, redRect, redRect->anchors()->right);
            //rect1->anchors()->bind(rect1->anchors()->horizontalCenter, redRect, redRect->anchors()->horizontalCenter);
            Item::addAnimation(.30, [=](double progress) {
                flickable->x = 600.0 * progress;
            });
        }

        {
            auto listView = root->add<ListView>(400, 100, 300, 400);
            listView->setAdaptor(new MyAdaptor());
        }
    }
}
