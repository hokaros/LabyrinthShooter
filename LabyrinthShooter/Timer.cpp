#include "Timer.h"

Timer* Timer::main{ NULL };

Timer* Timer::Main() {
	return main;
}

Timer::Timer() : paused(false) {
	if (main == NULL) {
		main = this;
	}

	lastFrameTime = clock();
}

Timer::~Timer() {
	if (main == this) {
		main = NULL;
	}
}

void Timer::NextFrame() {
	clock_t currentTime = clock();

	clock_t elapsedClocks = currentTime - lastFrameTime;
	if (paused) {
		elapsedClocks -= currentTime - pauseTime;
	}

	deltaTime = ((double)elapsedClocks) / CLOCKS_PER_SEC;

	lastFrameTime = currentTime;
}

double Timer::GetDeltaTime() const {
	return deltaTime;
}

void Timer::Pause() {
	paused = true;
	pauseTime = clock();
}

void Timer::Unpause() {
	paused = false;
}
