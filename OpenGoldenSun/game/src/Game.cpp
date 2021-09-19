#include "Game.hpp"

#include <gse/events/Event.hpp>

#include <gse/ecs/entities/Entity.hpp>

#include <gse/ecs/components/Component.hpp>
#include <gse/ecs/components/PlayerComponent.hpp>
#include <gse/ecs/components/TransformComponent.hpp>
#include <gse/ecs/components/VelocityComponent.hpp>
#include <gse/ecs/components/RenderComponent.hpp>
#include <gse/ecs/components/SpriteComponent.hpp>
#include <gse/ecs/components/AnimationComponent.hpp>
#include <gse/ecs/components/CollisionComponent.hpp>
#include <gse/ecs/components/TileEventComponent.hpp>
#include <gse/ecs/components/CameraComponent.hpp>

#include <gse/ecs/systems/System.hpp>
#include <gse/ecs/systems/PlayerSystem.hpp>
#include <gse/ecs/systems/TransformSystem.hpp>
#include <gse/ecs/systems/RenderSystem.hpp>
#include <gse/ecs/systems/AnimationSystem.hpp>
#include <gse/ecs/systems/CollisionSystem.hpp>
#include <gse/ecs/systems/TileEventSystem.hpp>
#include <gse/ecs/systems/CameraSystem.hpp>

#include <SDL2/SDL_image.h>
#ifdef GSE_DEBUG
#include <gse/ecs/components/DebugComponent.hpp>
#include <SDL2/SDL_ttf.h>
#endif // GSE_DEBUG

namespace {
    float deltaTime = 0.f;
}

void Game::init(const char* title, int posX, int posY, int width, int height) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL. Error code: " << SDL_GetError() << '\n';
        return;
    }

    m_window = SDL_CreateWindow(title, posX, posY, width, height, SDL_WINDOW_SHOWN);
    if (!m_window) {
        std::cout << "Error creating game window. Error code: " << SDL_GetError() << '\n';
        return;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
    if (!m_renderer) {
        std::cout << "Error creating renderer. Error code: " << SDL_GetError() << '\n';
        return;
    }

    m_textureResource.setRenderer(m_renderer);

    load("game/assets/maps/madra.tmx");
    run();
}

void Game::run() {
#ifdef GSE_DEBUG
    TTF_Init();
    TTF_Font* font = TTF_OpenFont("game/assets/fonts/Ubuntu-B.ttf", 18);
    if (!font) {
        std::cout << "Error loading TTF file. Error code: " << SDL_GetError() << '\n';
        return;
    }

    int textW, textH;
    SDL_Rect messageRect;
    SDL_Surface* messageSurface = nullptr;
    SDL_Texture* messageTexture = nullptr;

    int averageFPS = 0;
    int frameCounter = 0;
    int prevTime = 0;
#endif // GSE_DEBUG

    Uint32 prevFrameTimeStarted = 0;
    m_running = true;
    while (m_running) {
        // TODO: Better way to achieve framerate-independent movements.
        auto elapsedTime = SDL_GetTicks() - prevFrameTimeStarted;
        if (elapsedTime <= TIME_PER_FRAME) {   // frame finished early before time for next frame. preempt process.
            SDL_Delay(TIME_PER_FRAME - elapsedTime);
        }

        deltaTime = (SDL_GetTicks() - prevFrameTimeStarted) / 1000.f;
        prevFrameTimeStarted = SDL_GetTicks();

        SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
        SDL_RenderClear(m_renderer);

        handleEvents();
        update(deltaTime);
        //draw();   // TODO: don't have draw be a part of update function

#ifdef GSE_DEBUG
        if (SDL_GetTicks() >= prevTime + 1000) {
            prevTime = SDL_GetTicks();
            averageFPS = frameCounter;
            frameCounter = 0;
        }

        messageSurface = TTF_RenderText_Solid(font, ("FPS: " + std::to_string(averageFPS)).c_str(), SDL_Color({ 255, 45, 253 }));
        messageTexture = SDL_CreateTextureFromSurface(m_renderer, messageSurface);
        SDL_QueryTexture(messageTexture, nullptr, nullptr, &textW, &textH);

        messageRect = { 0, 0, textW, textH };
        SDL_RenderCopy(m_renderer, messageTexture, nullptr, &messageRect);
        ++frameCounter;
#endif // GSE_DEBUG

        SDL_RenderPresent(m_renderer);

#ifdef GSE_DEBUG
        SDL_FreeSurface(messageSurface);
        SDL_DestroyTexture(messageTexture);
#endif // GSE_DEBUG
    }

#ifdef GSE_DEBUG
    TTF_CloseFont(font);
    TTF_Quit();
#endif // GSE_DEBUG

    quit();
}

void Game::handleEvents() {
    SDL_Event event;

    while (SDL_PollEvent(&event)) {
        switch (event.type) {
        case SDL_QUIT:
            m_running = false;
            break;
        default:
            break;
        }
    }

    SDL_PumpEvents();
}

