#pragma once
#include "Player.h"
#include <iostream> 
#include <math.h>
#include "Collision.h"
#define SCALE 0.6

Player::Player(sf::Image &image, Level &lev, sf::Vector2f startPosition) :Entity(image, startPosition) {
	mapObjects = lev.GetObjects("solid");
	
	sprite.setScale(SCALE, SCALE);
	width = texture.getSize().x;
	height = texture.getSize().y;
	sprite.setOrigin(width / 2, height * 3 / 4);
	//начальные значени€
	position = startPosition;
	steer = 0;
	speed = 0;
	acceleration = 0;
	engineForce = 0;
	breakForce = 0;
	reverce = 0;
	
	rect.setSize((sf::Vector2f) texture.getSize());
	rect.setScale(sprite.getScale());
	rect.setOrigin(sprite.getOrigin());
	rect.setFillColor(sf::Color::Green);
	
	lifes = 6; // кол-во жизней
	homisides = 0; 
	lifeTime = sf::seconds(30); //врем€ жизни
}

void Player::update(sf::Time dt)
{
	sprite.setPosition(rect.getPosition());
	sprite.setRotation(rect.getRotation());

	lifeTime -= dt; // уменьшаем врем€ жизни
	driving(dt); 
	
	float forceFrict; // сила трени€ колес
	float forceResist; // сила сопротивлени€ воздуха
	float totalForce; //пона€ сила

	speedVec = Pixels2Meter(speed) * sf::Vector2f(sinf(rotationAngle* 3.14159265f / 180.0f), cosf(rotationAngle * 3.14159265f / 180.0f)); // вектор скорости
	rotationAngle += steer; //машину поворачиваем на угол
	// считаем силы
	forceFrict = -250.5f  * speed; 
	forceResist = -10.3f  * sgn(speed) * speed *speed;

	if (breaking) //если тормозим
	{
		totalForce = -sgn(speed) * breakForce + forceFrict + forceResist;
	}
	else if (reverce) //если сдаем назад
	{
		totalForce = -backward + forceFrict + forceResist;
	}
	else
	{
		totalForce = engineForce + forceFrict + forceResist;
	}
	acceleration = totalForce / mass; //ускорение
	collisionWithMap(); // столкновен€ с картой

	speed += acceleration * dt.asSeconds(); //скорость
	position.x += Meter2Pixels(speedVec.x);
	position.y -= Meter2Pixels(speedVec.y);
	rect.setPosition(position); // перемещаем машину
	rect.setRotation(rotationAngle);
}

void Player::driving(sf::Time dt)  //”правление
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //√аз
	{
		engineForce += 1000 * dt.asSeconds();
		if (engineForce > 1500)
		{
			engineForce = 1500;
		}
	}
	else
	{
		engineForce -= 500 * dt.asSeconds();
		if (engineForce < 0)
		{
			engineForce = 0;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //“ормоз
	{
		breakForce += 2000 * dt.asSeconds();
		breaking = true;
	}
	else {
		breaking = false;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		backward += 300 * dt.asSeconds();
		reverce = true;
		if (speed < -3) {
			speed = -3;
		}
	}
	else {
		reverce = false;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //¬лево
	{
		if(speed > 0.5)
			steer -= 0.05 / dt.asSeconds();
		if (steer < -1.0)
		{
			steer = -1.0;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //¬право
	{
		if(speed > 0.5)
			steer += 0.05 / dt.asSeconds();
		if (steer > 1.0)
		{
			steer = 1.0;
		}
	}
	else //¬ыровн€ть
	{
		if (abs(steer) < 0.05)
			steer = 0;
		else
			steer = steer - 2 * dt.asSeconds() * sgn(steer);
	}
}

void Player::collisionWithMap() // ќбраблтка столкновений с объектами на карте
{
	int counter = 0;
	for (std::vector<Object>::iterator mapObjectIter = mapObjects.begin(); mapObjectIter != mapObjects.end(); mapObjectIter++) {
		if (Collision::checkCollision(sprite, mapObjects[counter].rect)) 
		{
			if (mapObjects[counter].name == "solid") // если столкнулись
			{
				// отскакиваем от стены
				speedVec = -speedVec;
				speed = -0.7 * speed;
				lifes--;
			}
		}
		counter++;
	}
}

int Player::sgn(float val) {
	return (0.f < val) - (val < 0.f);
}

const float Player::Meter2Pixels(float meters)
{
	return meters * PixelsPerMeter;
}

const float Player::Pixels2Meter(float pixels)
{
	return pixels / PixelsPerMeter;
}

sf::Vector2f Player::getPosition()
{
	return sf::Vector2f(position);
}

void Player::draw(sf::RenderTarget &window)
{
	window.draw(sprite);
}

sf::FloatRect Player::getRect()
{
	return rect.getGlobalBounds();
}

sf::Vector2f Player::getSpeedVec()
{
	return sf::Vector2f(Meter2Pixels(speedVec.x), Meter2Pixels(-speedVec.y));
}

float Player::getRotation()
{
	return rotationAngle;
}

void Player::increaseHomisideCounter()
{
	homisides++;
}

int Player::getHomisides()
{
	return homisides;
}

sf::Sprite Player::getSprite()
{
	return sprite;
}

void Player::decreaseLifes(int lifes)
{
	this->lifes-=lifes;
}

int Player::getLifes()
{
	return lifes;
}

void Player::increaseLifeTime(sf::Time time)
{
	lifeTime += time;
}

sf::Time Player::getRemainingTime()
{
	return lifeTime;
}
