#pragma once

#include <SFML/Graphics.hpp>

class Collision {
public:
	static bool checkCollision(sf::Sprite &s1, sf::FloatRect &s2);	//Спрайт и прямоугольник (игрок и здание)
	static bool checkCollision(sf::Sprite &s1, sf::Sprite &s2); // Два спрайта (игрок и пуля или танк)
private:
	static void project(sf::Vector2f& axis, sf::VertexArray &_rectangle, float &min, float &max); // проекция одного вектора на другой
	static void normalize(sf::Vector2f& vector); // нормализация вектора
	static float dot(sf::Vector2f& vector1, sf::Vector2f& vector2); // скалярное произведение двух векторов
	static float distance(float minA, float maxA, float minB, float maxB); // расстояние между парами точек
	static bool intersects(sf::VertexArray &one, sf::VertexArray &two); // пересечение
};
