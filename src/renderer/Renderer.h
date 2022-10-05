#pragma once
#include <glm/vec2.hpp>

#include "../image/Image.h"
#include "../image/Color.h"

using glm::vec2;

class Renderer {
public:
    Image render(float width, float height);

private:
    Color shadePixel(vec2 coordinate);
};
