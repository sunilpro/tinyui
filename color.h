#ifndef COLOR_H
#define COLOR_H

#include <stdint.h>
#include "nanovg.h"

struct Color;

bool operator!=(const Color& lhs, const Color& rhs);

struct Color
{
    enum colors {
        red = 0xFF0000FF,
        green = 0x00FF00FF,
        blue = 0x0000FFFF,
        white = 0xFFFFFFFF,
        black = 0x000000FF
    };

    static Color random();

    constexpr Color(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : a(a), b(b), g(g), r(r)
    {}

    constexpr Color(uint32_t rgba): rgba(rgba) {}

    NVGcolor vgColor();

    union {
        uint32_t rgba;
        struct {
            uint8_t a,b,g,r;
        };
    };
};

#endif // COLOR_H
