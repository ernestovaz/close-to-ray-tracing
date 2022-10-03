#include "Color.h"

uint32_t Color::toInt() const {
    auto r = static_cast<unsigned char>(red);
    auto g = static_cast<unsigned char>(green);
    auto b = static_cast<unsigned char>(blue);

    //alpha channel always 255
    uint32_t color = (255 << 24) + (b << 16) + (g << 8) + r;

    return color;
}

Color::Color(double red, double green, double blue)
: red(red), green(green), blue(blue) {}
