#include "color.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

bool operator!=(const Color& lhs, const Color& rhs) {
    return lhs.rgba != rhs.rgba;
}

Color Color::random() {
    Color color(0x000000FF);
    color.r = rand() % 255;
    color.g = rand() % 255;
    color.b = rand() % 255;
    return color;
}

NVGcolor Color::vgColor() const {
    NVGcolor color;
    // Use longer initialization to suppress warning.
    color.r = r / 255.0f;
    color.g = g / 255.0f;
    color.b = b / 255.0f;
    color.a = a / 255.0f;

    //printf("%x -> %x %x %x %x\n", this->rgba, r, g, b, a);

    return color;
}
