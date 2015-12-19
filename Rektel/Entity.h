#pragma once

#include <SFML/Graphics.hpp>
#include "level.h"


class Entity
{
protected:
	std::vector<Object> obj;
	float width, height;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String name;
	Entity(sf::Image &image, float X, float Y, sf::String Name);
	~Entity();
};