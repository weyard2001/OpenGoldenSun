#include "gse/events/PlayerInput.hpp"

#include "gse/ecs/Scene.hpp"

#include "gse/ecs/components/CollisionComponent.hpp"
#include "gse/ecs/components/VelocityComponent.hpp"
#include "gse/ecs/components/TransformComponent.hpp"
#include "gse/ecs/components/CameraComponent.hpp"
#include "gse/ecs/components/PlayerComponent.hpp"

#include "gse/ecs/systems/CollisionSystem.hpp"
#include "gse/ecs/systems/PlayerSystem.hpp"

using namespace gse;

CollisionSystem::CollisionSystem(Scene& scene, EventBus& eventBus, SDL_Renderer* renderer)
	: System(scene, eventBus, typeid(CollisionSystem)),
	m_activeCollisionIndex(0),
	m_renderer(renderer),
	m_glanceAngles({ -22.5f, 22.5f, -45.f, 45.f, -67.5f, 67.5f }) {

	requireComponent<CollisionComponent>();
	requireComponent<TransformComponent>();

	getEventBus().listen<TileEvent>(this, &CollisionSystem::tileEventCallback);
}

void CollisionSystem::update(const float& deltaTime) {
	const auto& playerSystem = getScene().getSystem<PlayerSystem>();
	const auto& playerSystemEntities = playerSystem.getEntities();

	std::vector<Entity> characters;	// TODO: not a great idea to create a vector with player and NPCs here (too costly). move elsewhere (same for TileEventSystem update function).
	characters.insert(characters.end(), playerSystemEntities.begin(), playerSystemEntities.end());

	for (auto& character : characters) {	// TODO: currently checks every character on map with every other entity. look for more efficient implementation.
		const auto& characterCollision = character.getComponent<CollisionComponent>();
		auto& characterVelocity = character.getComponent<VelocityComponent>();
		auto& player = character.getComponent<PlayerComponent>();	// TODO: when NPCs are added to game, should check if character has player or npc component.

		for (const auto& entity : m_colliders[m_activeCollisionIndex]) {
			bool tempColliding = false;

			if (entity.getID() == character.getID()) {
				continue;
			}

			const auto& entityCollision = entity.getComponent<CollisionComponent>();
			const auto& entityVelocity = entity.getComponent<VelocityComponent>();

			for (auto i = 0u; i < player.collisionPoints.size(); ++i) {
				bool colliding = isCollidingWithPoint(player.collisionPoints[i], entityCollision.collider.get());

				if (colliding) {
					const auto& playerPointStart = player.collisionStartDegree;
					const auto& circleCollider = *(static_cast<CircleCollider*>(characterCollision.collider.get()));
					const auto& radius = circleCollider.radius;
					const auto oldPlayerInputMask = player.inputMask;
					auto offset = utils::degToRadians(22.5f);
					player.inputMask = Input::None;	// player is colliding. if there's a direction to deflect player towards, player inputmask will be updated again, else it remains None to indicate a collision.
					for (auto glanceIdx = 0u; glanceIdx < m_glanceAngles.size(); ++glanceIdx) {
						const auto& glanceAngle = m_glanceAngles[glanceIdx];
						const auto start = utils::degToRadians(player.collisionStartDegree + glanceAngle);
						for (auto j = 0u; j < player.collisionPoints.size(); ++j) {
							int pointX, pointY;

							if (j % 2 == 0) {
								pointX = static_cast<int>(radius * cos(start + (offset * (j / 2))));
								pointY = static_cast<int>(radius * sin(start + (offset * (j / 2))));
							}
							else {
								pointX = static_cast<int>(radius * cos(start - (offset * (j / 2 + 1))));
								pointY = static_cast<int>(radius * sin(start - (offset * (j / 2 + 1))));
							}

							auto tempPoint = SDL_Point({ radius + pointX + circleCollider.pos.x, radius - pointY + circleCollider.pos.y });

							// if one of the points on the proposed angle to deflect player towards still collides, don't consider it any further.
							tempColliding = isCollidingWithPoint(tempPoint, entityCollision.collider.get());
							if (tempColliding) {
								break;
							}

							// after checking all new points for collision and none of them are colliding, deflect player towards new direction.
							if (j == player.collisionPoints.size() - 1) {
								// update player state and collision points to face new direction that is free of collision.
								player.collisionStartDegree += glanceAngle;
								//player.isGlancing = true;

								auto newPlayerCollisionStart = utils::degToRadians(player.collisionStartDegree);
								pointX = static_cast<int>(radius * cos(newPlayerCollisionStart));
								pointY = static_cast<int>(radius * sin(newPlayerCollisionStart));
								if (oldPlayerInputMask) {
									if (pointX > 0) {
										player.inputMask |= Input::Right;
									}
									else if (pointX < 0) {
										player.inputMask |= Input::Left;
									}

									if (pointY < 0) {
										player.inputMask |= Input::Down;
									}
									else if (pointY > 0) {
										player.inputMask |= Input::Up;
									}
								}
							}
						}

						// current angle can deflect player, no need to check the other angles.
						if (!tempColliding) {
							break;
						}
					}

					break;
				}
			}
		}
	}
}

