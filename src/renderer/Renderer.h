#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "../image/Image.h"
#include "../image/Color.h"

#include "../scene/Camera.h"
#include "../scene/object/Sphere.h"

using std::vector;
using glm::vec2;
using glm::vec4;
using glm::mat4;

class Renderer {
public:
    Renderer();
    const Image& produceImage(float width, float height);

private:
    float width, height;
    Image image;
    Camera camera;
    vector<Sphere> objects;

    mat4 inverseView;
    mat4 inverseProjection;

    Color traceRay(vec2 pixelCoordinate);
    void addSceneObjects();
    void onResize(int width, int height);
    void updateView();
    void updateProjection();

    //pipeline methods
    vec4 project(vec2 coordinate);
    vec3 perspectiveDivide(vec4 projected);
    vec3 globalPosition(vec3 positionFromCamera);

};
