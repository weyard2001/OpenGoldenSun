#pragma once

#include <SDL2/SDL.h>

#include <cstdint>

namespace TileEventType {
	enum {
		None = -1,
		CollisionIndexChange, Jump, Climb, Slide, SceneChange, Count
	};
}

namespace gse {
	struct TileEventComponent final {
		using EventType = std::int32_t;

		SDL_Rect eventRect;
		EventType eventType;

		int collisionIndex;	// TODO: not every tile event will change collision index. should move out of this struct.

		TileEventComponent(SDL_Rect inEventRect = { 0, 0, 16, 16 }, EventType inEventType = TileEventType::None, int inCollisionIndex = -1)
			: eventRect(inEventRect),
			eventType((inEventType > TileEventType::None && inEventType < TileEventType::Count) ? inEventType : TileEventType::None),
			collisionIndex(inCollisionIndex) {

		}
	};
}