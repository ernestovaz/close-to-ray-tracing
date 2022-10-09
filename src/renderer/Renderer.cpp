#include "Renderer.h"
#include "../scene/object/Sphere.h"
#include "../scene/object/Plane.h"
#include <glm/glm.hpp>
#include <iostream>

using glm::vec3;
using glm::vec4;
using glm::max;
using glm::min;
using glm::dot;
using glm::normalize;
using glm::reflect;
using glm::pow;
using glm::sqrt;
using glm::length;

#define DISTANCE_FACTOR 0.1f

Renderer::Renderer() :
    image(480, 480), camera() {
    updateView();
    updateProjection();
    addSceneObjects();
    addSceneLights();
}

Renderer::~Renderer() {
    for(Object* object : sceneObjects)
        delete object;
}

const Image& Renderer::produceImage(float imageWidth, float imageHeight) {
    float width = floor(imageWidth * scale.x);
    float height = floor(imageHeight * scale.y);
    if(image.width != width || image.height != height){
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

    for(int i=0; i<sceneObjects.size(); i++){

        Object* object = sceneObjects[i];
        HitPayload payload = object->getIntersection(ray);
        if(payload.hit && payload.hitDistance < closestDistance){
            payload.objectIndex = i;
            closestDistance = payload.hitDistance;
            closestHit = object;
            hitPayload = payload;
        }
    }
    if(closestHit != nullptr) {
        return applyShading(hitPayload);
    }
    return {};
}

Color Renderer::applyShading(HitPayload payload) {
    vec3 position = payload.position;
    vec3 normal = payload.normal;
    Material material = payload.material;
    int objectIndex = payload.objectIndex;

    Color shaded = Color();
    for(Light light : lights) {

        vec3 lightDirection = light.position - position;

        float distance = length(lightDirection);
        lightDirection = lightDirection/distance;

        bool occluded = false;
        Ray lightRay(position, lightDirection);
        for(int i=0; i < sceneObjects.size() - 1; i++){
            if(objectIndex == i) continue;
            occluded = sceneObjects[i]->checkIntersection(lightRay);
            if(occluded) break;
        }
        if(occluded) continue;

        vec3 eye = normalize(camera.getPosition() - position);
        vec3 reflection = reflect(-lightDirection, normal);

        float attenuation = min(1.0f, 1.0f/(distance * distance * DISTANCE_FACTOR));

        Color ambient = light.color * material.ambient;
        Color diffuse = material.diffuse *
                dot(normal, normalize(lightDirection));
        Color specular = material.specular *
                pow(max(dot(eye, reflection), 0.0f), material.shininess);

        Color shading = ambient + light.color * attenuation * (diffuse + specular);
        shading.clamp();

        shaded += shading;
    }

    shaded.clamp();
    return shaded;
}

void Renderer::onResize(int width, int height) {
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
    Material material;

    center = vec3(0.2f, 0.1f, -1.0f);
    radius = 0.3f;

    material.ambient = Color::PURPLE * 0.5f;
    material.diffuse = Color::PURPLE;
    material.specular = Color::WHITE;
    material.shininess = 32;
    Object* purple = new Sphere(center, radius, material);

    center = vec3(-0.35f, 0.0f, -1.0f);
    radius = 0.2f;
    material.ambient = Color::GREEN * 0.5f;
    material.diffuse = Color::GREEN;
    Object* white = new Sphere(center, radius, material);

    material.ambient = Color::GRAY * 0.2f;
    material.diffuse = Color::GRAY;
    Object* plane = new Plane(
            vec3(0.0f, -0.2f, 0.0f),
            vec3(0.0, 1.0f, 0.0f),
            material
    );

    sceneObjects.push_back(purple);
    sceneObjects.push_back(white);
    sceneObjects.push_back(plane);
}

void Renderer::addSceneLights() {
    Light spotlight = {Color::WHITE, vec3(1.5f, 2.0f, -0.5f)};

    lights.push_back(spotlight);
}

vector<Object *> &Renderer::getScene() {
    return sceneObjects;
}

Camera &Renderer::getCamera() {
    return camera;
}

void Renderer::setCameraDirection(vec3 direction) {
    camera.direction = direction;
    updateView();
}
