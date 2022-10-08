#include "Sphere.h"

#include <glm/glm.hpp>

using glm::dot;
using glm::sqrt;

Sphere::Sphere(vec3 center, float radius, Color diffuse)
: center(center), radius(radius), diffuseReflectance(diffuse) {}

float Sphere::getIntersection(Ray ray) {
    vec3 direction = ray.direction;
    vec3 origin = ray.origin - center;

    //quadratic formula
    float a = dot(direction, direction);
    float b = 2.0f * dot(origin, direction);
    float c = dot(origin, origin) - (radius * radius);

    float discriminant = b * b - 4.0f * a * c;
    if(discriminant < 0.0f)
        return -1; // no solution

    float closestHit = (-b - sqrt(discriminant))/(2.0f * a);
    return closestHit;
}
