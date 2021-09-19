#pragma once

#include "utilities/Shapes.hpp"

#include "gse/ecs/components/CollisionComponent.hpp"

#include <array>

namespace gse {
	struct PlayerComponent final {
		enum class State {
			None, Idle, Walk, Sprint
		};

		enum class Direction {
			None = 0x0,
			Down = 0x2, DownLeft = 0x6, Left = 0x4, UpLeft = 0x5,
			Up = 0x1, UpRight = 0x9, Right = 0x8, DownRight = 0x10
		};

		std::uint8_t inputMask;
		State state;
		State oldState;
		Direction direction;
		Direction oldDirection;
		std::array<SDL_Point, 7u> collisionPoints;

		float collisionStartDegree;
		bool isGlancing;

		PlayerComponent(std::uint8_t inInputMask = 0)
			: inputMask(inInputMask),
			state(State::Idle),
			oldState(State::None),
			direction(Direction::Down),
			oldDirection(Direction::Down),
			collisionPoints({}),
			collisionStartDegree(270.f),
			isGlancing(false) {

		}

		void updateCollisionPoints(const CircleCollider& circleCollider) {
			const auto& radius = circleCollider.radius;
			auto centerPoint = utils::degToRadians(collisionStartDegree);
			auto offset = utils::degToRadians(22.5f);

			for (auto i = 0u; i < collisionPoints.size(); ++i) {
				int pointX, pointY;

				if (i % 2 == 0) {
					pointX = static_cast<int>(radius * cos(centerPoint + (offset * (i / 2))));
					pointY = static_cast<int>(radius * sin(centerPoint + (offset * (i / 2))));
				}
				else {
					pointX = static_cast<int>(radius * cos(centerPoint - (offset * (i / 2 + 1))));
					pointY = static_cast<int>(radius * sin(centerPoint - (offset * (i / 2 + 1))));
				}

				collisionPoints[i] = { radius + pointX + circleCollider.pos.x, radius - pointY + circleCollider.pos.y };
			}
		}
	};
}