#pragma once

//#include "gse/ecs/components/SpriteComponent.hpp"

#include <string>
#include <unordered_map>

namespace gse {
	class SpriteSheet final {
	public:
		SpriteSheet() = default;
		~SpriteSheet() = default;

	private:
		std::string m_resourcePath;

		//std::unordered_map<std::string, SpriteComponent> m_sprites;
	};
}