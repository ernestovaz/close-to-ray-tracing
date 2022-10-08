#pragma once
#include "../../image/Color.h"
#include "../../math/Ray.h"

#include <glm/vec3.hpp>

using glm::vec3;

typedef struct reflectance {
    Color diffuse;
    Color specular;
} Reflectance;

typedef struct hit_payload {
    bool hit = false;
    float hitDistance;
    vec3 position;
    vec3 normal;
    Reflectance reflectance;
} HitPayload;

class Object {
public:
    Reflectance reflectance;

    virtual ~Object() = default;
    virtual HitPayload getIntersection(Ray ray) = 0;
};
