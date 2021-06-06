#include "SpriteRenderer.h"
#include "Draw.h"

SpriteRenderer::SpriteRenderer(GameObject& owner, SDL_Surface* screen, SDL_Surface* sprite)
	: ObjectRenderer(owner, screen), sprite(sprite) {

}

void SpriteRenderer::Render(SDL_Surface* screen) {
	SDL_Renderer* renderer = Window::Main()->GetRenderer();
	SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, sprite);

	SDL_Rect dstRect;
	dstRect.x = gameObject.GetPosition().x;
	dstRect.y = gameObject.GetPosition().y;
	dstRect.w = gameObject.GetSize().x;
	dstRect.h = gameObject.GetSize().y;

	Window::Main()->RenderTexture(texture, dstRect, gameObject.GetRotation());
}

ObjectComponent* SpriteRenderer::Copy(GameObject& newOwner) {
	return new SpriteRenderer(
		newOwner,
		screen,
		sprite
	);
}