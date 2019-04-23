#include "cute.h"
#include <stdio.h>
#include <string.h>
#include <math.h>
#ifdef NANOVG_GLEW
#  include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>
#include "nanovg.h"
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"


#ifdef _MSC_VER
#define snprintf _snprintf
#elif !defined(__MINGW32__)
#include <iconv.h>
#endif

#define ICON_SEARCH 0x1F50D
#define ICON_CIRCLED_CROSS 0x2716
#define ICON_CHEVRON_RIGHT 0xE75E
#define ICON_CHECK 0x2713
#define ICON_LOGIN 0xE740
#define ICON_TRASH 0xE729

//static float minf(float a, float b) { return a < b ? a : b; }
static float maxf(float a, float b) { return a > b ? a : b; }
//static float absf(float a) { return a >= 0.0f ? a : -a; }
static float clampf(float a, float mn, float mx) { return a < mn ? mn : (a > mx ? mx : a); }

// Returns 1 if col.rgba is 0.0f,0.0f,0.0f,0.0f, 0 otherwise
int isBlack(NVGcolor col)
{
    if( col.r == 0.0f && col.g == 0.0f && col.b == 0.0f && col.a == 0.0f )
    {
        return 1;
    }
    return 0;
}

static char* cpToUTF8(int cp, char* str)
{
    int n = 0;
    if (cp < 0x80) n = 1;
    else if (cp < 0x800) n = 2;
    else if (cp < 0x10000) n = 3;
    else if (cp < 0x200000) n = 4;
    else if (cp < 0x4000000) n = 5;
    else if (cp <= 0x7fffffff) n = 6;
    str[n] = '\0';
    switch (n) {
    case 6: str[5] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x4000000;
    case 5: str[4] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x200000;
    case 4: str[3] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x10000;
    case 3: str[2] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0x800;
    case 2: str[1] = 0x80 | (cp & 0x3f); cp = cp >> 6; cp |= 0xc0;
    case 1: str[0] = cp;
    }
    return str;
}


void drawWindow(NVGcontext* vg, const char* title, float x, float y, float w, float h)
{
    float cornerRadius = 3.0f;
    NVGpaint shadowPaint;
    NVGpaint headerPaint;

    nvgSave(vg);
    //	nvgClearState(vg);

    // Window
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgFillColor(vg, nvgRGBA(28,30,34,192));
    //	nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgFill(vg);

    // Drop shadow
    shadowPaint = nvgBoxGradient(vg, x,y+2, w,h, cornerRadius*2, 10, nvgRGBA(0,0,0,128), nvgRGBA(0,0,0,0));
    nvgBeginPath(vg);
    nvgRect(vg, x-10,y-10, w+20,h+30);
    nvgRoundedRect(vg, x,y, w,h, cornerRadius);
    nvgPathWinding(vg, NVG_HOLE);
    nvgFillPaint(vg, shadowPaint);
    nvgFill(vg);

    // Header
    headerPaint = nvgLinearGradient(vg, x,y,x,y+15, nvgRGBA(255,255,255,8), nvgRGBA(0,0,0,16));
    nvgBeginPath(vg);
    nvgRoundedRect(vg, x+1,y+1, w-2,30, cornerRadius-1);
    nvgFillPaint(vg, headerPaint);
    nvgFill(vg);
    nvgBeginPath(vg);
    nvgMoveTo(vg, x+0.5f, y+0.5f+30);
    nvgLineTo(vg, x+0.5f+w-1, y+0.5f+30);
    nvgStrokeColor(vg, nvgRGBA(0,0,0,32));
    nvgStroke(vg);

    nvgFontSize(vg, 18.0f);
    nvgFontFace(vg, "sans-bold");
    nvgTextAlign(vg,NVG_ALIGN_CENTER|NVG_ALIGN_MIDDLE);

    nvgFontBlur(vg,2);
    nvgFillColor(vg, nvgRGBA(0,0,0,128));
    nvgText(vg, x+w/2,y+16+1, title, NULL);

    nvgFontBlur(vg,0);
    nvgFillColor(vg, nvgRGBA(220,220,220,160));
    nvgText(vg, x+w/2,y+16, title, NULL);

    nvgRestore(vg);
}

#include "rectangle.h"
#include "row.h"
#include "column.h"
#include "flickable.h"

static Item *root = nullptr;

extern "C" {
void scrollCallbackEvent(GLFWwindow* window, double x, double y) {
    const double scrollVal = 1;
    float _x = abs(x)>scrollVal?(x>0?-scrollVal:scrollVal):-x;
    float _y = abs(y)>scrollVal?(y>0?-scrollVal:scrollVal):-y;

    //printf("scrollCallbackEvent %f, %f  %lf, %lf\n", _x, _y, x, y);
    root->scrollEvent(150, 150, _x, _y);
}
}

