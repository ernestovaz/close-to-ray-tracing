#include "Plane.h"

#include <glm/glm.hpp>

using glm::dot;

Plane::Plane(vec3 position, vec3 normal, Material material)
: normal(normal) {
    this->position = position;
    this->material = material;
}

HitPayload Plane::getIntersection(Ray ray) {
    double denominator = dot(normal, ray.direction);
    if(fabs(denominator) > 1e-8f){
        float hitDistance = dot((position - ray.origin), normal) / denominator;
        if(hitDistance > 0.01f) {
            HitPayload payload;
            payload.hit = true;
            payload.hitDistance = hitDistance;
            payload.position = ray.origin + ray.direction * hitDistance;
            payload.normal = normal;
            payload.material = material;

            return payload;
        }
    }
    return {};
}

bool Plane::checkIntersection(Ray ray) {
    double denominator = dot(normal, ray.direction);
    if(fabs(denominator) > 1e-8f) {
        float hitDistance = dot((position - ray.origin), normal) / denominator;
        if (hitDistance > 0.01f)
            return true;
    }
    return false;
}
