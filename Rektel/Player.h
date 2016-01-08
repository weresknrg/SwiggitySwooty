#pragma once
#include "Entity.h"


class Player :
	public Entity
{
private:

	float		speed; // скорость
	float		steer; // управление
	const float	mass = 1695; // масса
	float		engineForce; // усилие двигателя 
	float		backward; // задний ход
	float		breakForce; // тормоз
	bool		breaking, reverce; // состояния, 1 - тормозит, 2 - задний ход
	int			lifes; // кол-жизней
	int			homisides; // кол - во убийств

	float		rotationAngle; //угол поворота
	float		acceleration; //ускорение
	const float PixelsPerMeter = 18; // пикселей на метр
	sf::Time lifeTime; //оставшееся время
	
	void driving(sf::Time dt); //вождение
	void collisionWithMap(); //столкноения со зданиями

	int sgn(float val); // ф-ция взятия знака
	const float Meter2Pixels(float meters);
	const float Pixels2Meter(float pixels);

	public:
	sf::Sprite getSprite();
	
	Player(sf::Image &image, Level &lev, sf::Vector2f startPosition);
	void update(sf::Time dt);
	sf::Vector2f getPosition();
	void draw(sf::RenderTarget &renderTarget);
	sf::FloatRect getRect();
	sf::Vector2f getSpeedVec();
	float getRotation();
	void increaseHomisideCounter();
	int getHomisides();
	void decreaseLifes(int lifes);
	int getLifes();
	void increaseLifeTime(sf::Time time);
	sf::Time getRemainingTime();
};

