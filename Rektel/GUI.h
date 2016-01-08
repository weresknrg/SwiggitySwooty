#pragma once
#include "SFML\Graphics.hpp"
class GUI
{
public:
	void updateData(sf::FloatRect &visibleArea, sf::Time timeRemaining, int killingSpree, int lifesLeft, sf::Time gameTime); //обновить инфу
	void draw(sf::RenderTarget &renderTarget); //нарисовать
	GUI(sf::Font &font); // конструктор( шрифт )
	~GUI();

private:
	sf::Text killingSpree; // кол-во убийств
	sf::Text timeRemaining; // осталось времени
	sf::Text lifesLeft; // осталось HP
	sf::Text gameTime; // время в игре

	sf::RectangleShape leftPart; // левая часть интерфейса
	sf::RectangleShape rightPart; // правая
};

