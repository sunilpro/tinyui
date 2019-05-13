#include "screen.h"
#include "rectangle.h"
#include "text.h"
#include "column.h"
#include "row.h"
#include "mousearea.h"
#include "popup.h"
#include "menu.h"
#include <GLFW/glfw3.h>
extern Screen *gScreen;
extern int randInRange(int min, int max);

Rectangle* createBadge(Item *container, float width, float hieght, const char *text, Color bkgColor, Color txtColor) {
    auto badge = container->add<Rectangle>(container->width()/2, container->hieght()/2 - 20, width, hieght)
            ->set_radius(7)
            ->set_color(bkgColor);
    badge->add<Text>(0, 0, width, hieght)
            ->set_alignment(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE)
            ->set_text(text)
            ->set_color(txtColor)
            ->set_fontSize(14);
    return badge;
}

MouseArea* createButton(Item *container, float width, float hieght, const char *text) {
    auto mouseArea = container->add<MouseArea>(0, 0, width, hieght);
    auto bkg = mouseArea->add<Rectangle>(0, 0, width, hieght);
    auto txt = bkg->addChild(Text::createIcon(width, hieght, text, 0x333333FF, 14));
    auto h = [=](bool entered) -> bool {
        if(mouseArea->focus()) entered = true;
        static_cast<Text*>(txt)->set_color(entered ? 0xCCCCCCFF: 0x333333FF);
        bkg->set_color(entered ? 0x333333FF: 0x0);
        gScreen->setCursor(entered ? Cursor::Hand : Cursor::Arrow);
        //printf("MouseArea=%p %d\n", mouseArea, mouseArea->cursor());
        return true;
    };
    mouseArea->hovered(h);
    mouseArea->focused(h);
    return mouseArea;
}

static void setupUI(Screen *screen) {
    screen->setBackground(0xF1F2F7FF);
    auto titleBar = screen->add<Rectangle>(0, 0, screen->width(), 50);
    titleBar->anchors = new Anchors(titleBar);
    {
        auto row = titleBar->add<Row>(0, 0, 0, 50);
        row->add<Rectangle>(0, 0, 150, 50)
                ->set_color(0x283744FF)
                ->add<Text>(0, 0, 150, 50)
                ->set_alignment(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE)
                ->set_fontSize(24)
                ->set_color(Color::white)
                ->set_text("Admin");
        row->addChild(Text::createIcon(40, 50, "\uf0c9", 0x333333FF, 24));
    }

    {
        auto row = titleBar->add<Row>(0, 0, 0, 50);
        row->anchors = new Anchors(row);
        row->anchors()->bind(row->anchors()->right, titleBar, titleBar->anchors()->right);

        auto btnMail = createButton(row, 40, 50, "\uf0e0");
        createBadge(btnMail, 15, 15, "4", 0x27C247FF, 0xDFF5E4FF);
        btnMail->clicked([=](bool down) -> bool {
            if (!down) return false;
            gScreen->popup()->show(btnMail, new Rectangle(nullptr, 0, 0, 200, 100));
            return true;
        });

        auto btnTasks = createButton(row, 40, 50, "\uf0ae");
        createBadge(btnTasks, 15, 15, "9", 0xF05050FF, 0xFAE6E6FF);
        btnTasks->clicked([=](bool down) -> bool {
            if (!down) return false;
            gScreen->popup()->show(btnTasks, new Rectangle(nullptr, 0, 0, 200, 100));
            return true;
        });

        auto btnUser = createButton(row, 128, 50, "\uf007 Sunil Reddy\uf0d7");
        btnUser->clicked([=](bool down) -> bool {
            if (!down) return false;
            auto menu = new Menu(nullptr);
            menu->addMenuItem(Menu::MenuItem("Updates", "\uf1da", "10", 0x27C24CFF));
            menu->addMenuItem(Menu::MenuItem("Messages", "\uf0e0", "5", 0xF05050FF));
            menu->addMenuItem(Menu::MenuItem("Subscriptions", "\uf076", "3", 0x23B7E5FF));
            menu->addMenuItem(Menu::MenuItem("FAQ", "\uf128", "11", 0x777777FF));
            menu->addMenuItem(Menu::MenuItem());
            menu->addMenuItem(Menu::MenuItem("Profile", "\uf007"));
            menu->addMenuItem(Menu::MenuItem("Settings", "\uf013"));
            menu->addMenuItem(Menu::MenuItem());
            menu->addMenuItem(Menu::MenuItem("Logout", "\uf05e"));
            gScreen->popup()->show(btnUser, menu);
            return true;
        });
    }

    fflush(stdout);
}

int main() {
    //std::srand(time(nullptr));
    return app_exec(setupUI);
}
