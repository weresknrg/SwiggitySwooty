#pragma once

#include <SFML/Graphics.hpp>
#include "level.h"


class Entity
{
protected:
	std::vector<Object> objVector;
	std::vector<Object>::const_iterator ObjIter;
	float width, height;
	sf::RectangleShape rect;
	sf::Vector2f position;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String name;
	Entity(sf::Image &image, float X, float Y, sf::String Name);
	~Entity();
};