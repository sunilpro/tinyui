
#include "cute.h"
#include "screen.h"

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>

#include "rectangle.h"
#include "row.h"
#include "column.h"
#include "flickable.h"
#include "listview.h"
#include "text.h"

#include <emscripten.h>
#include <emscripten/html5.h>

#include "perf.h"

double w, h, dpi_scale;

int loadDemoData(NVGcontext* vg, DemoData* data)
{
    int i;

    if (vg == nullptr)
        return -1;

    data->fontIcons = nvgCreateFont(vg, "icons", "entypo.ttf");
    if (data->fontIcons == -1) {
        printf("Could not add font icons.\n");
        return -1;
    }
    data->fontNormal = nvgCreateFont(vg, "sans", "Roboto-Regular.ttf");
    if (data->fontNormal == -1) {
        printf("Could not add font italic.\n");
        return -1;
    }
    data->fontBold = nvgCreateFont(vg, "sans-bold", "Roboto-Bold.ttf");
    if (data->fontBold == -1) {
        printf("Could not add font bold.\n");
        return -1;
    }
    data->fontEmoji = nvgCreateFont(vg, "emoji", "NotoEmoji-Regular.ttf");
    if (data->fontEmoji == -1) {
        printf("Could not add font emoji.\n");
        return -1;
    }
    nvgAddFallbackFontId(vg, data->fontNormal, data->fontEmoji);
    nvgAddFallbackFontId(vg, data->fontBold, data->fontEmoji);

    return 0;
}

void freeDemoData(NVGcontext* vg, DemoData* data)
{
    int i;

    if (vg == nullptr)
        return;

}


PerfGraph fps;
double prevt = 0;

void render();
extern Screen *gScreen;
void render() {
    auto t = glfwGetTime();
    auto dt = t - prevt;
    prevt = t;
    updateGraph(&fps, dt);

    Item::performAnimations();
    gScreen->drawAll();
    renderGraph(gScreen->vg(), 5,5, &fps);

    glfwPollEvents();
}

EM_BOOL _sapp_emsc_size_changed(int event_type, const EmscriptenUiEvent* ui_event, void* user_data) {
    emscripten_get_element_css_size("canvas", &w, &h);
    dpi_scale = emscripten_get_device_pixel_ratio();
    w *= dpi_scale;
    h *= dpi_scale;
    emscripten_set_canvas_element_size("canvas", w, h);
    glfwSetWindowSize(gScreen->window(), w, h);
    printf("_sapp_emsc_size_changed %f, %f, %f\n", w, h, dpi_scale);
    return 1;
}

int randInRange(int min, int max)
{
    int range = max - min + 1;
    return rand() % range + min;
}

int app_exec(ScreenReadyCallback screenReadyCallback)
{
    emscripten_get_element_css_size("canvas", &w, &h);
    emscripten_set_resize_callback(0, 0, 0, _sapp_emsc_size_changed);
    printf("main %f, %f\n", w, h);

    dpi_scale = emscripten_get_device_pixel_ratio();
    w *= dpi_scale;
    h *= dpi_scale;
    emscripten_set_canvas_element_size("canvas", w, h);

    printf("main %f, %f, %f\n", w, h, dpi_scale);

    DemoData data;
    Screen *screen = new Screen(w, h);

    if (loadDemoData(screen->vg(), &data) == -1) {
        abort();
        return -1;
    }

    glfwSwapInterval(0);

    glfwSetTime(0);
    prevt = glfwGetTime();

    initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

    screenReadyCallback(screen);

    emscripten_set_main_loop(render, 0, 1);

    freeDemoData(screen->vg(), &data);

    glfwTerminate();

    return 0;
}

