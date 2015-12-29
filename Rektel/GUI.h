#pragma once
#include "SFML\Graphics.hpp"
class GUI
{
public:
	void updateData(sf::FloatRect &visibleArea, sf::Time timeRemaining, int killingSpree, int lifesLeft, sf::Time gameTime);
	void draw(sf::RenderTarget &renderTarget);
	GUI();
	~GUI();

private:
	sf::Font font;
	sf::Text killingSpree;
	sf::Text timeRemaining;
	sf::Text lifesLeft;
	sf::Text gameTime;

	sf::RectangleShape leftPart;
	sf::RectangleShape rightPart;
};

