#pragma once
#include <time.h>
#include <functional>
#include <vector>

using std::pair;
using std::function;
using std::vector;

struct FunctionInvokeArgs {
	function<void()> fun;
	double time;
};

class InvokeQueue {
public:
	void Add(function<void()> action, double time);

	size_t GetSize() const;
	double ClosestTime() const;
	function<void()> PopFirst();

	void UpdateTimes(double dTime);
private:
	vector<FunctionInvokeArgs> items;
};

class Timer
{
public:
	Timer();
	~Timer();
	// Aktualizuje deltaTime. Nale�y wywo�a� na pocz�tku nowej klatki
	void NextFrame();
	// Zwraca czas od ostatniej klatki [s]
	double GetDeltaTime() const;
	// Zatrzymuje up�yw czasu
	void Pause();
	// Odblokowuje up�yw czasu
	void Unpause();

	// Umo�liwia wykonanie funkcji po pewnym czasie
	void InvokeOnNextFrame(function<void()> action, double time);
	// Wykonuje funkcje, kt�rych czas nadszed�
	void InvokeTimed();

	static Timer* Main();

private:
	clock_t lastFrameTime = 0;
	double deltaTime = 0.0f;

	bool paused = false;
	clock_t pauseTime = 0;

	InvokeQueue invokeQueue;

	static Timer* main;
};

