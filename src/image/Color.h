#pragma once

#include <cinttypes>

class Color {
public:
    double red;
    double green;
    double blue;

    Color():red(0), green(0), blue(0){}
    Color(double red, double green, double blue);
    uint32_t toInt() const;
};
