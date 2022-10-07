#include "Renderer.h"
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
using glm::dot;
using glm::normalize;

Renderer::Renderer() :
    width(480), height(480), image(width, height), camera() {
    updateView();
    updateProjection();
}

Image Renderer::render(float width, float height) {

    if(this->width != width || this->height != height){
        onResize(width, height);
    }
    updateProjection();

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            //clip to [0,1]
            vec2 coordinate(x/width, y/height);
            //translate to [-1,1]
            coordinate = coordinate * 2.0f - 1.0f;
            coordinate.x *= width/height;

            image[x][y] = traceRay(coordinate);
        }
    }

    return image;
}

Color Renderer::traceRay(vec2 pixelCoordinate) {
    vec3 projectedPixel = perspectiveDivide(project(pixelCoordinate));

    vec3 rayOrigin = camera.getPosition();
    vec3 rayDirection = globalPosition(projectedPixel);

    float sphereRadius = 0.5f;

    //quadratic formula
    float a = dot(rayDirection, rayDirection);
    float b = 2.0f * dot(rayOrigin, rayDirection);
    float c = dot(rayOrigin, rayOrigin) - (sphereRadius * sphereRadius);

    float discriminant = b * b - 4.0f * a * c;
    if(discriminant >= 0.0f) //has solution
        return {75.0f, 41.0f, 89.0f};
    else return {};
}

void Renderer::onResize(int width, int height) {
    this->width = width;
    this->height = height;
    image.resize(width, height);
    updateView();
}

void Renderer::updateView() {
    inverseView = camera.getInverseView();
}

void Renderer::updateProjection() {
    inverseProjection = camera.getInverseProjection();
}

vec4 Renderer::project(vec2 coordinate) {
    return inverseProjection * vec4(coordinate, 1.0f, 1.0f);
}

vec3 Renderer::perspectiveDivide(vec4 projected) {
    return projected/projected.w;
}

vec3 Renderer::globalPosition(vec3 positionFromCamera) {
    return inverseView * vec4(positionFromCamera, 0.0f);
}
