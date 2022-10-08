#pragma once

#include <cinttypes>
#include <glm/vec3.hpp>

using glm::vec3;

class Color {
public:
    static const Color WHITE;
    static const Color PURPLE;
    static const Color GREEN;

    double red;
    double green;
    double blue;

    Color():red(0), green(0), blue(0){}
    Color(double red, double green, double blue);

    void clamp();
    uint32_t toInt() const;

};

Color& operator += (Color& a, const Color& b);

Color operator + (const Color& a, const Color& b);

Color operator * (const Color& a, const Color& b);
Color operator * (const Color& color, float multiplier);
Color operator * (float multiplier, const Color& color);
