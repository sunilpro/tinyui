
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

int randInRange(int min, int max)
{
  return min + (int) (rand() / (double) (RAND_MAX + 1) * (max - min + 1));
}

class MyAdaptor: public ListAdaptor {
public:
    virtual int count() {
        return 100;
    }

    virtual float hieghtForitemAt(int position) {
        return 60;
    }

    virtual ListItem *itemAt(int position) {
        auto listItem = new ListItem(position, 0, 0, 200, 60);
        auto rect = listItem->add<Rectangle>(0, 0, 200, 60);
        listItem->add<Text>(0, 0, 200, 60)->text = std::string("Item ") + std::to_string(position);
        if (position % 2 == 0)
            rect->color = nvgRGB(255-position%3, position*2, 255-position%4);
        else
            rect->color = nvgRGB(position%3, position*2, position%4);
        return listItem;
    }

    virtual ~MyAdaptor() {

    }
};

int main()
{
    DemoData data;
    Screen *screen = new Screen(1000, 800);

    if (loadDemoData(screen->vg(), &data) == -1)
        return -1;

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
            content->spacing = 5;
            content->padding = 5;
            content->anchors = new Anchors(content);
            //rect1->anchors()->bind(rect1->anchors()->bottom, redRect, redRect->anchors()->bottom);
            //rect1->anchors()->bind(rect1->anchors()->horizontalCenter, redRect, redRect->anchors()->horizontalCenter);

            content->add<Rectangle>(0, 0, 100, 100)->color = nvgRGBA(255, 0, 0, 255);
            content->add<Rectangle>(0, 0, 200, 100)->color = nvgRGBA(255, 255, 0, 255);
            content->add<Rectangle>(0, 0, 100, 200)->color = nvgRGBA(255, 0, 155, 255);
            content->add<Rectangle>(0, 0, 100, 10)->color = nvgRGBA(0, 0, 255, 255);
            content->add<Rectangle>(0, 0, 10, 100)->color = nvgRGBA(0, 255, 0, 255);
        }
        {
            auto rect1 = root->add<Rectangle>(100, 600, 200, 100);
            rect1->radius = 10;
            rect1->color = nvgRGBA(255, 0, 255, 255);
            rect1->anchors = new Anchors(rect1);
            rect1->anchors()->bind(rect1->anchors()->left, flickable, flickable->anchors()->left);
            //rect1->anchors()->bind(rect1->anchors()->right, redRect, redRect->anchors()->right);
            //rect1->anchors()->bind(rect1->anchors()->horizontalCenter, redRect, redRect->anchors()->horizontalCenter);
        }

        {
            auto listView = root->add<ListView>(400, 100, 300, 400);
            listView->setAdaptor(new MyAdaptor());
        }
    }

    while (!glfwWindowShouldClose(screen->window()))
    {
        screen->drawAll();
        glfwPollEvents();
    }

    freeDemoData(screen->vg(), &data);

    glfwTerminate();
}

