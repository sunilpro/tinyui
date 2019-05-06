#include "screen.h"
#include "rectangle.h"
#include "text.h"
#include "column.h"
#include "row.h"
#include <GLFW/glfw3.h>
extern int randInRange(int min, int max);

static void setupUI(Screen *screen) {
    screen->setBackground(0xF1F2F7FF);
    auto titleBar = screen->add<Rectangle>(0, 0, screen->width(), 40);
    titleBar->anchors = new Anchors(titleBar);
    {
        auto row = titleBar->add<Row>(0, 0, 0, 40);
        row->add<Rectangle>(0, 0, 150, 40)
                ->set_color(0x283744FF)
                ->add<Text>(0, 0, 150, 40)
                ->set_alignment(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE)
                ->set_fontSize(24)
                ->set_color(Color::white)
                ->set_text("Admin");
        row->addChild(Text::createIcon(40, 40, "\uf0c9", 0x333333FF, 24));
    }
    {
        auto row = titleBar->add<Row>(0, 0, 0, 40);
        row->anchors = new Anchors(row);
        row->anchors()->bind(row->anchors()->right, titleBar, titleBar->anchors()->right);

        row->addChild(Text::createIcon(40, 40, "\uf0e0", 0x333333FF, 20));
        row->addChild(Text::createIcon(40, 40, "\uf0ae", 0x333333FF, 20));
        row->add<Text>(0, 0, 128, 40)
                ->set_alignment(NVG_ALIGN_CENTER | NVG_ALIGN_MIDDLE)
                ->set_fontName("fas")
                ->set_fontSize(14)
                ->set_color(0x333333FF)
                ->set_text("\uf007 Sunil Reddy\uf0d7");
    }

    fflush(stdout);
}

int main() {
    //std::srand(time(nullptr));
    return app_exec(setupUI);
}