/*
#include <stdio.h>
#define GLFW_INCLUDE_ES3
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#define NANOVG_FBO_VALID 1
#include "nanovg_gl_utils.h"
#include "cute.h"
#include "perf.h"

#include <emscripten.h>
#include <emscripten/html5.h>

double w, h, dpi_scale;

void errorcb(int error, const char* desc)
{
    printf("GLFW error %d: %s\n", error, desc);
}

int blowup = 0;
int screenshot = 0;
int premult = 0;

GLFWwindow* window;
DemoData data;
NVGcontext* vg = NULL;
PerfGraph fps;
double prevt = 0;

static void key(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    NVG_NOTUSED(scancode);
    NVG_NOTUSED(mods);
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
    if (key == GLFW_KEY_SPACE && action == GLFW_PRESS)
        blowup = !blowup;
    if (key == GLFW_KEY_S && action == GLFW_PRESS)
        screenshot = 1;
    if (key == GLFW_KEY_P && action == GLFW_PRESS)
        premult = !premult;
}

void render();

void render() {
    double mx, my, t, dt;
    int winWidth, winHeight;
    int fbWidth, fbHeight;

    t = glfwGetTime();
    dt = t - prevt;
    prevt = t;
    updateGraph(&fps, dt);

    glfwGetCursorPos(window, &mx, &my);
    glfwGetWindowSize(window, &winWidth, &winHeight);
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);


    //printf("render %d,%d %d,%d %f\n", winWidth, winHeight, fbWidth, fbHeight, dpi_scale);

    // Update and render
    glViewport(0, 0, fbWidth, fbHeight);
    if (premult)
        glClearColor(0,0,0,0);
    else
        glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_CULL_FACE);
    glDisable(GL_DEPTH_TEST);

    nvgBeginFrame(vg, winWidth/dpi_scale, winHeight/dpi_scale, dpi_scale);

    renderDemo(vg, mx/dpi_scale,my/dpi_scale, winWidth/dpi_scale, winHeight/dpi_scale, t, blowup, &data);
    renderGraph(vg, 5,5, &fps);

    nvgEndFrame(vg);

    glEnable(GL_DEPTH_TEST);

    if (screenshot) {
        screenshot = 0;
        saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
    }

    glfwSwapBuffers(window);
    glfwPollEvents();
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    printf("framebuffer_size_callback %d,%d\n", width, height);
    glViewport(0, 0, width, height);
}

EM_BOOL _sapp_emsc_size_changed(int event_type, const EmscriptenUiEvent* ui_event, void* user_data) {
    emscripten_get_element_css_size("canvas", &w, &h);
    dpi_scale = emscripten_get_device_pixel_ratio();
    w *= dpi_scale;
    h *= dpi_scale;
    emscripten_set_canvas_element_size("canvas", w, h);
    glfwSetWindowSize(window, w, h);
    //printf("_sapp_emsc_size_changed %f, %f, %f\n", w, h, dpi_scale);
    return 1;
}

int main()
{
    emscripten_get_element_css_size("canvas", &w, &h);
    emscripten_set_resize_callback(0, 0, 0, _sapp_emsc_size_changed);
    printf("main %f, %f\n", w, h);

    dpi_scale = emscripten_get_device_pixel_ratio();
    w *= dpi_scale;
    h *= dpi_scale;
    emscripten_set_canvas_element_size("canvas", w, h);

    printf("main %f, %f, %f\n", w, h, dpi_scale);

    if (!glfwInit()) {
        printf("Failed to init GLFW.");
        return -1;
    }

    initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");

    glfwSetErrorCallback(errorcb);

    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);

    window = glfwCreateWindow(w, h, "NanoVG", NULL, NULL);
    //window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwSetKeyCallback(window, key);

    glfwMakeContextCurrent(window);

    vg = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
    if (vg == NULL) {
        printf("Could not init nanovg.\n");
        return -1;
    }

    if (loadDemoData(vg, &data) == -1)
        return -1;

    glfwSwapInterval(0);

    glfwSetTime(0);
    prevt = glfwGetTime();

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    emscripten_set_main_loop(render, 0, 1);

    freeDemoData(vg, &data);

    nvgDeleteGLES2(vg);

    glfwTerminate();
    return 0;
}
*/
