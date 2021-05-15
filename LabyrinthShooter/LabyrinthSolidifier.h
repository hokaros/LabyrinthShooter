#pragma once
#include "Labirynt.h"
#include "GameObject.h"
#include "RectangleRenderer.h"
#include "Window.h"

// Zamienia informacje o labiryncie na obiekty w grze i zarz¹dza nimi
class LabyrinthSolidifier : public IUpdateable
{
public:
	LabyrinthSolidifier(const Vector& pos,
		int wallWidth, int wallLength,
		int xCount, int yCount,
		const std::list<GameObject*>& allObjects,
		double changeTime
	);
	~LabyrinthSolidifier();

	const Labirynt& GetLab() const;
	int WallsCount() const;
	GameObject** GetWalls() const;
	int BorderElements() const;
	GameObject** GetBorder() const;
	Vector GetSize() const;

	void ChangeLab();

	void Update();

	static Vector LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount);
private:
	Vector position;

	int wallWidth;
	int wallLength;
	int xCount;
	int yCount;

	Labirynt labyrinth;

	const std::list<GameObject*>& allObjects;
	GameObject** walls; // œciany podzielone na pionowe i poziome
	GameObject** border; // czêœci obwódki
	int borderCount;

	int wallColor;
	int gateColor;

	double changeTime;
	double timeSinceLastChange;
private:
	void PlaceWalls();

	GameObject* BuildWall(const Vector& size);
	GameObject* BuildWall(const Vector& size, int color);
	void BuildBorder();
	GameObject** BuildGateWall(Direction side);
};

