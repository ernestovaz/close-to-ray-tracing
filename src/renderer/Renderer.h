#pragma once

#include <vector>
#include <glm/vec2.hpp>
#include <glm/vec4.hpp>
#include <glm/mat4x4.hpp>

#include "../image/Image.h"
#include "../image/Color.h"

#include "../scene/Camera.h"
#include "../scene/object/Object.h"

using std::vector;
using glm::vec2;
using glm::vec4;
using glm::mat4;

typedef struct light {
    Color color;
    vec3 position;
} Light;

class Renderer {
public:
    Renderer();
    ~Renderer();
    const Image& produceImage(float width, float height);

private:
    float width, height;
    Image image;
    Camera camera;

    vector<Object*> scene;
    vector<Light> lights;

    mat4 inverseView;
    mat4 inverseProjection;

    Color traceRay(vec2 pixelCoordinate);
    Color applyShading(vec3 position, vec3 normal, Reflectance reflectance);

    void addSceneObjects();
    void addSceneLights();

    void onResize(int width, int height);
    void updateView();
    void updateProjection();

    //pipeline methods
    vec4 project(vec2 coordinate);
    vec3 perspectiveDivide(vec4 projected);
    vec3 globalPosition(vec3 positionFromCamera);
};
