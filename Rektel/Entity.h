#pragma once

#include <SFML/Graphics.hpp>
#include "level.h"


class Entity
{
public:
	
protected:
	virtual void draw(sf::RenderTarget& renderTarget) = 0; // виртуальная ф-ция отрисовки
	sf::Sprite sprite; //спрайт
	std::vector<Object> mapObjects; // массив объектов карты
	float width, height; // ширина высота спрайта
	sf::Vector2f speedVec; // скорость
	sf::RectangleShape rect; // прямугольник для столкновений
	sf::Vector2f position; //координаты на карте
	sf::Texture texture; //текстура

	Entity(sf::Image &image, sf::Vector2f startPosition);
	Entity();
	~Entity();
};