#include "LabyrinthSolidifier.h"

LabyrinthSolidifier::LabyrinthSolidifier(const Vector& pos,
	int wallWidth, int wallLength,
	int xCount, int yCount,
	const std::list<GameObject*>& allObjects,
	double changeTime)
	: position(pos), wallWidth(wallWidth), wallLength(wallLength),
	xCount(xCount), yCount(yCount),
	labyrinth(xCount, yCount), allObjects(allObjects),
	changeTime(changeTime)
	{

	SDL_Surface* screen = Window::Main()->GetScreen();
	wallColor = SDL_MapRGB(screen->format, 0x00, 0x00, 0xAA);
	gateColor = SDL_MapRGB(screen->format, 0x00, 0xCC, 0xAA);

	// Stworzenie �cian
	walls = new GameObject * [labyrinth.ActiveCount()];
	for (int i = 0; i < labyrinth.ActiveCount(); i++) {
		walls[i] = BuildWall(Vector(wallWidth, wallLength));
	}

	PlaceWalls(); // wstawienie �cian w odpowiednie miejsca
	BuildBorder();

	labyrinth.PrintLab();
}

LabyrinthSolidifier::~LabyrinthSolidifier() {
	for (int i = 0; i < labyrinth.ActiveCount(); i++) {
		delete walls[i];
	}
	delete[] walls;

	for (int i = 0; i < borderCount; i++) {
		delete border[i];
	}
	delete[] border;
}

const Labirynt& LabyrinthSolidifier::GetLab() const {
	return labyrinth;
}

int LabyrinthSolidifier::WallsCount() const {
	return labyrinth.ActiveCount();
}

GameObject** LabyrinthSolidifier::GetWalls() const {
	return walls;
}

int LabyrinthSolidifier::BorderElements() const {
	return borderCount;
}

GameObject** LabyrinthSolidifier::GetBorder() const {
	return border;
}

Vector LabyrinthSolidifier::GetSize() const {
	return LabyrinthSize(wallWidth, wallLength, xCount, yCount);
}

Vector LabyrinthSolidifier::LabyrinthSize(int wallWidth, int wallLength, int xCount, int yCount) {
	return Vector(wallLength * xCount, wallLength * yCount);
}

void LabyrinthSolidifier::PlaceWalls() {
	int nextWall = 0;
	Vector verticalWall(wallWidth, wallLength);
	Vector horizontalWall(wallLength, wallWidth);

	// �ciany pionowe
	for (int y = 0; y < yCount; y++) {
		for (int x = 1; x <= xCount - 1; x++) {
			int index = (xCount - 1) * y + x - 1;
			if (labyrinth[index]) {
				// Obecna �ciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(verticalWall);
				wall->ForceSetPosition(Vector(x * wallLength, y * wallLength) + position);
				wall->BumpOut();
			}
		}
	}

	int horStart = labyrinth.VerticalCount();
	// �ciany poziome
	for (int y = 1; y <= yCount - 1; y++) {
		for (int x = 0; x < xCount; x++) {
			int index = horStart + xCount * (y - 1) + x;
			if (labyrinth[index]) {
				// Obecna �ciana
				GameObject* wall = walls[nextWall++];
				wall->SetSize(horizontalWall);
				wall->ForceSetPosition(Vector(x * wallLength, y * wallLength) + position);
				wall->BumpOut();
			}
		}
	}
}

void LabyrinthSolidifier::ChangeLab() {
	labyrinth.ChangeLab();
	PlaceWalls();
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size) {
	return BuildWall(size, wallColor);
}

GameObject* LabyrinthSolidifier::BuildWall(const Vector& size, int color) {
	GameObject* wall = new GameObject(size, allObjects);
	wall->AddComponent(new RectangleRenderer(*wall, Window::Main()->GetScreen(), color, color));
	return wall;
}

void LabyrinthSolidifier::BuildBorder() {
	borderCount = 8;  // 2* przedziurawione �ciany + zwyk�e �ciany + drzwi
	border = new GameObject * [borderCount];

	GameObject** newBorder = BuildGateWall(Direction::EAST);
	memcpy(border, newBorder, sizeof(GameObject*) * 3);
	newBorder = BuildGateWall(Direction::WEST);
	memcpy(border + 3, newBorder, sizeof(GameObject*) * 3);

	Vector nextPos = position;
	Vector elemSize(GetSize().x, wallWidth);
	border[6] = BuildWall(elemSize);
	border[6]->ForceSetPosition(nextPos);
	nextPos.y += yCount * wallLength;
	border[7] = BuildWall(elemSize);
	border[7]->ForceSetPosition(nextPos);
}

GameObject** LabyrinthSolidifier::BuildGateWall(Direction side) {
	GameObject** w = new GameObject * [3];

	Vector nextPos = position;
	if (side == Direction::WEST) {
		nextPos.x += xCount * wallLength;
	}
	else if (side == Direction::SOUTH) {
		nextPos.y += wallWidth + yCount * wallLength;
	}
	VectorInt exit = labyrinth.GetExit(side);
	
	if (side == Direction::EAST || side == Direction::WEST) {
		Vector elemSize(wallWidth, exit.y * wallLength);
		w[0] = BuildWall(elemSize);
		w[0]->ForceSetPosition(nextPos);
		nextPos.y += elemSize.y;

		// Brama
		elemSize = Vector(wallWidth, wallLength);
		w[1] = BuildWall(elemSize, gateColor);
		w[1]->ForceSetPosition(nextPos);
		nextPos.y += elemSize.y;

		elemSize = Vector(
			wallWidth,
			(yCount - exit.y - 1) * wallLength
		);
		w[2] = BuildWall(elemSize);
		w[2]->ForceSetPosition(nextPos);
	}
	else {
		Vector elemSize(exit.x * wallLength, wallWidth);
		w[0] = BuildWall(elemSize);
		w[0]->ForceSetPosition(nextPos);
		nextPos.x += elemSize.x;

		// Brama
		elemSize = Vector(wallLength, wallWidth);
		w[1] = BuildWall(elemSize, gateColor);
		w[1]->ForceSetPosition(nextPos);
		nextPos.x += elemSize.x;

		elemSize = Vector(
			(xCount - exit.x - 1) * wallLength,
			wallWidth
		);
		w[2] = BuildWall(elemSize);
		w[2]->ForceSetPosition(nextPos);
	}

	return w;
}

void LabyrinthSolidifier::Update() {
	timeSinceLastChange += Timer::Main()->GetDeltaTime();
	if (timeSinceLastChange >= changeTime) {
		ChangeLab();
		timeSinceLastChange = 0;
	}
}