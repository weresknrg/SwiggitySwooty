#pragma once

#include <SFML/Graphics.hpp>
#include "level.h"


class Entity
{
public:
	
protected:
	virtual void draw(sf::RenderTarget& renderTarget) = 0;
	sf::Sprite sprite;
	std::vector<Object> mapObjects;
	float width, height;
	sf::Vector2f speedVec;
	sf::RectangleShape rect;
	sf::Vector2f position;
	sf::Texture texture;

	sf::String name;
	Entity(sf::Image &image, sf::Vector2f startPosition, sf::String Name);
	Entity();
	~Entity();
};