#include "RectangleRenderer.h"

RectangleRenderer::RectangleRenderer(GameObject& owner, SDL_Surface* screen, Uint32 outlineColor, Uint32 fillColor)
	:ObjectRenderer(owner, screen), outlineColor(outlineColor), fillColor(fillColor) {

}

void RectangleRenderer::Render(SDL_Surface* screen) {
	Vector size = gameObject.GetSize();
	DrawRectangle(screen, gameObject.position.x, gameObject.position.y, size.x, size.y, outlineColor, fillColor);
}