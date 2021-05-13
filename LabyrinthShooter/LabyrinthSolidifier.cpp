#include "LabyrinthSolidifier.h"

LabyrinthSolidifier::LabyrinthSolidifier(int wallWidth, int wallLength, int xCount, int yCount, const std::list<GameObject*> allObjects)
	: wallWidth(wallWidth), wallLength(wallLength), xCount(xCount), yCount(yCount),
	labyrinth(xCount, yCount) {

	SDL_Surface* screen = Window::Main()->GetScreen();
	int blue = SDL_MapRGB(screen->format, 0x00, 0x00, 0xAA);

	walls = new GameObject * [labyrinth.ActiveCount()];
	for (int i = 0; i < labyrinth.ActiveCount(); i++) {
		// Stworzenie pocz¹tkowo pionowych œcian
		walls[i] = new GameObject(Vector(wallWidth, wallLength), allObjects);
		walls[i]->AddComponent(new RectangleRenderer(*(walls[i]), screen, blue, blue));
	}

	PlaceWalls();

	labyrinth.PrintLab();
}

LabyrinthSolidifier::~LabyrinthSolidifier() {
	for (int i = 0; i < labyrinth.ActiveCount(); i++) {
		delete walls[i];
	}

	delete[] walls;
}

int LabyrinthSolidifier::WallsCount() const {
	return labyrinth.ActiveCount();
}

GameObject** LabyrinthSolidifier::GetWalls() const {
	return walls;
}

Vector LabyrinthSolidifier::LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount) {
	return Vector(wallLength * xCount, wallLength * yCount);
}

void LabyrinthSolidifier::PlaceWalls() {
	int nextWall = 0;
	Vector verticalWall(wallWidth, wallLength);
	Vector horizontalWall(wallLength, wallWidth);

	// Œciany pionowe
	for (int y = 0; y < yCount; y++) {
		for (int x = 1; x <= xCount - 1; x++) {
			int index = (xCount - 1) * y + x - 1;
			if (labyrinth[index]) {
				// Obecna œciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(verticalWall);
				wall->ForceSetPosition(Vector(x * wallLength, y * wallLength));
				wall->BumpOut();
			}
		}
	}

	int horStart = labyrinth.VerticalCount();
	// Œciany poziome
	for (int y = 1; y <= yCount - 1; y++) {
		for (int x = 0; x < xCount; x++) {
			int index = horStart + xCount * (y - 1) + x;
			if (labyrinth[index]) {
				// Obecna œciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(horizontalWall);
				wall->ForceSetPosition(Vector(x * wallLength, y * wallLength));
				wall->BumpOut();
			}
		}
	}
}

void LabyrinthSolidifier::ChangeLab() {
	labyrinth.ChangeLab();
	PlaceWalls();
}