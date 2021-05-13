#pragma once
#include "Labirynt.h"
#include "GameObject.h"
#include "RectangleRenderer.h"
#include "Window.h"

// Zamienia informacje o labiryncie na obiekty w grze i zarz¹dza nimi
class LabyrinthSolidifier
{
public:
	LabyrinthSolidifier(int wallWidth, int wallLength, int xCount, int yCount, const std::list<GameObject*> allObjects);
	~LabyrinthSolidifier();

	int WallsCount() const;
	GameObject** GetWalls() const;

	void ChangeLab();

	static Vector LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount);
private:
	int wallWidth;
	int wallLength;
	int xCount;
	int yCount;

	GameObject** walls; // œciany podzielone na pionowe i poziome
	Labirynt labyrinth;
private:
	void PlaceWalls();
};

