#pragma once

#include <SFML/Graphics.hpp>
#include "level.h"


class Entity
{
protected:
	std::vector<Object> mapObjects;
	std::vector<Object>::iterator mapObjectIter;
	float width, height;
	sf::Vector2f speedVec;
	sf::RectangleShape rect;
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String name;
	Entity(sf::Image &image, sf::Vector2f startPosition, sf::String Name);
	Entity();
	~Entity();
};