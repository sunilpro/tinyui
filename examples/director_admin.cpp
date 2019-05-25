#include "screen.h"
#include "rectangle.h"
#include "text.h"
#include "column.h"
#include "row.h"
#include "mousearea.h"
#include "popup.h"
#include "menu.h"
#include "listview.h"
#include <GLFW/glfw3.h>

#include <any>

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
        //printf("MouseArea=%p %d\n", mouseArea, entered);
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
            auto listView = new ListView(nullptr, 0, 0, 260, 240);
            class MyAdaptor: public ListAdaptor {
                const std::vector<std::tuple<std::string, std::string, std::string>> data = {
                    {"Support Team", "Why not a buy a new awesome theme", "5 mins"},
                    {"Director Design Team", "Why not a buy a new awesome theme", "2 hours"},
                    {"Developers", "Why not a buy a new awesome theme", "Today"},
                    {"Sales Department", "Why not a buy a new awesome theme", "Yesterday"},
                    {"Reviewers", "Why not a buy a new awesome theme", "2 days"}
                };
            public:
                virtual int count() {
                    return data.size();
                }

                virtual float hieghtForitemAt(int) {
                    return 68;
                }

                virtual ListItem *itemAt(int position) {
                    auto [title, subtitle, time] = data.at(position);
                    auto listItem = new ListItem(position, 0, 0, 260, 60);
                    listItem->add<Text>(10, 10, 140, 20)->set_text(title)->set_fontSize(18)->set_color(0x444444FF);
                    listItem->add<Text>(10, 28, 140, 20)->set_text(subtitle)->set_fontSize(16)->set_color(0x888888FF);
                    listItem->add<Text>(200 - 20, 45, 30, 20)->set_text(time)->set_fontSize(16)->set_color(0x333333FF);

                    listItem->add<Rectangle>(0, 67, 260, 1)->set_color(0x00000011);
                    return listItem;
                }

                virtual ~MyAdaptor() {

                }
            };
            auto adapter = new MyAdaptor();
            listView->setAdaptor(adapter);

            auto col = new Column(nullptr, 0, 0, 260, 0);
            col->add<Text>(10, 0, 220, 32)
                        ->set_alignment(NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE)
                        ->set_text("You have 4 messages")
                        ->set_color(0x444444FF)
                        ->set_fontSize(18);
            col->add<Rectangle>(0, 0, 260, 1)->set_color(0x00000011);
            col->addChild(listView);
            col->add<Rectangle>(0, 0, 260, 30)->set_color(0xF4F4F4FF)
               ->add<Text>(0, 0, 260, 32)
                    ->set_alignment(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE)
                    ->set_text("See All Messages")
                    ->set_color(0x444444FF)
                    ->set_fontSize(16);
            gScreen->popup()->show(btnMail, col);
            return true;
        });

        auto btnTasks = createButton(row, 40, 50, "\uf0ae");
        createBadge(btnTasks, 15, 15, "9", 0xF05050FF, 0xFAE6E6FF);
        btnTasks->clicked([=](bool down) -> bool {
            if (!down) return false;
            auto listView = new ListView(nullptr, 0, 0, 200, 240);
            class MyAdaptor: public ListAdaptor {
                const std::vector<std::tuple<std::string, float, Color>> data = {
                    {"Design some buttons", 0.2f, 0x27C24CFF},
                    {"Create a nice theme", 0.4f, 0xF05050FF},
                    {"Some task I need to do", 0.6f, 0x23B7E5FF},
                    {"Design some buttons", 0.8f, 0xFAD733FF}
                };
            public:
                virtual int count() {
                    return data.size();
                }

                virtual float hieghtForitemAt(int) {
                    return 60;
                }

                virtual ListItem *itemAt(int position) {
                    auto [title, progress, color] = data.at(position);
                    auto listItem = new ListItem(position, 0, 0, 200, 60);
                    listItem->add<Text>(10, 10, 140, 20)->set_text(title)->set_fontSize(18)->set_color(0x666666FF);

                    char progress_str[20];
                    sprintf(progress_str, "%d%%", (int)(progress*100));
                    std::string p = progress_str;
                    listItem->add<Text>(200 - 30, 14, 30, 20)->set_text(p)->set_fontSize(14)->set_color(0xAAAAAAFF);

                    auto progressBar = listItem->add<Rectangle>(10, 36, 180, 8)->set_color(0xCCCCCCAA)->set_radius(4);
                    progressBar->add<Rectangle>(0, 0, 180*progress, 8)->set_color(color)->set_radius(4);

                    listItem->add<Rectangle>(0, 59, 200, 1)->set_color(0x00000011);
                    return listItem;
                }

                virtual ~MyAdaptor() {

                }
            };
            auto adapter = new MyAdaptor();
            listView->setAdaptor(adapter);

            auto col = new Column(nullptr, 0, 0, 200, 0);
            col->add<Text>(10, 0, 180, 32)
                        ->set_alignment(NVG_ALIGN_LEFT | NVG_ALIGN_MIDDLE)
                        ->set_text("You have 9 tasks")
                        ->set_color(0x444444FF)
                        ->set_fontSize(18);
            col->add<Rectangle>(0, 0, 200, 1)->set_color(0x00000011);
            col->addChild(listView);
            col->add<Rectangle>(0, 0, 200, 30)->set_color(0xF4F4F4FF)
               ->add<Text>(0, 0, 200, 32)
                    ->set_alignment(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE)
                    ->set_text("View All Tasks")
                    ->set_color(0x444444FF)
                    ->set_fontSize(16);
            gScreen->popup()->show(btnTasks, col);
            return true;
        });

        auto btnUser = createButton(row, 128, 50, "\uf007 Sunil Reddy\uf0d7");
        btnUser->clicked([=](bool down) -> bool {
            if (!down) return false;
            auto menu = new Menu("Account");
            menu->addMenuItem(Menu::MenuItem("Updates", "\uf1da", "10", 0x27C24CFF));
            menu->addMenuItem(Menu::MenuItem("Messages", "\uf0e0", "5", 0xF05050FF));
            menu->addMenuItem(Menu::MenuItem("Subscriptions", "\uf076", "3", 0x23B7E5FF));
            menu->addMenuItem(Menu::MenuItem("FAQ", "\uf128", "11", 0x777777FF));
            menu->addMenuItem(Menu::MenuItem());
            menu->addMenuItem(Menu::MenuItem("Profile", "\uf007"));
            menu->addMenuItem(Menu::MenuItem("Settings", "\uf013"));
            menu->addMenuItem(Menu::MenuItem());
            menu->addMenuItem(Menu::MenuItem("Logout", "\uf05e"));
            gScreen->popup()->show(btnUser, menu, 0xF9FAFCFF);
            return true;
        });
    }

    fflush(stdout);
}

int main() {
    //std::srand(time(nullptr));
    return app_exec(setupUI);
}
