#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

#define ORIGIN    vec3(0.0f)
#define UP_VECTOR vec3(0.0f, 1.0f, 0.0f)

using glm::lookAt;
using glm::perspective;
using glm::inverse;

vec3 Camera::getPosition() {
    return position;
}

vec3 Camera::getDirection() {
    return direction;
}

Camera::Camera() :
    position(vec3(0.0f, 0.0f, 1.0f)),
    direction(vec3(0.0f, 0.0f, -1.0f)),
    fov(45.0f),
    near(0.1f),
    far(20.0f),
    aspectRatio(1.0f) {}

void Camera::setAspectRatio(float aspect) {
    this->aspectRatio = aspect;
}

mat4 Camera::getInverseView() {
    vec3 center = ORIGIN + direction;
    mat4 view = lookAt(position, center, UP_VECTOR);
    return inverse(view);
}

mat4 Camera::getInverseProjection() {
    mat4 projection = perspective(fov, aspectRatio, near, far);
    return inverse(projection);
}

