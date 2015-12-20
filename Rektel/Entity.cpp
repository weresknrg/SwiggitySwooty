#include "Entity.h"

Entity::Entity(sf::Image &image, float X, float Y, sf::String Name)
{
	image.createMaskFromColor(sf::Color(255, 255, 255));
	
	texture.loadFromImage(image);
	
	sprite.setTexture(texture);
	sprite.setPosition(X, Y);
	
}

/*
sf::FloatRect Entity::getRect() {//ф-ция получения прямоугольника. его коорд,размеры (шир,высот).
	return sf::FloatRect(x, y, w, h);//эта ф-ция нужна для проверки столкновений 
}
*/
Entity::~Entity()
{
}
