#include "Sphere.h"

#include <glm/glm.hpp>

using glm::dot;
using glm::sqrt;
using glm::normalize;

Sphere::Sphere(vec3 center, float radius, Color diffuse)
: center(center), radius(radius){
    material.diffuse = diffuse;
}

HitPayload Sphere::getIntersection(Ray ray) {
    vec3 direction = ray.direction;
    vec3 origin = ray.origin - center;

    //quadratic formula
    float a = dot(direction, direction);
    float b = 2.0f * dot(origin, direction);
    float c = dot(origin, origin) - (radius * radius);

    float discriminant = b * b - 4.0f * a * c;
    if(discriminant < 0.0f)
        return {}; // no solution

    float closestHit = (-b - sqrt(discriminant))/(2.0f * a);
    vec3 surfacePosition = origin + ray.direction * closestHit;

    HitPayload payload;
    payload.hit = true;
    payload.hitDistance = closestHit;
    payload.position = surfacePosition + center;
    payload.normal = normalize(surfacePosition);
    payload.material = material;

    return payload;
}

bool Sphere::checkIntersection(Ray ray) {
    vec3 direction = ray.direction;
    vec3 origin = ray.origin - center;

    //quadratic formula
    float a = dot(direction, direction);
    float b = 2.0f * dot(origin, direction);
    float c = dot(origin, origin) - (radius * radius);

    float discriminant = b * b - 4.0f * a * c;
    return (-b - sqrt(discriminant))/(2.0f * a) >= 0;
}