void Game::update(float deltaTime) {
    m_playerInput.update();
    m_scene.update(deltaTime);
}

//void Game::draw() {
//    SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 255);
//    SDL_RenderClear(m_renderer);
//
//    SDL_RenderPresent(m_renderer);
//}

void Game::quit() {
    SDL_DestroyRenderer(m_renderer);
    SDL_DestroyWindow(m_window);
    SDL_Quit();
}

void Game::load(const std::string& mapName) {
    // TODO: move to game state when state machine is implemented.
    tmx::Map madraMap;
    if (!madraMap.load(mapName)) {
        return;
    }

    auto& eventBus = m_scene.getEventBus();
    m_scene.addSystem<gse::TileEventSystem>(m_scene, eventBus);
    m_scene.addSystem<gse::CollisionSystem>(m_scene, eventBus, m_renderer);
    m_scene.addSystem<gse::PlayerSystem>(m_scene, eventBus);
    m_scene.addSystem<gse::TransformSystem>(m_scene, eventBus);
    m_scene.addSystem<gse::AnimationSystem>(m_scene, eventBus);
    m_scene.addSystem<gse::CameraSystem>(m_scene, eventBus);
    m_scene.addSystem<gse::RenderSystem>(m_scene, eventBus, m_renderer);

    const auto& layers = madraMap.getLayers();
    const auto& bounds = madraMap.getBounds();
    m_mapBounds = { static_cast<int>(bounds.left), static_cast<int>(bounds.top), static_cast<int>(bounds.width), static_cast<int>(bounds.height) };
    for (const auto& layer : layers) {
        const auto& layerType = layer->getType();
        layerType == tmx::Layer::Type::Tile ? loadTiles(layer, madraMap) : loadObjects(layer, madraMap);
    }
    //

    auto entity = m_scene.createEntity();
    auto entityMainTexture = m_textureResource.getResource("game/assets/entities/isaac.png");
    auto entityShadow = m_textureResource.getResource("game/assets/entities/character_shadow.png");

    int textureW, textureH;
    SDL_QueryTexture(entityMainTexture, nullptr, nullptr, &textureW, &textureH);
    auto entityMainSprite = gse::SpriteData({ entityMainTexture, textureW, textureH, SDL_Rect({ 0, 0, 32, 32 }), glm::ivec2(-8, -22) });

    SDL_QueryTexture(entityShadow, nullptr, nullptr, &textureW, &textureH);
    auto entityShadowSprite = gse::SpriteData({ entityShadow, textureW, textureH, SDL_Rect({ 0, 0, 12, 6 }), glm::ivec2(2, 5) });

    entity.addComponent<gse::PlayerComponent>();
    entity.addComponent<gse::SpriteComponent>(std::map<gse::SpriteComponent::Type, gse::SpriteData>({ { gse::SpriteComponent::Type::Main, entityMainSprite }, { gse::SpriteComponent::Type::Background, entityShadowSprite} }));
    entity.addComponent<gse::TransformComponent>(glm::vec2(305.f, 182.f));
    entity.addComponent<gse::VelocityComponent>();
    entity.addComponent<gse::AnimationComponent>(0, 0, 0, 8);
    entity.addComponent<gse::RenderComponent>(5);

    const auto& transform = entity.getComponent<gse::TransformComponent>();
    entity.addComponent<gse::CollisionComponent>(-1, SDL_Rect({ static_cast<int>(transform.position.x), static_cast<int>(transform.position.y), static_cast<int>(16.f * transform.scale.x), static_cast<int>(16.f * transform.scale.y) }), gse::ColliderType::Circle);
    entity.addComponent<gse::CameraComponent>(0.f, 0.f, static_cast<float>(SDL_GetWindowSurface(m_window)->w), static_cast<float>(SDL_GetWindowSurface(m_window)->h), m_mapBounds);

    m_playerInput.setPlayerEntity(entity);
    m_scene.setActiveCameraEntity(entity);
}

bool Game::running() {
    return m_running;
}

