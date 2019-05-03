
#include "cute.h"
#include "screen.h"

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

int randInRange(int min, int max)
{
    int range = max - min + 1;
    return rand() % range + min;
}

int app_exec(ScreenReadyCallback screenReadyCallback)
{
    DemoData data;
    Screen *screen = new Screen(1000, 800);

    if (loadDemoData(screen->vg(), &data) == -1)
        return -1;

    screenReadyCallback(screen);

    while (!glfwWindowShouldClose(screen->window()))
    {
        //double start = glfwGetTime();
        //printf("Start = %lf, ", start);
        Item::performAnimations();
        //printf("Animation = %lf, ", glfwGetTime() - start);
        //start = glfwGetTime();
        screen->drawAll();
        //printf("Draw = %lf\n", glfwGetTime() - start);
        glfwPollEvents();
    }

    freeDemoData(screen->vg(), &data);

    glfwTerminate();
    return 0;
}


int loadDemoData(NVGcontext* vg, DemoData* data)
{
    if (vg == nullptr)
        return -1;

    data->fontIcons = nvgCreateFont(vg, "icons", "/Users/sunilreddy/Desktop/crystalmatter/webassembly/tinygui/TinyGUI/resources/entypo.ttf");
    if (data->fontIcons == -1) {
        printf("Could not add font icons.\n");
        return -1;
    }
    data->fontNormal = nvgCreateFont(vg, "sans", "/Users/sunilreddy/Desktop/crystalmatter/webassembly/tinygui/TinyGUI/resources/Roboto-Regular.ttf");
    if (data->fontNormal == -1) {
        printf("Could not add font italic.\n");
        return -1;
    }
    data->fontBold = nvgCreateFont(vg, "sans-bold", "/Users/sunilreddy/Desktop/crystalmatter/webassembly/tinygui/TinyGUI/resources/Roboto-Bold.ttf");
    if (data->fontBold == -1) {
        printf("Could not add font bold.\n");
        return -1;
    }
    data->fontEmoji = nvgCreateFont(vg, "emoji", "/Users/sunilreddy/Desktop/crystalmatter/webassembly/tinygui/TinyGUI/resources/NotoEmoji-Regular.ttf");
    if (data->fontEmoji == -1) {
        printf("Could not add font emoji.\n");
        return -1;
    }
    nvgAddFallbackFontId(vg, data->fontNormal, data->fontEmoji);
    nvgAddFallbackFontId(vg, data->fontBold, data->fontEmoji);

    return 0;
}

void freeDemoData(NVGcontext* vg, DemoData*)
{
    if (vg == nullptr)
        return;
}
