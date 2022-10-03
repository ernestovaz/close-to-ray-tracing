#include "Image.h"

Image::Image(int width, int height)
: width(width), height(height),
data(width, vector<Color>(height, Color()))
//initialize black image
{}

Color& Image::at(int x, int y) {
    return data[x][y];
}
