#pragma once

#include "gse/ecs/systems/System.hpp"

namespace gse {
    class TransformSystem final : public System {
    public:
        TransformSystem(Scene& scene, EventBus& eventBus);
        ~TransformSystem() = default;

        void update(const float& deltaTime) override;
    };
}