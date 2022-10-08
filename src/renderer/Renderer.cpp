#include "Renderer.h"
#include "../scene/object/Sphere.h"
#include <glm/glm.hpp>

using glm::vec3;
using glm::vec4;
using glm::max;
using glm::min;
using glm::dot;
using glm::normalize;
using glm::sqrt;
using glm::length;

#define DISTANCE_FACTOR 0.1f

Renderer::Renderer() :
    width(480), height(480), image(width, height), camera() {
    updateView();
    updateProjection();
    addSceneObjects();
    addSceneLights();
}

Renderer::~Renderer() {
    for(Object* object : scene)
        delete object;
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

    Object* closestHit = nullptr;
    float closestDistance = FLT_MAX;
    HitPayload hitPayload;

    for(Object* object : scene) {
        HitPayload payload = object->getIntersection(ray);
        if(payload.hit && payload.hitDistance < closestDistance){
            closestDistance = payload.hitDistance;
            closestHit = object;
            hitPayload = payload;
        }
    }
    if(closestHit != nullptr) {
        vec3 position = hitPayload.position;
        vec3 normal = hitPayload.normal;
        Reflectance reflectance = hitPayload.reflectance;

        return applyShading(position, normal, reflectance);
    }
    return {};
}

Color Renderer::applyShading(vec3 position, vec3 normal, Reflectance reflectance) {
    Color shaded = Color();
    for(Light light : lights) {
        vec3 lightDirection = light.position - position;

        float distance = length(lightDirection);
        float attenuation = min(1.0f, 1.0f/(distance * distance * DISTANCE_FACTOR));

        float intensity = dot(normal, normalize(lightDirection));

        Color shading = attenuation * intensity * reflectance.diffuse * light.color;
        shading.clamp();

        shaded += shading;
    }

    shaded.clamp();
    return shaded;
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
    vec3 center;
    float radius;
    Color diffuseReflectance;

    center = vec3(0.0f);
    radius = 0.2f;
    diffuseReflectance = Color::PURPLE;
    Object* purple = new Sphere(center, radius, diffuseReflectance);

    center = vec3(-0.5f, 0.0f, -1.0f);
    radius = 0.2f;
    diffuseReflectance = Color::WHITE;
    Object* white = new Sphere(center, radius, diffuseReflectance);

    scene.push_back(purple);
    scene.push_back(white);
}

void Renderer::addSceneLights() {
    Light spotlight = {Color::PURPLE, camera.getPosition()};
    spotlight.position.y += 3.0f;
    spotlight.position.z += 1.0f;

    lights.push_back(spotlight);
}
