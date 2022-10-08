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
    addSceneObjects();
}

const Image& Renderer::produceImage(float width, float height) {

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
    vec3 direction = globalPosition(projectedPixel);

    Ray ray(camera.getPosition(), direction);

    Sphere* closestHit = nullptr;
    float closestDistance = FLT_MAX;
    for(Sphere& sphere : objects) {
        float hit = sphere.getIntersection(ray);
        if(hit >= 0 && hit < closestDistance){
            closestDistance = hit;
            closestHit = &sphere;
        }
    }
    if(closestHit != nullptr) {
        return closestHit->diffuseReflectance;
    }
    return {};
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

void Renderer::addSceneObjects() {
    Sphere purple;
    purple.center = vec3(0.0f);
    purple.radius = 0.2f;
    purple.diffuseReflectance = Color(75.0f, 41.0f, 89.0f);

    Sphere green;
    green.center = vec3(-0.5f, 0.0f, -1.0f);
    green.radius = 0.2f;
    green.diffuseReflectance = Color(0.0f, 75.0f, 41.0f);

    objects.push_back(purple);
    objects.push_back(green);
}
