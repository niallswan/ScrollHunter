#include "cmp_spritesheet.h"
#include "system_renderer.h"
#include "engine.h"
#include <cmath>

using namespace sf;
using namespace std;

SpriteSheetComponent::SpriteSheetComponent(Entity* p, Vector2f size) : Component(p), set_size(size) {
	animationTimer = 0.0f;
	currentFrame.width = set_size.x;
	currentFrame.height = set_size.y;
	currentTexture = Vector2u(0, 0);
	frameTime = 0.2f;
	frameCount = 1;
	end = false;
	auto origin = set_size / 2.f;
	origin.x = floor(origin.x);
	origin.y = floor(origin.y);
	sprite.setOrigin(origin);
}

void SpriteSheetComponent::update(double dt) {

	end = false;
	animationTimer += dt;
	if (animationTimer > frameTime) {
		animationTimer -= frameTime;
		currentTexture.x++;
		if (currentTexture.x >= frameCount) {
			end = true;
			currentTexture.x = 0;
		}
	}

	

	//cout << _parent->scene->ents.find("enemy")[0]->isVisible() << endl;

	currentFrame.left = currentTexture.x * set_size.x;
	currentFrame.top = currentTexture.y * set_size.y;
	sprite.setTextureRect(currentFrame);
	sprite.setPosition(_parent->getPosition());
}

void SpriteSheetComponent::render() {
	Renderer::queueSpritesheet(sprite);
}

void SpriteSheetComponent::setSpritesheet(const sf::Texture& setSpritesheet) {
	spritesheet = setSpritesheet;
	sprite.setTexture(spritesheet);
}

void SpriteSheetComponent::setFrameTime(float time) {
	frameTime = time;
}

void SpriteSheetComponent::setFrameCount(int count) {
	frameCount = count;
}

void SpriteSheetComponent::setSize(const Vector2f& size) {
	set_size = size;
	currentFrame.left = set_size.x;
	currentFrame.top = set_size.y;

	auto origin = set_size / 2.f;
	origin.x = floor(origin.x);
	origin.y = floor(origin.y);
	sprite.setOrigin(origin);
}

const Vector2f& SpriteSheetComponent::getSize() const {
	return set_size;
}

int SpriteSheetComponent::getFrameCount() const {
	return frameCount;
}

float SpriteSheetComponent::getFrameTime() const {
	return frameTime;
}

bool SpriteSheetComponent::finished() const {
	return end;
}

const Sprite& SpriteSheetComponent::getSprite() const {
	return sprite;
}

