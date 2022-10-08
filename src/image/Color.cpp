#include "Color.h"

#include <glm/glm.hpp>

using glm::min;
using glm::max;

const Color Color::WHITE = Color(255.0f, 255.0f, 255.0f);

Color::Color(double red, double green, double blue)
        : red(red), green(green), blue(blue) {}

void Color::clamp() {
    this->red   = min(255.0, max(this->red,   0.0));
    this->green = min(255.0, max(this->green, 0.0));
    this->blue  = min(255.0, max(this->blue,  0.0));
}

uint32_t Color::toInt() const {
    auto r = static_cast<unsigned char>(red);
    auto g = static_cast<unsigned char>(green);
    auto b = static_cast<unsigned char>(blue);

    //alpha channel always 255
    uint32_t color = (255 << 24) + (b << 16) + (g << 8) + r;

    return color;
}


// operator overloading
Color& operator+=(Color &a, const Color &b) {
    a.red    += b.red;
    a.green  += b.green;
    a.blue   += b.blue;

    return a;
}

Color operator+(const Color &a, const Color &b) {
    Color result;

    result.red      = a.red     + b.red;
    result.blue     = a.blue    + b.blue;
    result.green    = a.green   + b.green;

    return result;
}

Color operator*(const Color &a, const Color &b) {
    Color multiplier, result;

    multiplier.red      = a.red / 255.0f;
    multiplier.green    = a.green / 255.0f;
    multiplier.blue     = a.blue / 255.0f;

    result.red      = multiplier.red * b.red;
    result.blue     = multiplier.blue * b.blue;
    result.green    = multiplier.green * b.green;

    return result;
}

Color operator*(const Color &color, float multiplier) {
    Color result;

    result.red = multiplier * color.red;
    result.blue = multiplier * color.blue;
    result.green = multiplier * color.green;

    return result;
}

Color operator*(float multiplier, const Color &color) {
    Color result;

    result.red = multiplier * color.red;
    result.blue = multiplier * color.blue;
    result.green = multiplier * color.green;

    return result;
}
