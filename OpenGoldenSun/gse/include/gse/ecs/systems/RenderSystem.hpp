#pragma once

#include "gse/ecs/systems/System.hpp"

#include <SDL2/SDL.h>

namespace gse {
	class RenderSystem : public System {
	public:
		RenderSystem(Scene& scene, EventBus& eventBus, SDL_Renderer* renderer);
		~RenderSystem() = default;

		void update(const float& deltaTime) override;

	protected:
		void entityAddedCallback(Entity entity) override;

	private:
		SDL_Renderer* m_renderer;
		bool m_sort;

		void draw();	// TODO: Redo Game.cpp to have draw be separate from update. Currently main loop just calls update and draw is called from update.
	};
}