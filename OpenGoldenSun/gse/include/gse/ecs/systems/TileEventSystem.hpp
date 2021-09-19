#pragma once

#include "gse/ecs/systems/System.hpp"

namespace gse {
	class TileEventSystem final : public System {
	public:
		TileEventSystem(Scene& scene, EventBus& eventBus);
		~TileEventSystem() = default;

		void update(const float& deltaTime) override;

	protected:
		void entityAddedCallback(Entity entity) override;
		void entityRemovedCallback(Entity entity) override;

	private:
		bool eventActive(const std::unique_ptr<Collider>& entityCollider, const SDL_Rect& eventTileRect);
	};
}