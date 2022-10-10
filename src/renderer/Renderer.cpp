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
    updateView();

    for(int y=0; y<height; y++){
        for(int x=0; x<width; x++){
            //clip to [0,1]
            vec2 coordinate(x/width, y/height);
            //translate to [-1,1]
            coordinate = coordinate * 2.0f - 1.0f;
            coordinate.x *= width/height;
            vec3 projectedPixel = perspectiveDivide(project(coordinate));
            vec3 direction = globalPosition(projectedPixel);
            Ray ray(camera.getPosition(), direction);

            int recursions = recursionLimit;
            image[x][y] = traceRay(ray, recursions);
        }
    }

    return image;
}

Color Renderer::traceRay(Ray ray, int recursions) {
    Object* closestHit = nullptr;
    float closestDistance = FLT_MAX;
    HitPayload hitPayload;

    for(int i=0; i<sceneObjects.size(); i++){
        Object* object = sceneObjects[i];
        HitPayload payload = object->getIntersection(ray);
        if(i == 3 and recursions < recursionLimit and payload.hit){
            volatile int a = 0;
            a++;
        }
        if(payload.hit && payload.hitDistance < closestDistance && payload.hitDistance >= 0.0f){
            payload.objectIndex = i;
            closestDistance = payload.hitDistance;
            closestHit = object;
            hitPayload = payload;
        }
    }
    if(closestHit != nullptr) {
        return applyShading(hitPayload, recursions);
    }
    return Color::BLACK;
}

Color Renderer::applyShading(HitPayload payload, int recursions) {
    vec3 position = payload.position;
    vec3 normal = payload.normal;
    Material material = payload.material;
    int objectIndex = payload.objectIndex;
    if(objectIndex == 3 and recursions < recursionLimit){
        volatile int a = 3;
        a++;
    }

    Color shaded = Color();
    for(Light light : lights) {

        vec3 lightDirection = light.position - position;
        float distance = length(lightDirection);
        lightDirection = lightDirection/distance;
        Color ambient = light.color * material.ambient;

        vec3 eye = normalize(camera.getPosition() - position);
        vec3 reflection = reflect(-eye, normal);
        Color reflective;
        if(material.reflectivity > 0.0 and recursions > 0){
            recursions--;
            Ray ray(position, reflection);
            reflective = material.reflectivity * traceRay(ray, recursions);
        }

        Ray lightRay(position, lightDirection);
        bool occluded = false;
        for(int i=0; i < sceneObjects.size(); i++){
            if(objectIndex == i) continue;
            if(sceneObjects[i]->checkIntersection(lightRay)) {
                shaded += ambient + reflective;
                occluded = true;
            }
        }
        if(occluded) continue;

        Color diffuse;
        Color specular;
        float attenuation = 0.0f;

        vec3 lightReflection = reflect(-lightDirection, normal);
        if(dot(normal, lightDirection) >= 0){
            attenuation = min(1.0f, 1.0f/(distance * distance * DISTANCE_FACTOR));
            diffuse = material.diffuse *
                      dot(normal, normalize(lightDirection));
            specular = material.specular *
                       pow(max(dot(eye, lightReflection), 0.0f), material.shininess);
        }

        Color shading = reflective + ambient + light.color * attenuation * (diffuse + specular);
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
    Material metallic = {
            Color::DARK_GRAY * 0.1f,
            Color::GRAY,
            Color::BLACK,
            100.0f,
            0.3f
    };

    Material material;
    float radiusBig = 0.3f, radiusSmall = 0.2f;
    vec3 center = vec3(0.0f, 0.0f, -1.0f);
    vec3 left = center, right = center;
    left.x -= 0.5f;
    right.x += 0.5f;
    right.y += 0.1f;
    center.z -= 0.2f;

    material.ambient = Color::GREEN * 0.0f;
    material.diffuse = Color::GREEN;
    material.specular = Color::WHITE;
    material.shininess = 30;
    material.reflectivity = 0.1f;
    Object* green = new Sphere(left, radiusSmall, material);

    Object* metal = new Sphere(center, radiusSmall, metallic);

    material.ambient = Color::PURPLE * 0.0f;
    material.diffuse = Color::PURPLE;
    material.specular = Color::WHITE;
    Object* purple = new Sphere(right, radiusBig, material);

    material.ambient = Color::GRAY * 0.1f;
    material.diffuse = Color::GRAY * 0.3f;
    material.reflectivity = 0.2f;
    Object* plane = new Plane(
            vec3(0.0f, -0.2f, 0.0f),
            vec3(0.0, 1.0f, 0.0f),
            material
    );

    sceneObjects.push_back(green);
    sceneObjects.push_back(purple);
    sceneObjects.push_back(metal);
    sceneObjects.push_back(plane);
}

void Renderer::addSceneLights() {
    Light spotlight = {Color::WHITE, vec3(-0.2f, 1.0f, 0.0f)};
    lights.push_back(spotlight);
    spotlight.position.x += 2.0f;
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
}
