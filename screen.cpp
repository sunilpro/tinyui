#include "screen.h"
#include "popup.h"

#ifdef __APPLE__
#	define GLFW_INCLUDE_GLCOREARB
#endif

#ifdef EMSCRIPTEN
#define GLFW_INCLUDE_ES2
#endif

#define GLFW_INCLUDE_GLEXT
#include <GLFW/glfw3.h>
#include "nanovg.h"

/*
#if defined (__wasm__)
//#define NANOVG_GLES3_IMPLEMENTATION
#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
//#define GLFW_INCLUDE_ES3
#define NANOVG_FBO_VALID 1
#include "nanovg_gl_utils.h"

#else

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#endif
*/

#ifdef EMSCRIPTEN

#define NANOVG_GLES2_IMPLEMENTATION
#include "nanovg_gl.h"
#define NANOVG_FBO_VALID 1
#include "nanovg_gl_utils.h"

#include <emscripten.h>
#include <emscripten/html5.h>

#else

#define NANOVG_GL3_IMPLEMENTATION
#include "nanovg_gl.h"

#endif

#include <cmath>

Screen *gScreen = nullptr;
Screen::Screen(float w, float h, float dpr): Item(nullptr, 0, 0, w, h), mColor(Color::white), mPixelRatio(dpr)
{
    gScreen = this;
    memset(mCursors, 0, sizeof(GLFWcursor *) * (int) Cursor::CursorCount);

    if (!glfwInit()) {
        printf("Failed to init GLFW.\n");
        return;
    }


#ifdef EMSCRIPTEN
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
    glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#else
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);
#endif

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);

#ifdef EMSCRIPTEN
    mGLFWWindow = glfwCreateWindow(w*mPixelRatio, h*mPixelRatio, "Hello World!", nullptr, nullptr);
#else
    mGLFWWindow = glfwCreateWindow(w, h, "Hello World!", nullptr, nullptr);
#endif

    if (!mGLFWWindow)
        throw std::runtime_error("Could not create an OpenGL 3 2 context!");

    glfwMakeContextCurrent(mGLFWWindow);

    glfwGetFramebufferSize(mGLFWWindow, &mFBSize[0], &mFBSize[1]);
    glViewport(0, 0, mFBSize[0], mFBSize[1]);
    glClearColor(mColor.r/255.0f, mColor.g/255.0f, mColor.b/255.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    glfwSwapInterval(0);
    glfwSwapBuffers(mGLFWWindow);

#if defined(__APPLE__)
    /* Poll for events once before starting a potentially
       lengthy loading process. This is needed to be
       classified as "interactive" by other software such
       as iTerm2 */

    glfwPollEvents();
#endif

    /* Propagate GLFW events to the appropriate Screen instance */
    glfwSetCursorPosCallback(mGLFWWindow, [](GLFWwindow *, double x, double y) {
#ifdef EMSCRIPTEN
        gScreen->cursorPosCallbackEvent(x/gScreen->mPixelRatio, y/gScreen->mPixelRatio);
#else
        gScreen->cursorPosCallbackEvent(x, y);
#endif
    });
    glfwSetMouseButtonCallback(mGLFWWindow, [](GLFWwindow *, int button, int action, int modifiers) {
        gScreen->mouseButtonCallbackEvent(button, action, modifiers);
    });

    glfwSetKeyCallback(mGLFWWindow, [](GLFWwindow *, int key, int scancode, int action, int mods) {
        gScreen->keyCallbackEvent(key, scancode, action, mods);
    });

    glfwSetCharCallback(mGLFWWindow, [](GLFWwindow *, unsigned int codepoint) {
        gScreen->charCallbackEvent(codepoint);
    });

    glfwSetDropCallback(mGLFWWindow, [](GLFWwindow *, int count, const char **filenames) {
        gScreen->dropCallbackEvent(count, filenames);
    });

    glfwSetScrollCallback(mGLFWWindow, [](GLFWwindow *, double x, double y) {
        const double scrollVal = 1;
        float _x = std::abs(x)>scrollVal?(x>0?-scrollVal:scrollVal):-x;
        float _y = std::abs(y)>scrollVal?(y>0?-scrollVal:scrollVal):-y;

        //printf("scrollCallbackEvent %f, %f  %lf, %lf\n", _x, _y, x, y);
        gScreen->scrollEvent(gScreen->mMousePos[0], gScreen->mMousePos[1], _x, _y);
    });

    /* React to framebuffer size events -- includes window
       size events and also catches things like dragging
       a window from a Retina-capable screen to a normal
       screen on Mac OS X */
    glfwSetFramebufferSizeCallback(mGLFWWindow, [](GLFWwindow *, int width, int height) {
        gScreen->resizeCallbackEvent(width, height);
    });

    // notify when the screen has lost focus (e.g. application switch)
    glfwSetWindowFocusCallback(mGLFWWindow, [](GLFWwindow *, int focused) {
        gScreen->focusEvent(focused != 0);
    });

    initialize(mGLFWWindow);
}

