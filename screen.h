#ifndef SCREEN_H
#define SCREEN_H

#include "item.h"
#include "color.h"

struct GLFWwindow;
struct GLFWcursor;

class Screen: public Item
{
public:
    Screen(float w, float h, float dpr);

    void updateFocus(Item *widget);

    GLFWwindow* window() { return mGLFWWindow; }
    NVGcontext* vg() { return mNVGContext; }

    void setBackground(Color color);

    /// Draw the Screen contents
    virtual void drawAll();

    /// Draw the window contents --- put your OpenGL draw calls here
    virtual void drawContents() { /* To be overridden */ }

    void drawWidgets();

protected:
    void initialize(GLFWwindow *window);

    /// Handle a file drop event
    virtual bool dropEvent(const std::vector<std::string> & /* filenames */) { return false; /* To be overridden */ }

    bool keyboardEvent(int key, int scancode, int action, int modifiers);

    bool keyboardCharacterEvent(unsigned int codepoint);

    bool cursorPosCallbackEvent(float x, float y);

    bool mouseButtonCallbackEvent(int button, int action, int modifiers);

    bool keyCallbackEvent(int key, int scancode, int action, int mods);

    bool charCallbackEvent(unsigned int codepoint);

    bool dropCallbackEvent(int count, const char **filenames);

    bool resizeCallbackEvent(int, int);

private:
    Color mColor;
    std::vector<Item *> mFocusPath;
    bool mDragActive;
    Item *mDragWidget = nullptr;
    float mPixelRatio;
    GLFWwindow *mGLFWWindow;
    NVGcontext* mNVGContext;
    GLFWcursor *mCursors[(int) Cursor::CursorCount];
    Cursor mCursor = Cursor::Arrow;
    int mMouseState, mModifiers;
    int mMousePos[2];
    int mFBSize[2];
    int mSize[2];
};

typedef void (*ScreenReadyCallback)(Screen*);

int app_exec(ScreenReadyCallback screenReadyCallback);

#endif // SCREEN_H
