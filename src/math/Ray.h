#pragma once

#include <glm/vec3.hpp>

using glm::vec3;

class Ray {
public:
    vec3 origin;
    vec3 direction;

    Ray(const vec3 &origin, const vec3 &direction);
};
