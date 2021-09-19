#pragma once

#include <SDL2/SDL_image.h>

#include <unordered_map>
#include <memory>

namespace gse {
	template <class T>
	class Resource {
	public:
		Resource() = default;
		virtual ~Resource() = default;

		template <typename... Args>
		T* getResource(const std::string& resourcePath, Args&&... args) {
			if (m_resources.find(resourcePath) != m_resources.end()) {
				return m_resources[resourcePath];
			}

			loadResource(resourcePath);
			return m_resources[resourcePath];
		}

	protected:
		std::unordered_map<std::string, T*> m_resources;	// SDL is built using C, so smart pointer is not used as value.

		virtual void loadResource(const std::string& resourcePath) = 0;
	};

	class TextureResource final : public Resource<SDL_Texture> {
	public:
		void setRenderer(SDL_Renderer* renderer) {
			m_renderer = renderer;
		}

	private:
		SDL_Renderer* m_renderer;

		void loadResource(const std::string& resourcePath) override {
			SDL_Texture* texture = IMG_LoadTexture(m_renderer, resourcePath.c_str());
			if (!texture) {
				texture = IMG_LoadTexture(m_renderer, "game/assets/placeholder_texture.png");
			}

			m_resources[resourcePath] = texture;
		}
	};
}