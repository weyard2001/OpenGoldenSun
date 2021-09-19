#pragma once

namespace gse {
	struct RenderComponent final {
		int zIndex;	// TODO: dynamic z-indexing based on player position. e.g., important when under/above a bridge. maybe only change player's z-index?

		RenderComponent(int inZIndex = 0)
			: zIndex(inZIndex) {

		}
	};
}