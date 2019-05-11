#ifndef ITEM_H
#define ITEM_H

#include "anchors.h"
#include "object.h"
#include <vector>
#include <math.h>

struct NVGcontext;

/// Cursor shapes available to use in GLFW.  Shape of actual cursor determined by Operating System.
enum class Cursor {
    Arrow = 0,  ///< The arrow cursor.
    IBeam,      ///< The I-beam cursor.
    Crosshair,  ///< The crosshair cursor.
    Hand,       ///< The hand cursor.
    HResize,    ///< The horizontal resize cursor.
    VResize,    ///< The vertical resize cursor.
    CursorCount ///< Not a cursor --- should always be last: enables a loop over the cursor types.
};


typedef void (* AnimationProgressCallback)(double);

struct Animation {
    double duration;
    double startTime;
    std::function<void(double)> cb;

};

class Item: public Object {
public:
    static void addAnimation(double duration, std::function<void(double)> cb);
    static void performAnimations();
private:
    static std::vector<Animation> animations;

public:
    PROPERTY(Item*, parent);
    PROPERTY(Anchors*, anchors) = nullptr;
    PROPERTY(bool, visible);
    PROPERTY(bool, focus) = false;
    PROPERTY(float, x);
    PROPERTY(float, y);
    PROPERTY(float, width);
    PROPERTY(float, hieght);

public:
    Item(Item *parent, float x = 0, float y = 0, float width = 0, float hieght = 0);

    int childCount() const { return (int) mChildren.size(); }

    const std::vector<Item *> &children() const { return mChildren; }

    virtual Item* addChild(int index, Item * widget);

    Item* addChild(Item * widget);

    void removeChild(const Item *widget);

    void removeChild(int index);

    /// Retrieves the child at the specific position
    const Item* childAt(int index) const { return mChildren[index]; }

    /// Retrieves the child at the specific position
    Item* childAt(int index) { return mChildren[index]; }

    /// Returns the index of a specific child or -1 if not found
    int childIndex(Item* widget) const;

    /// Variadic shorthand notation to construct and add a child widget
    template<typename WidgetClass, typename... Args>
    WidgetClass* add(const Args&... args) {
        return new WidgetClass(this, args...);
    }

    /// Return the absolute position on screen
    void absolutePosition(float *ax, float *ay) const {
        if (parent()) {
            float _x, _y;
            parent()->absolutePosition(&_x, &_y);
            *ax = _x + x();
            *ay = _y + y();
        } else {
            *ax = x();
            *ay = y();
        }
    }

    bool contains(float x, float y) const {
        float dx = x - this->x();
        float dy = y - this->y();
        return dx >= 0 && dy >= 0 && dx < width() && dy < hieght();
    }

    /// Determine the widget located at the given position value (recursive)
    Item *findItem(float x, float y);

    /// Determine the immediate child Item located at the given position value (recursive)
    Item *findChild(float x, float y);

    virtual void performLayout();
    virtual void draw(NVGcontext *ctx);

    /// Request the focus to be moved to this widget
    void requestFocus();

    /// Handle a mouse button event (default implementation: propagate to children)
    virtual bool mouseButtonEvent(float mx, float my, int button, bool down, int modifiers);

    /// Handle a mouse motion event (default implementation: propagate to children)
    virtual bool mouseMotionEvent(float mx, float my, float rx, float ry, int button, int modifiers);

    /// Handle a mouse drag event (default implementation: do nothing)
    virtual bool mouseDragEvent(float mx, float my, float rx, float ry, int button, int modifiers);

    /// Handle a mouse enter/leave event (default implementation: record this fact, but do nothing)
    virtual bool mouseEnterEvent(float mx, float my, bool enter);

    /// Handle a focus change event (default implementation: record the focus status, but do nothing)
    virtual bool focusEvent(bool focused);

    /// Handle a keyboard event (default implementation: do nothing)
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);

    /// Handle text input (UTF-32 format) (default implementation: do nothing)
    virtual bool keyboardCharacterEvent(unsigned int codepoint);

    /// Handle a mouse scroll event (default implementation: propagate to children)
    virtual bool scrollEvent(float mx, float my, float rx, float ry);
protected:
    virtual ~Item();
    std::vector<Item *> mChildren;
    bool mMouseFocus;
};
#endif // ITEM_H
