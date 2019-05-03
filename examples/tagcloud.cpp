#include "screen.h"
#include "rectangle.h"
#include "text.h"
#include <GLFW/glfw3.h>
extern int randInRange(int min, int max);

static const char *_tags[] = {
    "The Lost",
    "Rick & Morty",
    "West World",
    "True Detective",
    "Breaking Bad",
    "Game of Thrones",
    "Billions",
    "The Walking Dead",
    "Silicon Valley",
    "Preacher",
    "Family Guy",
    "Mr Robot",
    "Dexter",
    "Tom & Jerry",
    "Bugs Bunny",
    "Avengers"
};
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>

template <typename T>
void remove_duplicates(std::vector<T>& vec)
{
    std::sort(vec.begin(), vec.end());
    vec.erase(std::unique(vec.begin(), vec.end()), vec.end());
}

/**
 * Check if two rectangles collide
 * x_1, y_1, width_1, and height_1 define the boundaries of the first rectangle
 * x_2, y_2, width_2, and height_2 define the boundaries of the second rectangle
 */
bool rectangle_collision(float x_1, float y_1, float width_1, float height_1, float x_2, float y_2, float width_2, float height_2)
{
    return !(x_1 > x_2+width_2 || x_1+width_1 < x_2 || y_1 > y_2+height_2 || y_1+height_1 < y_2);
}

enum Quadrant {
    TopLeft,
    TopRight,
    BottomLeft,
    BottomRight
};

bool findFreeRectangle(Quadrant quadrant, Item *container, float width, float height, float &posx, float &posy) {
    int xDirection, yDirection;
    switch (quadrant) {
    case TopLeft: xDirection = -1; yDirection = -1; break;
    case TopRight: xDirection = 1; yDirection = -1; break;
    case BottomLeft: xDirection = -1; yDirection = 1; break;
    case BottomRight: xDirection = 1; yDirection = 1; break;
    }
    float containerWidth = container->width();
    float containerHieght = container->hieght();
    const std::vector<Item*> &children = container->children();
    for (float y=containerHieght/2 - (yDirection < 0 ? height : 0); y > 0 && y < containerHieght-height; y+=yDirection) {
        for (float x=containerWidth/2 - (xDirection < 0 ? width : 0); x > 0 && x < containerWidth-width; x+=xDirection) {
            bool collided = false;
            for (auto child : children) {
                if(rectangle_collision(child->x(), child->y(), child->width(), child->hieght(), x, y, width, height)) {
                    collided = true;
                    if (xDirection < 0 && x > child->x())
                        x = child->x();
                    else if(xDirection < 0 && x+width > child->x())
                        x = child->x() - width;
                    else if (xDirection > 0 && x < child->x() + child->width())
                        x = child->x() + child->width();
                    //printf("-> Collision (%f,%f,%f,%f) (%f,%f,%f,%f)\n", child->x(), child->y(), child->width(), child->hieght(), x, y, width, height);
                    break;
                }
            }
            if (!collided) {
                posx = x;
                posy = y;
                return true;
            }
        }
    }
    return false;
}

const static std::string sentence = "Another possibility would be to merge a word’s tree with a single large tree once it is placed I think this operation would be fairly expensive though compared with the analagous sprite mask operation, which is essentially ORing a whole block as word placement can be quite slow for more than a few hundred words the layout algorithm can be run asynchronously with a configurable time step size This makes it possible to animate words as they are placed without stuttering It is recommended to always use a time step even without animations as it prevents the browser’s event loop from blocking while placing the words";
void setupTagcloud(Screen *screen) {
    std::srand(time(nullptr));

    std::istringstream iss(sentence);
    std::vector<std::string> tokens{std::istream_iterator<std::string>{iss}, std::istream_iterator<std::string>{}};

    remove_duplicates(tokens);

    std::random_shuffle(std::begin(tokens), std::end(tokens));

    auto root = new Item(nullptr, 0, 0, 800, 600);
    //root->add<Rectangle>( 200, 200, 50, 50)->color = nvgRGB(255, 0, 0);
    double started = glfwGetTime();
    for (size_t i=0; i < tokens.size(); ++i) {
        auto txt = new Text(nullptr, 400, 400);
        txt->fontSize = randInRange(20, 80);
        txt->color.r = rand() %255;
        txt->color.g = rand() %255;
        txt->color.b = rand() %255;
        txt->text = tokens[i];
        txt->performLayout();
        float x,y;
        //double start = glfwGetTime();
        if(findFreeRectangle((Quadrant)(i%4), root, txt->width(), txt->hieght(), x, y)) {
            //printf("[%lf]-> Free Rectangle (%f,%f,%f,%f)\n", glfwGetTime() - start, x, y, txt->width(), txt->hieght());
            txt->x = x;
            txt->y = y;
            root->addChild(txt);
        } else {
            printf("-> No Free Rectangle (%f, %f) %s %d\n", txt->width(), txt->hieght(), tokens[i].c_str(), txt->fontSize);
        }
    }

    auto _root = screen->add<Item>(200, 100, 800, 600);
    for (auto child: root->children()) {
        auto _x = child->x();
        auto _y = child->y();
        auto x = randInRange(0, 800);
        auto y = randInRange(0, 600);
        Item::addAnimation(1.0, [=](double progress) {
            child->x = _x * progress + x*(1-progress);
            child->y = _y * progress + y*(1-progress);
        });
        _root->addChild(child);
    }

    while(root->childCount() > 0)
        root->removeChild(root->childAt(0));

    printf("Took %lf seconds for placing %d words\n", glfwGetTime() - started, root->childCount());

    fflush(stdout);
}

int main() {
    return app_exec(setupTagcloud);
}
