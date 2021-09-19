#pragma once

#include "gse/ecs/entities/Entity.hpp"

#include <SDL2/SDL.h>
#include <glm/glm.hpp>

#include <array>
#include <unordered_set>
#include <memory>

namespace gse {
	enum class ColliderType {
		Rectangle, Circle, Triangle
	};

	//struct CollisionData final {
	//	Entity::ID other;
	//};

	struct Collider {
		glm::ivec2 pos;
		ColliderType type;
		
		//glm::ivec2 scaledPos;	// TODO: make this account for the scaling and make pos be the pos without scaling taken into effect. also need to set entity's size dimensions here when camera zooming is added.

		Collider(int inX, int inY, ColliderType inType)
			: pos({ inX, inY }),
			type(inType) {

		}

		virtual void updatePos(int newX, int newY) {
			pos = { newX, newY };
		}
	};

	struct RectangleCollider final : Collider {
		int w, h;

		RectangleCollider(SDL_Rect inAABB)
			: Collider(inAABB.x, inAABB.y, ColliderType::Rectangle),
			w(inAABB.w),
			h(inAABB.h) {

		}
	};

	struct CircleCollider final : Collider {
		int radius;

		CircleCollider(SDL_Rect inAABB, int inRadius)
			: Collider(inAABB.x, inAABB.y, ColliderType::Circle),
			radius(inRadius) {

		}
	};

	struct TriangleCollider final : Collider {
		int orientation;
		int oppAndAdjLength;

		std::array<glm::ivec2, 3u> vertices;

		TriangleCollider(SDL_Rect inAABB, int inOrientation)
			: Collider(inAABB.x, inAABB.y, ColliderType::Triangle),
			orientation(inOrientation),
			oppAndAdjLength(inAABB.w) {

			updateVertices(inAABB.x, inAABB.y);
		}

		void updateVertices(int x, int y) {
			auto topLeftVertex = glm::ivec2(x, y);
			auto topRightVertex = glm::ivec2(x + oppAndAdjLength, y);
			auto bottomLeftVertex = glm::ivec2(x, y + oppAndAdjLength);
			auto bottomRightVertex = glm::ivec2(x + oppAndAdjLength, y + oppAndAdjLength);

			switch (orientation) {
			case 0:
				vertices = { topLeftVertex, topRightVertex, bottomRightVertex };
				break;
			case 1:
				vertices = { topRightVertex, bottomRightVertex, bottomLeftVertex };
				break;
			case 2:
				vertices = { bottomRightVertex, bottomLeftVertex, topLeftVertex };
				break;
			case 3:
				vertices = { bottomLeftVertex, topLeftVertex, topRightVertex };
				break;
			}
		}

		void updatePos(int newX, int newY) override {
			updateVertices(newX, newY);
			Collider::updatePos(newX, newY);
		}
	};

	struct CollisionComponent final {
		static constexpr std::uint8_t MAX_COLLISIONS = 8;

		std::unique_ptr<Collider> collider;
		std::unordered_set<Entity::ID> data;

		int collisionIndex;

		CollisionComponent(int inCollisionIndex = -1, SDL_Rect inAABB = { 0, 0, 16, 16 }, ColliderType inType = ColliderType::Rectangle, int inOrientation = -1) 
			: collisionIndex(inCollisionIndex) {

			if (inType == ColliderType::Triangle && inOrientation == -1) {
				inOrientation = 0;
				// TODO: log warn no default orientation was provided for inType triangle, defaulting to 0.
			}

			switch (inType) {
			case ColliderType::Rectangle:
				collider = std::make_unique<RectangleCollider>(inAABB);
				break;
			case ColliderType::Circle:
				collider = std::make_unique<CircleCollider>(inAABB, inAABB.w / 2);
				break;
			case ColliderType::Triangle:
				collider = std::make_unique<TriangleCollider>(inAABB, inOrientation);
				break;
			default:
				break;
			}
		}
	};
}
