#pragma once

#include "gse/ecs/entities/Entity.hpp"

#include <SDL2/SDL.h>

namespace Input {
	enum {
		None = 0x0,
		Up = 0x1,
		Down = 0x2,
		Left = 0x4,
		Right = 0x8
	};
}

namespace gse {
	class PlayerInput final {
	public:
		PlayerInput() = default;
		~PlayerInput() = default;

		void setPlayerEntity(Entity playerEntity);
		Entity getPlayerEntity() const;

		const Uint8* getKeyState() const;

		void update();

	private:
		const Uint8* m_keyState = SDL_GetKeyboardState(nullptr);
		Entity m_playerEntity;
	};
}