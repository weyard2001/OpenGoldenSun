#pragma once

#include <SDL2/SDL.h>

namespace gse {
	struct AnimationComponent final {
		std::vector<std::uint8_t> frames;
		int startFrameX;
		int startFrameY;
		int startTime;
		int curFrame;
		int animationSpeed;

		AnimationComponent(int inStartFrameX = 0, int inStartFrameY = 0, int inCurFrame = 0, int inAnimationSpeed = 1, std::vector<std::uint8_t> inFrames = {})
			: frames(inFrames),
			curFrame(inCurFrame),
			startFrameX(inStartFrameX),
			startFrameY(inStartFrameY),
			startTime(SDL_GetTicks()),
			animationSpeed(inAnimationSpeed) {

		}
	};
}