#include "GUI.h"

void GUI::updateData(sf::FloatRect &visibleArea, sf::Time timeRemaining, int killingSpree, int lifesLeft, sf::Time gameTime)
{
	//размещаем части интерфейса на экране
	leftPart.setPosition(visibleArea.left, visibleArea.top);
	rightPart.setPosition(visibleArea.left + visibleArea.width - 250, visibleArea.top);
	//размещаем надписи
	this->lifesLeft.setPosition(leftPart.getPosition().x + 5, leftPart.getPosition().y + 5);
	this->timeRemaining.setPosition(leftPart.getPosition().x + 5, leftPart.getPosition().y + 10 + 20 + 5);
	this->killingSpree.setPosition(rightPart.getPosition().x + 5, rightPart.getPosition().y + 5);
	this->gameTime.setPosition(rightPart.getPosition().x + 5, rightPart.getPosition().y + 10 + 20 + 5);
	//присваиваем строкам обновленные значения 
	this->lifesLeft.setString("Durablity    \t" + std::to_string(lifesLeft));
	this->timeRemaining.setString("Time remain \t" + std::to_string((int)timeRemaining.asSeconds()));
	this->killingSpree.setString( "Kills        \t " + std::to_string(killingSpree));
	this->gameTime.setString("Game time\t" + std::to_string((int)gameTime.asSeconds()));

}

void GUI::draw(sf::RenderTarget & renderTarget)
{
	// рисуем
	renderTarget.draw(leftPart);
	renderTarget.draw(rightPart);
	renderTarget.draw(lifesLeft);
	renderTarget.draw(timeRemaining);
	renderTarget.draw(killingSpree);
	renderTarget.draw(gameTime);
}

GUI::GUI(sf::Font &font)
{
	//здесь задаюися цвет и шрифты
	font.loadFromFile("images/font.ttf");
	killingSpree = sf::Text("0", font, 15);
	lifesLeft = sf::Text("0", font, 15);
	timeRemaining = sf::Text("30", font, 15);
	gameTime = sf::Text("0", font, 15);

	rightPart.setFillColor(sf::Color(0, 0, 0, 128));
	rightPart.setSize(sf::Vector2f(250, 75));	
	leftPart.setSize(sf::Vector2f(250, 75));
	leftPart.setFillColor(sf::Color(0, 0, 0, 128));
}


GUI::~GUI()
{
}

