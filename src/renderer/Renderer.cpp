#include "Renderer.h"

Image Renderer::render() {
    Image image(480, 480);
    for(int x=0; x<480; x++){
        for(int y=0; y<480; y++){
            image[x][y].red = (static_cast<double>(x)/480.0) * 255;
            image[x][y].green = (static_cast<double>(y)/480.0) * 255;
        }
    }
    return image;
}
