#pragma once
#include "../../image/Color.h"
#include "../../math/Ray.h"

#include <glm/vec3.hpp>

using glm::vec3;

typedef struct material {
    Color diffuse;
    Color specular;
    Color ambient;

    float shininess;
    float reflectivity;
} Material;

typedef struct hit_payload {
    bool hit = false;
    float hitDistance;
    vec3 position;
    vec3 normal;
    Material material;
    int objectIndex = -1;
} HitPayload;

class Object {
public:
    vec3 position;
    Material material;

    virtual ~Object() = default;

    virtual HitPayload getIntersection(Ray ray) = 0;
    virtual bool checkIntersection(Ray ray) = 0;
};
