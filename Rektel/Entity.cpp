#include "Entity.h"

Entity::Entity(sf::Image &image, sf::Vector2f startPosition)
{
	image.createMaskFromColor(sf::Color(255, 255, 255));
	texture.loadFromImage(image);
	sprite.setTexture(texture);
	sprite.setPosition(startPosition);
	
}

Entity::Entity()
{
	
}

Entity::~Entity()
{
}
