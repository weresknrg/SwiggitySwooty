#pragma once

#include <SFML/Graphics.hpp>

class Collision {
public:
	
	static bool checkCollision(sf::Sprite &s1, sf::FloatRect &s2);	
	static bool checkCollision(sf::Sprite &s1, sf::Sprite &s2);
private:
	static void project(sf::Vector2f& axis, sf::VertexArray &_rectangle, float &min, float &max);
	static void normalize(sf::Vector2f& vector);
	static float dot(sf::Vector2f& vector1, sf::Vector2f& vector2);
	static float distance(float minA, float maxA, float minB, float maxB);
	static bool intersects(sf::VertexArray &one, sf::VertexArray &two);
};
