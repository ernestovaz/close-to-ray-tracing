#pragma once

#include <glm/vec3.hpp>

#include "../../image/Color.h"
#include "../../math/Ray.h"

using glm::vec3;

class Sphere {
public:
    vec3 center;
    float radius;
    Color diffuseReflectance;

    Sphere(): center(0.0f), radius(1.0f){};
    Sphere(vec3 center, float radius, Color diffuse);
    float getIntersection(Ray ray);
};