void Game::loadTiles(const tmx::Layer::Ptr& layer, const tmx::Map& map) {
    const auto& tilesets = map.getTilesets();
    const auto& tiles = layer->getLayerAs<tmx::TileLayer>().getTiles();
    const auto& rows = map.getTileCount().y;
    const auto& columns = map.getTileCount().x;

    std::vector<SDL_Texture*> tilesetTextures(tilesets.size());
    for (auto i = 0u; i < tilesets.size(); ++i) {
        SDL_Texture* texture = IMG_LoadTexture(m_renderer, tilesets[i].getImagePath().c_str());
        tilesetTextures[i] = texture;
    }

    for (auto row = 0u; row < rows; ++row) {
        for (auto column = 0u; column < columns; ++column) {
            const auto index = column + (row * columns);
            const auto& tileID = tiles[index].ID;

            if (tileID == 0) {
                continue;
            }

            for (auto i = 0u; i < tilesets.size(); ++i) {   // look for the tileset current tile belongs to
                const auto& tileset = tilesets[i];
                const auto& tilesetFirstGID = tileset.getFirstGID();
                const auto& tilesetLastGID = tileset.getLastGID();

                if (tileID >= tilesetFirstGID && tileID <= tilesetLastGID) {    // tile belongs to current tileset since its ID is between [tilesetFirstGID, tilesetLastGID]
                    const auto& tilesetColumnCount = tileset.getColumnCount();

                    const auto& tilesetTexture = tilesetTextures[i];
                    const auto tileIndex = tileID - tilesetFirstGID;    // normalize tile ID
                    const auto tileRow = tileIndex % tilesetColumnCount;
                    const auto tileColumn = tileIndex / tilesetColumnCount;

                    auto entity = m_scene.createEntity();

                    int tileW, tileH;
                    SDL_QueryTexture(tilesetTexture, nullptr, nullptr, &tileW, &tileH);
                    auto entityTile = gse::SpriteData({ tilesetTexture, tileW, tileH, SDL_Rect({ static_cast<int>(tileRow * 16), static_cast<int>(tileColumn * 16), 16, 16 }) });

                    entity.addComponent<gse::RenderComponent>(layer->getProperties()[0].getIntValue());
                    entity.addComponent<gse::SpriteComponent>(std::map<gse::SpriteComponent::Type, gse::SpriteData>({ { gse::SpriteComponent::Type::Main, entityTile } }));
                    entity.addComponent<gse::TransformComponent>(glm::vec2(column * 16, row * 16));

                    break;
                }
            }
        }
    }
}

void Game::loadObjects(const tmx::Layer::Ptr& layer, const tmx::Map& map) {
    const auto& objectGroup = layer->getLayerAs<tmx::ObjectGroup>();
    const auto& objectGroupProperties = objectGroup.getProperties();
    const auto& objectType = objectGroupProperties[0].getIntValue();
    const auto& objects = objectGroup.getObjects();

    auto& collisionSystem = m_scene.getSystem<gse::CollisionSystem>();
    for (const auto& object : objects) {
        const auto& objectAABB = object.getAABB();
        // TODO: parse flags from Tiled properties to check which components should be added
        auto entity = m_scene.createEntity();

        const auto& objectProperties = object.getProperties();
        const auto& scale = entity.getComponent<gse::TransformComponent>().scale;
        auto objectRect = SDL_Rect({ static_cast<int>(objectAABB.left * scale.x), static_cast<int>(objectAABB.top * scale.x), static_cast<int>(objectAABB.width * scale.x), static_cast<int>(objectAABB.height * scale.y) });
        switch (objectType) {
        case ObjectType::Collision: {
            const auto& collisionIndex = objectGroupProperties[1].getIntValue();
            const auto& collisionType = objectProperties[0].getStringValue();

            if (collisionType == "rectangle") { // TODO: replace with switch and enum
                entity.addComponent<gse::CollisionComponent>(collisionIndex, objectRect);
            }
            else if (collisionType == "circle") {
                entity.addComponent<gse::CollisionComponent>(collisionIndex, objectRect, gse::ColliderType::Circle);
            }
            else {
                const auto& orientation = objectProperties[1].getIntValue();
                entity.addComponent<gse::CollisionComponent>(collisionIndex, objectRect, gse::ColliderType::Triangle, orientation);
            }

            entity.addComponent<gse::TransformComponent>(glm::vec2(objectAABB.left, objectAABB.top));
#ifdef GSE_DEBUG
            entity.addComponent<gse::DebugComponent>(SDL_Color({ 255, 0, 0, 255 }));
            entity.addComponent<gse::RenderComponent>(999);
#endif // GSE_DEBUG

            collisionSystem.insertCollider(collisionIndex, entity);
            break;
        }
        case ObjectType::Event: {
            const auto& eventType = objectProperties[0].getIntValue();
            const auto& newCollisionIndex = objectProperties[1].getIntValue();

            entity.addComponent<gse::TileEventComponent>(objectRect, eventType, newCollisionIndex);
            entity.addComponent<gse::TransformComponent>(glm::vec2(objectAABB.left, objectAABB.top));
#ifdef GSE_DEBUG
            entity.addComponent<gse::DebugComponent>(SDL_Color({ 0, 0, 255, 255 }));
            entity.addComponent<gse::RenderComponent>(999);
#endif // GSE_DEBUG
            break;
        }
        case ObjectType::NPC: {
            const auto& npcType = objectProperties[0].getStringValue();
            const auto& texturePath = objectProperties[1].getStringValue();
            // TODO: create NPCs

            break;
        }
        default:
            break;
        }
    }
}
