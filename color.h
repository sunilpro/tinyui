#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include "nanovg.h"

struct Color
{
    static constexpr Color red() { return Color(255, 0, 0); }
    static constexpr Color green() { return Color(0, 255, 0); }
    static constexpr Color blue() { return Color(0, 0, 255); }
    static constexpr Color black() { return Color(0, 0, 0); }
    static constexpr Color white() { return Color(255, 255, 255); }

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r), g(g), b(b), a(a)
    {}

    NVGcolor vgColor() {
        NVGcolor color;
        // Use longer initialization to suppress warning.
        color.r = r / 255.0f;
        color.g = g / 255.0f;
        color.b = b / 255.0f;
        color.a = a / 255.0f;
        return color;
    }

    union {
        uint8_t rgba[4];
        struct {
            uint8_t r,g,b,a;
        };
    };
};

#endif // COLOR_H
