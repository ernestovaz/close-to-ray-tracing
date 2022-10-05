#pragma once
#include <vector>
#include <valarray>

#include "Color.h"

using std::vector;
using std::size_t;

class Image {
public:
    int width, height;

    Image(int width, int height);
    Color& at(int x, int y);

private:
    vector<vector<Color>> data;

// proxy subclass for accessing image pixel with brackets
// ex: image[0][0] = Color(0,0,0);
    class PixelRow {
        Image& image;
        size_t row;
    public:
        PixelRow(Image& image, size_t row) : image(image), row(row) {}
        Color& operator[](size_t column) { return image.at(row, column);}
    };
public:
    PixelRow operator[](size_t row) {return PixelRow{*this, row};}

};
