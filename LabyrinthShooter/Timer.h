#pragma once
#include <time.h>

class Timer
{
public:
	Timer();
	~Timer();
	// Aktualizuje deltaTime. Nale¿y wywo³aæ na pocz¹tku nowej klatki
	void NextFrame();
	// Zwraca czas od ostatniej klatki [s]
	double GetDeltaTime() const;
	// Zatrzymuje up³yw czasu
	void Pause();
	// Odblokowuje up³yw czasu
	void Unpause();

	static Timer* Main();
private:
	clock_t lastFrameTime = 0;
	double deltaTime = 0.0f;

	bool paused = false;
	clock_t pauseTime = 0;

	static Timer* main;
};