void Screen::initialize(GLFWwindow *window) {
    mGLFWWindow = window;
    glfwGetWindowSize(mGLFWWindow, &mSize[0], &mSize[1]);
    glfwGetFramebufferSize(mGLFWWindow, &mFBSize[0], &mFBSize[1]);

    /* Detect framebuffer properties and set up compatible NanoVG context */
    //GLint nStencilBits = 0, nSamples = 0;
    //glGetFramebufferAttachmentParameteriv(GL_DRAW_FRAMEBUFFER, GL_STENCIL, GL_FRAMEBUFFER_ATTACHMENT_STENCIL_SIZE, &nStencilBits);
    //glGetIntegerv(GL_SAMPLES, &nSamples);
#ifdef EMSCRIPTEN
    mNVGContext = nvgCreateGLES2(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#else
    mNVGContext = nvgCreateGL3(NVG_ANTIALIAS | NVG_STENCIL_STROKES);
#endif
    if (mNVGContext == nullptr)
        throw std::runtime_error("Could not initialize NanoVG!");

    mMousePos[0] = 0;
    mMousePos[1] = 0;
    mMouseState = mModifiers = 0;
    mDragActive = false;

    for (int i=0; i < (int) Cursor::CursorCount; ++i)
        mCursors[i] = glfwCreateStandardCursor(GLFW_ARROW_CURSOR + i);

    /// Fixes retina display-related font rendering issue (#185)
    nvgBeginFrame(mNVGContext, mSize[0], mSize[1], mPixelRatio);
    nvgEndFrame(mNVGContext);

    mPopup = add<Popup>(0, 0, 200, 400);
}

Screen::~Screen() {
    for (int i=0; i < (int) Cursor::CursorCount; ++i) {
        if (mCursors[i])
            glfwDestroyCursor(mCursors[i]);
    }
#ifdef EMSCRIPTEN
    if (mNVGContext)
        nvgDeleteGLES2(mNVGContext);
#else
    if (mNVGContext)
        nvgDeleteGL3(mNVGContext);
#endif
    if (mGLFWWindow)
        glfwDestroyWindow(mGLFWWindow);
}

void Screen::setBackground(Color color) {
    mColor = color;
}

void Screen::setCursor(Cursor cursor) {
    mCursor = cursor;
    glfwSetCursor(mGLFWWindow, mCursors[(int) mCursor]);
}

void Screen::drawAll() {
    glClearColor(mColor.r/255.0f, mColor.g/255.0f, mColor.b/255.0f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    drawContents();
    drawWidgets();

    glfwSwapBuffers(mGLFWWindow);
}

void Screen::drawWidgets() {

    glfwMakeContextCurrent(mGLFWWindow);

    glfwGetFramebufferSize(mGLFWWindow, &mFBSize[0], &mFBSize[1]);
    glfwGetWindowSize(mGLFWWindow, &mSize[0], &mSize[1]);

    glViewport(0, 0, mFBSize[0], mFBSize[1]);
    //printf("mSize = (%d, %d)\n", mSize[0], mSize[1]);
    //glBindSampler(0, 0);
#ifdef EMSCRIPTEN
    mSize[0] = mSize[0]/mPixelRatio;
    mSize[1] = mSize[1]/mPixelRatio;
#endif
    nvgBeginFrame(mNVGContext, mSize[0], mSize[1], mPixelRatio);

    draw(mNVGContext);

    nvgEndFrame(mNVGContext);
}

bool Screen::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (mFocusPath.size() > 0) {
        for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
            if ((*it)->focus() && (*it)->keyboardEvent(key, scancode, action, modifiers))
                return true;
    }

    return false;
}

bool Screen::keyboardCharacterEvent(unsigned int codepoint) {
    if (mFocusPath.size() > 0) {
        for (auto it = mFocusPath.rbegin() + 1; it != mFocusPath.rend(); ++it)
            if ((*it)->focus() && (*it)->keyboardCharacterEvent(codepoint))
                return true;
    }
    return false;
}

bool Screen::cursorPosCallbackEvent(float x, float y) {
    float p[2] = {x, y};

    bool ret = false;
    try {
        //p[0] -= 1;
        //p[1] -= 2;

        if (mDragActive) {
            float _x, _y;
            mDragWidget->parent()->absolutePosition(&_x, &_y);
            ret = mDragWidget->mouseDragEvent(p[0] - _x, p[1] - _y, p[0] - mMousePos[0], p[1] - mMousePos[1],
                    mMouseState, mModifiers);
        }

        if (!ret)
            ret = mouseMotionEvent(p[0], p[1], p[0] - mMousePos[0], p[1] - mMousePos[1], mMouseState, mModifiers);

        mMousePos[0] = p[0];
        mMousePos[1] = p[1];
        return ret;
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << __FUNCTION__ << e.what() << std::endl;
        return false;
    }
}

bool Screen::mouseButtonCallbackEvent(int button, int action, int modifiers) {
    mModifiers = modifiers;
    try {
        if (action != GLFW_PRESS && popup()->visible() && popup()->anchorItem()) {
            popup()->visible = popup()->anchorItem()->containsAbs(mMousePos[0], mMousePos[1]);
        }
        if (action == GLFW_PRESS)
            mMouseState |= 1 << button;
        else
            mMouseState &= ~(1 << button);

        auto dropWidget = findItem(mMousePos[0], mMousePos[1]);
        if (mDragActive && action == GLFW_RELEASE && dropWidget != mDragWidget) {
            float _x, _y;
            mDragWidget->parent()->absolutePosition(&_x, &_y);
            mDragWidget->mouseButtonEvent(mMousePos[0] - _x, mMousePos[1] - _y, button,
                    false, mModifiers);
        }

        if (action == GLFW_PRESS && (button == GLFW_MOUSE_BUTTON_1 || button == GLFW_MOUSE_BUTTON_2)) {
            mDragWidget = findItem(mMousePos[0], mMousePos[1]);
            if (mDragWidget == this)
                mDragWidget = nullptr;
            mDragActive = mDragWidget != nullptr;
            if (!mDragActive)
                updateFocus(nullptr);
        } else {
            mDragActive = false;
            mDragWidget = nullptr;
        }

        return mouseButtonEvent(mMousePos[0], mMousePos[1], button, action == GLFW_PRESS, mModifiers);
    } catch (const std::exception &e) {
        printf("Caught exception in event handler: %s %d\n", __FUNCTION__, action == GLFW_PRESS);
        return false;
    }
}

bool Screen::keyCallbackEvent(int key, int scancode, int action, int mods) {
    try {
        return keyboardEvent(key, scancode, action, mods);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << __FUNCTION__ << e.what() << std::endl;
        return false;
    }
}

bool Screen::charCallbackEvent(unsigned int codepoint) {
    try {
        return keyboardCharacterEvent(codepoint);
    } catch (const std::exception &e) {
        std::cerr << "Caught exception in event handler: " << e.what()
                  << std::endl;
        return false;
    }
}

bool Screen::dropCallbackEvent(int count, const char **filenames) {
    std::vector<std::string> arg(count);
    for (int i = 0; i < count; ++i)
        arg[i] = filenames[i];
    return dropEvent(arg);
}

bool Screen::resizeCallbackEvent(int, int) {
    int fbSize[2], size[2];
    glfwGetFramebufferSize(mGLFWWindow, &fbSize[0], &fbSize[1]);
    glfwGetWindowSize(mGLFWWindow, &size[0], &size[1]);

    if ((mFBSize[0] == 0 && mFBSize[1] == 0) || (size[0] == 0 && size[1] == 0))
        return false;

    mFBSize[0] = fbSize[0]; mSize[0] = size[0];
    mFBSize[1] = fbSize[1]; mSize[1] = size[1];

    //printf("resizeCallbackEvent\n");
    return false;
}

void Screen::updateFocus(Item *widget) {
    for (auto w: mFocusPath) {
        if (!w->focus())
            continue;
        w->focusEvent(false);
    }
    mFocusPath.clear();
    while (widget) {
        mFocusPath.push_back(widget);
        widget = widget->parent();
    }
    for (auto it = mFocusPath.rbegin(); it != mFocusPath.rend(); ++it)
        (*it)->focusEvent(true);
}