/*
#include <stdio.h>
#ifdef NANOVG_GLEW
#	include <GL/glew.h>
#endif
#ifdef __APPLE__
#	define GLFW_INCLUDE_GLCOREARB
#endif
#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"
#include "cute.h"
#include "perf.h"


void errorcb(int error, const char* desc)
{
	printf("GLFW error %d: %s\n", error, desc);
}

int blowup = 0;
int screenshot = 0;
int premult = 0;

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

extern void scrollCallbackEvent(GLFWwindow* window, double x, double y);

int main()
{
	GLFWwindow* window;
	DemoData data;
	NVGcontext* vg = NULL;
	GPUtimer gpuTimer;
	PerfGraph fps, cpuGraph, gpuGraph;
	double prevt = 0, cpuTime = 0;

	if (!glfwInit()) {
		printf("Failed to init GLFW.");
		return -1;
	}

	initGraph(&fps, GRAPH_RENDER_FPS, "Frame Time");
	initGraph(&cpuGraph, GRAPH_RENDER_MS, "CPU Time");
	initGraph(&gpuGraph, GRAPH_RENDER_MS, "GPU Time");

	glfwSetErrorCallback(errorcb);
#ifndef _WIN32 // don't require this on win32, and works with more cards
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#endif
	glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

#ifdef DEMO_MSAA
	glfwWindowHint(GLFW_SAMPLES, 4);
#endif
    window = glfwCreateWindow(1000, 800, "NanoVG", NULL, NULL);
//	window = glfwCreateWindow(1000, 600, "NanoVG", glfwGetPrimaryMonitor(), NULL);
	if (!window) {
		glfwTerminate();
		return -1;
	}

	glfwSetKeyCallback(window, key);
    glfwSetScrollCallback(window, scrollCallbackEvent);
	glfwMakeContextCurrent(window);
#ifdef NANOVG_GLEW
	glewExperimental = GL_TRUE;
	if(glewInit() != GLEW_OK) {
		printf("Could not init glew.\n");
		return -1;
	}
	// GLEW generates GL error because it calls glGetString(GL_EXTENSIONS), we'll consume it here.
	glGetError();
#endif

#ifdef DEMO_MSAA
	vg = nvgCreateGL3(NVG_STENCIL_STROKES | NVG_DEBUG);
#else
    vg = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES | NVG_DEBUG);
#endif
	if (vg == NULL) {
		printf("Could not init nanovg.\n");
		return -1;
	}

	if (loadDemoData(vg, &data) == -1)
		return -1;

	glfwSwapInterval(0);

	initGPUTimer(&gpuTimer);

	glfwSetTime(0);
	prevt = glfwGetTime();

	while (!glfwWindowShouldClose(window))
	{
		double mx, my, t, dt;
		int winWidth, winHeight;
		int fbWidth, fbHeight;
		float pxRatio;
		float gpuTimes[3];
		int i, n;

		t = glfwGetTime();
		dt = t - prevt;
		prevt = t;

		startGPUTimer(&gpuTimer);

		glfwGetCursorPos(window, &mx, &my);
		glfwGetWindowSize(window, &winWidth, &winHeight);
		glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
		// Calculate pixel ration for hi-dpi devices.
		pxRatio = (float)fbWidth / (float)winWidth;

		// Update and render
		glViewport(0, 0, fbWidth, fbHeight);
		if (premult)
			glClearColor(0,0,0,0);
		else
			glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT|GL_STENCIL_BUFFER_BIT);

		nvgBeginFrame(vg, winWidth, winHeight, pxRatio);

		renderDemo(vg, mx,my, winWidth,winHeight, t, blowup, &data);

		renderGraph(vg, 5,5, &fps);
		renderGraph(vg, 5+200+5,5, &cpuGraph);
		if (gpuTimer.supported)
			renderGraph(vg, 5+200+5+200+5,5, &gpuGraph);

		nvgEndFrame(vg);

		// Measure the CPU time taken excluding swap buffers (as the swap may wait for GPU)
		cpuTime = glfwGetTime() - t;

		updateGraph(&fps, dt);
		updateGraph(&cpuGraph, cpuTime);

		// We may get multiple results.
		n = stopGPUTimer(&gpuTimer, gpuTimes, 3);
		for (i = 0; i < n; i++)
			updateGraph(&gpuGraph, gpuTimes[i]);

		if (screenshot) {
			screenshot = 0;
			saveScreenShot(fbWidth, fbHeight, premult, "dump.png");
		}

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	freeDemoData(vg, &data);

	nvgDeleteGL3(vg);

	printf("Average Frame Time: %.2f ms\n", getGraphAverage(&fps) * 1000.0f);
	printf("          CPU Time: %.2f ms\n", getGraphAverage(&cpuGraph) * 1000.0f);
	printf("          GPU Time: %.2f ms\n", getGraphAverage(&gpuGraph) * 1000.0f);

	glfwTerminate();
	return 0;
}
*/
