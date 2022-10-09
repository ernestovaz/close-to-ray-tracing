#pragma once

#include "Object.h"

class Plane : public Object{
public:
    Plane() : position(0.0f), normal(0.0f, 1.0f, 0.0f){}
    Plane(vec3 position, vec3 normal, Material material);

    HitPayload getIntersection(Ray ray) override;
    bool checkIntersection(Ray ray) override;

private:
    vec3 position;
    vec3 normal;
};
