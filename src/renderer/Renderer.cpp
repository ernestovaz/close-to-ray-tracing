#include "Renderer.h"
#include <glm/glm.hpp>

using glm::vec3;
using glm::dot;
using glm::normalize;

Image Renderer::render(float width, float height) {
    Image image(width, height);
    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            //clip to [0,1]
            vec2 coordinate(x/width, y/height);
            //translate to [-1,1]
            coordinate = coordinate * 2.0f - 1.0f;
            coordinate.x *= width/height;

            image[x][y] = shadePixel(coordinate);
        }
    }
    return image;
}

Color Renderer::shadePixel(vec2 coordinate) {

    float radius = 0.5f; //sphere radius

    //right hand coordinate system
    vec3 direction(coordinate.x, coordinate.y, -1.0f);
    normalize(direction);

    vec3 origin(0.0f, 0.0f, 2.0f);

    //quadratic formula
    float a = dot(direction, direction);
    float b = 2.0f * dot(origin, direction);
    float c = dot(origin, origin) - (radius * radius);

    float discriminant = b * b - 4.0f * a * c;
    if(discriminant >= 0.0f) //has solution
        return {75.0f, 41.0f, 89.0f};
    else return {};
}