void CollisionSystem::setActiveCollisionIndex(int collisionIndex) {
	m_activeCollisionIndex = collisionIndex;
}

const int& CollisionSystem::getActiveCollisionIndex() const {
	return m_activeCollisionIndex;
}

void CollisionSystem::insertCollider(std::size_t index, Entity entity) {
	if (index >= m_colliders.size()) {
		m_colliders.resize((index + 1) * 2);
	}

	m_colliders[index].push_back(entity);
}

void CollisionSystem::tileEventCallback(TileEvent& tileEvent) {
	// don't need to destroy the current collision indices' colliders. just update active collision index and compare against the colliders belonging to the same group.
	//for (const auto& entity : getEntities()) {
	//	if (entity.getComponent<CollisionComponent>().collisionIndex == m_activeCollisionIndex && !entity.hasComponent<PlayerComponent>() /* && !entity.hasComponent<NPCComponent>() */) {
	//		getScene().destroyEntity(entity);
	//	}
	//}

	m_activeCollisionIndex = tileEvent.eventTileComponent.collisionIndex;
}

void CollisionSystem::entityAddedCallback(Entity entity) {
	//if (entity.hasComponent<PlayerComponent>() /* || entity.hasComponent<NPCComponent>() */) {
	//	m_characters.push_back(entity);
	//}
}

bool CollisionSystem::isCollidingWithPoint(const SDL_Point& point, Collider* collider) const {
	const auto& collisionType = collider->type;

	switch (collisionType) {
	case ColliderType::Rectangle: {
		const auto& rectCollider = *(static_cast<RectangleCollider*>(collider));
		const auto& colliderPos = collider->pos;
		SDL_Rect colliderRect = { colliderPos.x, colliderPos.y, rectCollider.w, rectCollider.h };

		return SDL_PointInRect(&point, &colliderRect);
	}
	case ColliderType::Circle: {
		const auto& circleCollider = *(static_cast<CircleCollider*>(collider));
		return circlePointColliding(circleCollider, point);
	}
	case ColliderType::Triangle: {
		const auto& triangleCollider = *(static_cast<TriangleCollider*>(collider));
		return trianglePointColliding(triangleCollider, point);
	}
	default:
		return false;
	}
}

bool CollisionSystem::trianglePointColliding(const TriangleCollider& triangle, const SDL_Point& point) const {
	const auto& vertices = triangle.vertices;
	const auto& vertex1 = vertices[0];
	const auto& vertex2 = vertices[1];
	const auto& vertex3 = vertices[2];

	const auto& x1 = vertex1.x, x2 = vertex2.x, x3 = vertex3.x;
	const auto& y1 = vertex1.y, y2 = vertex2.y, y3 = vertex3.y;
	const auto& pX = point.x, pY = point.y;

	auto area = abs(((x2 - x1) * (y3 - y1) - (x3 - x1) * (y2 - y1)) / 2.f);
	auto dummyArea1 = abs(((x1 - pX) * (y2 - pY) - (x2 - pX) * (y1 - pY)) / 2.f);
	auto dummyArea2 = abs(((x2 - pX) * (y3 - pY) - (x3 - pX) * (y2 - pY)) / 2.f);
	auto dummyArea3 = abs(((x3 - pX) * (y1 - pY) - (x1 - pX) * (y3 - pY)) / 2.f);

	return dummyArea1 + dummyArea2 + dummyArea3 == area;
}

bool CollisionSystem::circlePointColliding(const CircleCollider& circle, const SDL_Point& point) const {
	const glm::ivec2 circleRadiusPos = { circle.pos.x + circle.radius, circle.pos.y + circle.radius };
	const auto a = point.x - circleRadiusPos.x;
	const auto b = point.y - circleRadiusPos.y;

	return sqrt(a * a + b * b) < circle.radius;
}
