#pragma once

#include <cinttypes>

class Color {
public:
    double red;
    double green;
    double blue;

    Color() = default;
    Color(double red, double green, double blue);
    uint32_t toInt() const;
};
