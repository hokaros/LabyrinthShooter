#pragma once
#include "Window.h"
#include "Vector.h"

// Element wy�wietlaj�cy pewn� statystyk� (zdrowie / liczb� graczy itp.)
class StatRenderer
{
public:
	virtual void Render() = 0;

	void UpdateStat(int newStat);

protected:
	int stat = 0;
};

// Element reprezentuj�cy statystyk�, mno��c elementy graficzne (wy�wietlanie serc itp.)
class BMPStats : public StatRenderer 
{
public:
	BMPStats(SDL_Surface* bitmap, const VectorInt& elementSize, const VectorInt& startPosition);

	void Render() override;

private:
	SDL_Surface* bitmap;
	VectorInt elementSize;
	VectorInt startPos;
};

