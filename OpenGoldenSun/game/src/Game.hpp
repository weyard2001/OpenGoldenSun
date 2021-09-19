#pragma once

#include <gse/ecs/Scene.hpp>
#include <resources/Resource.hpp>
#include <gse/events/PlayerInput.hpp>

#include <SDL2/SDL.h>
#include <glm/glm.hpp>
#include <imgui/imgui_sdl.h>
#include <tmxlite/Map.hpp>
#include <tmxlite/TileLayer.hpp>

#include <iostream>

constexpr int FPS = 60;
constexpr int TIME_PER_FRAME = 1000 / FPS;    // how many milliseconds each frame should take.

namespace ObjectType {
    enum {
        Collision, Event, NPC
    };
}

class Game final {
public:
    Game() = default;
    ~Game() = default;

    void init(const char* title, int posX, int posY, int width, int height);
    void run();
    void handleEvents();
    void update(float deltaTime);
    //void draw();
    void quit();

    // TODO: these methods will be moved to GameState when state machine is implemented
    void load(const std::string& mapName);
    //

    bool running();

private:
    SDL_Window* m_window;
    SDL_Renderer* m_renderer;

    bool m_running;

    // TODO: these variables and methods will be moved to GameState when state machine is implemented
    gse::Scene m_scene;
    gse::TextureResource m_textureResource;
    gse::PlayerInput m_playerInput;

    SDL_Rect m_mapBounds;

    void loadTiles(const tmx::Layer::Ptr& layer, const tmx::Map& map);
    void loadObjects(const tmx::Layer::Ptr& layer, const tmx::Map& map);
};