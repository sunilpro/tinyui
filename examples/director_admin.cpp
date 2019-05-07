#include "screen.h"
#include "rectangle.h"
#include "text.h"
#include "column.h"
#include "row.h"
#include "mousearea.h"
#include <GLFW/glfw3.h>
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
    mouseArea->hovered([=](bool entered) -> bool {
        static_cast<Text*>(txt)->set_color(entered ? 0xCCCCCCFF: 0x333333FF);
        bkg->set_color(entered ? 0x333333FF: 0x0);
        return true;
    });
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

        auto btnTasks = createButton(row, 40, 50, "\uf0ae");
        createBadge(btnTasks, 15, 15, "9", 0xF05050FF, 0xFAE6E6FF);

        createButton(row, 128, 50, "\uf007 Sunil Reddy\uf0d7")->clicked([=](bool down) -> bool {
            return true;
        });
    }

    fflush(stdout);
}

int main() {
    //std::srand(time(nullptr));
    return app_exec(setupUI);
}