void renderDemo(NVGcontext* vg, float mx, float my, float width, float height,
                float t, int blowup, DemoData* data)
{
    nvgSave(vg);
    if (blowup) {
        nvgRotate(vg, sinf(t*0.3f)*5.0f/180.0f*NVG_PI);
        nvgScale(vg, 2.0f, 2.0f);
    }

    static Flickable *flickable = nullptr;
    if(root == nullptr) {
        root = new Item(nullptr, 0, 0, 800, 800);
        Column *content;
        {
            flickable = root->add<Flickable>(100, 100, 210, 200);
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
    }
    //redRect->x = mx;
    //redRect->y = my;

    // Widgets
    //drawWindow(vg, "Widgets `n Stuff", mx, my, 300, 400);

    //drawRectangle(vg, 100, 100, 100, 100, nvgRGBA(255, 0, 0, 255));
    //drawRectangle(vg, 300, 100, 100, 100, nvgRGBA(0, 255, 0, 255));
    //drawRectangle(vg, 500, 100, 100, 100, nvgRGBA(0, 0, 255, 255));

    root->draw(vg);

    nvgRestore(vg);
}

static int mini(int a, int b) { return a < b ? a : b; }

static void unpremultiplyAlpha(unsigned char* image, int w, int h, int stride)
{
    int x,y;

    // Unpremultiply
    for (y = 0; y < h; y++) {
        unsigned char *row = &image[y*stride];
        for (x = 0; x < w; x++) {
            int r = row[0], g = row[1], b = row[2], a = row[3];
            if (a != 0) {
                row[0] = (int)mini(r*255/a, 255);
                row[1] = (int)mini(g*255/a, 255);
                row[2] = (int)mini(b*255/a, 255);
            }
            row += 4;
        }
    }

    // Defringe
    for (y = 0; y < h; y++) {
        unsigned char *row = &image[y*stride];
        for (x = 0; x < w; x++) {
            int r = 0, g = 0, b = 0, a = row[3], n = 0;
            if (a == 0) {
                if (x-1 > 0 && row[-1] != 0) {
                    r += row[-4];
                    g += row[-3];
                    b += row[-2];
                    n++;
                }
                if (x+1 < w && row[7] != 0) {
                    r += row[4];
                    g += row[5];
                    b += row[6];
                    n++;
                }
                if (y-1 > 0 && row[-stride+3] != 0) {
                    r += row[-stride];
                    g += row[-stride+1];
                    b += row[-stride+2];
                    n++;
                }
                if (y+1 < h && row[stride+3] != 0) {
                    r += row[stride];
                    g += row[stride+1];
                    b += row[stride+2];
                    n++;
                }
                if (n > 0) {
                    row[0] = r/n;
                    row[1] = g/n;
                    row[2] = b/n;
                }
            }
            row += 4;
        }
    }
}

static void setAlpha(unsigned char* image, int w, int h, int stride, unsigned char a)
{
    int x, y;
    for (y = 0; y < h; y++) {
        unsigned char* row = &image[y*stride];
        for (x = 0; x < w; x++)
            row[x*4+3] = a;
    }
}

static void flipHorizontal(unsigned char* image, int w, int h, int stride)
{
    int i = 0, j = h-1, k;
    while (i < j) {
        unsigned char* ri = &image[i * stride];
        unsigned char* rj = &image[j * stride];
        for (k = 0; k < w*4; k++) {
            unsigned char t = ri[k];
            ri[k] = rj[k];
            rj[k] = t;
        }
        i++;
        j--;
    }
}

void saveScreenShot(int w, int h, int premult, const char* name)
{
    unsigned char* image = (unsigned char*)malloc(w*h*4);
    if (image == NULL)
        return;
    glReadPixels(0, 0, w, h, GL_RGBA, GL_UNSIGNED_BYTE, image);
    if (premult)
        unpremultiplyAlpha(image, w, h, w*4);
    else
        setAlpha(image, w, h, w*4, 255);
    flipHorizontal(image, w, h, w*4);
    stbi_write_png(name, w, h, 4, image, w*4);
    free(image);
}

int loadDemoData(NVGcontext* vg, DemoData* data)
{
    int i;

    if (vg == NULL)
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

void freeDemoData(NVGcontext* vg, DemoData* data)
{
    int i;

    if (vg == nullptr)
        return;

}
