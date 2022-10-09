#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

using glm::vec3;
using glm::mat4;

class Camera {
public:
    vec3 position;
    vec3 direction;

    Camera();
    void setAspectRatio(float aspect);

    //ray tracing pipeline uses inverse matrix
    mat4 getInverseView();
    mat4 getInverseProjection();

private:

    float fov, near, far, aspectRatio;

public:
    vec3 getPosition();
    vec3 getDirection();
};
