#pragma once

#include <glm/vec3.hpp>

#include "../../image/Color.h"
#include "../../math/Ray.h"
#include "Object.h"

using glm::vec3;

class Sphere : public Object{
public:
    Sphere(): center(0.0f), radius(1.0f){};
    Sphere(vec3 center, float radius, Material material);
    ~Sphere() override = default;

    HitPayload getIntersection(Ray ray) override;
    bool checkIntersection(Ray ray) override;

private:
    vec3 center;
    float radius;
};
