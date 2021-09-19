#pragma once

#include <glm/glm.hpp>

namespace gse {
    struct TransformComponent final {
        glm::vec2 position;
        glm::vec2 scale;
        float rotation;

        TransformComponent(glm::vec2 inPosition = glm::vec2(0.f, 0.f), glm::vec2 inScale = glm::vec2(3.25f, 3.25f), float inRotation = 0.f)
            : position(inPosition),
            scale(inScale),
            rotation(inRotation) {

        }
    };
}