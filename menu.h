#ifndef MENU_H
#define MENU_H

#include "item.h"
#include "color.h"

class Menu: public Item
{
public:
    struct MenuItem {
        MenuItem(const char *title = "", const char* icon = "", const char* badge = "", Color badgeColor = 0xFF):
            title(title), icon(icon), badge(badge), badgeColor(badgeColor), hovered(false) {
        }
        std::string title;
        std::string icon;
        std::string badge;
        Color       badgeColor;
        bool        hovered;
    };
public:
    Menu(Item *parent);
    void addMenuItem(MenuItem item);

protected:
    virtual void draw(NVGcontext* ctx) override;
    virtual bool focusEvent(bool focused) override;
    virtual bool mouseEnterEvent(float mx, float my, bool enter) override;

private:
    std::vector<MenuItem> mMenuItems;
};

#endif // MENU_H
