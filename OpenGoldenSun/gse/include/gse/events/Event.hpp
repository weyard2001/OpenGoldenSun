#pragma once

#include "gse/ecs/entities/Entity.hpp"

#include "gse/ecs/components/TileEventComponent.hpp"

#include <SDL2/SDL.h>
#include <iostream>

namespace gse {
	struct Event {
		Event() = default;
		virtual ~Event() = default;
	};

	struct KeyboardEvent final : Event {
		KeyboardEvent(Uint32 inEventType, SDL_Keycode inKeycode)
			: eventType(inEventType),
			keycode(inKeycode) {

		}

		Uint32 eventType;
		SDL_Keycode keycode;
	};

	struct CollisionEvent final : Event {
		CollisionEvent(Entity inEntityA, Entity inEntityB)
			: entityA(inEntityA),
			entityB(inEntityB) {

		}

		~CollisionEvent() = default;

		Entity entityA;
		Entity entityB;
	};

	struct TileEvent final : Event {
		TileEvent(Entity inPlayerEntity, TileEventComponent inEventTileComponent)
			: playerEntity(inPlayerEntity),
			eventTileComponent(inEventTileComponent) {

		}

		~TileEvent() = default;

		Entity playerEntity;
		TileEventComponent eventTileComponent;
	};
}