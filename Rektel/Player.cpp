#pragma once
#include "Player.h"
#include <iostream> 
#include <math.h>
#include "Collision.h"
#define SCALE 0.6

Player::Player(sf::Image &image, Level &lev, sf::Vector2f startPosition, sf::String Name) :Entity(image, startPosition, Name) {
	mapObjects = lev.GetAllObjects();
	steer = 0;
	position = startPosition;
	sprite.setScale(SCALE, SCALE);
	width = texture.getSize().x;
	height = texture.getSize().y;
	sprite.setOrigin(width / 2, height * 3 / 4);
	rect.setSize((sf::Vector2f) texture.getSize());
	rect.setScale(sprite.getScale());
	rect.setOrigin(sprite.getOrigin());
	rect.setFillColor(sf::Color::Green);
}

void Player::update(sf::Time dt)
{
	sprite.setPosition(rect.getPosition());
	sprite.setRotation(rect.getRotation());

	driving(dt);
	float forceFrict;
	float forceResist;
	float totalForce;

	speedVec = Pixels2Meter(speed) * sf::Vector2f(sinf(rotationAngle* 3.14159265f / 180.0f), cosf(rotationAngle * 3.14159265f / 180.0f));
	rotationAngle += steer;
	forceFrict = -250.5f  * speed;
	forceResist = -15.3f  * sgn(speed) * speed;

	if (breaking) {
		totalForce = -sgn(speed) * breakForce + forceFrict + forceResist;
	}
	else if (reverce) {
		totalForce = -backward + forceFrict + forceResist;
	}
	else {
		totalForce = engineForce + forceFrict + forceResist;
	}

	//std::cout << "w " << rect.getSize().x << "\t h \t " << height*SCALE << std::endl;
	//std::cout << "acc " << acceleration <<"\t speed \t "<< speed <<"\t frc \t"<< totalForce <<std::endl;

	acceleration = totalForce / mass;
	collisionWithMap();

	speed += acceleration * dt.asSeconds();
	position.x += Meter2Pixels(speedVec.x);
	position.y -= Meter2Pixels(speedVec.y);
	rect.setPosition(position);
	rect.setRotation(rotationAngle);
}

void Player::driving(sf::Time dt)  //Управление
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //Газ
	{
		engineForce += 1000 * dt.asSeconds();
		if (engineForce > 1000)
		{
			engineForce = 1000;
		}
	}
	else
	{
		engineForce -= 10 * dt.asSeconds();
		if (engineForce < 0)
		{
			engineForce = 0;
		}
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //Тормоз
	{
		breakForce += 0.8 * dt.asSeconds();
		breaking = 1;
	}
	else {
		breaking = 0;
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
		backward += 0.3 * dt.asSeconds();
		reverce = 1;
		if (speed < -3) {
			speed = -3;
		}
	}
	else {
		reverce = 0;
	}


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //Влево
	{
		steer -= 0.05 / dt.asSeconds() * 1 / sqrt(abs(speed));
		if (steer < -1.0)
		{
			steer = -1.0;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //Вправо
	{
		steer += 0.05 / dt.asSeconds() * 1 / sqrt(abs(speed));
		if (steer > 1.0)
		{
			steer = 1.0;
		}
	}
	else //Выровнять
	{
		if (abs(steer) < 0.05)
			steer = 0;
		else
			steer = steer - 2 * dt.asSeconds() * sgn(steer);
	}
}

void Player::collisionWithMap() // Обраблтка столкновений с объектами на карте
{
	int counter = 0;
	for (mapObjectIter = mapObjects.begin(); mapObjectIter != mapObjects.end(); mapObjectIter++) {
		if (Collision::checkCollision(sprite, mapObjects[counter].rect)) {
			if (mapObjects[counter].name == "solid")
			{
				// вместо этого, сюда написать что нибуть нормальное
				speedVec = -speedVec;
				speed = -0.5 * speed;
			}
		}
		counter++;
	}
}

template <typename T> int Player::sgn(T val) {
	return (T(0) < val) - (val < T(0));
}

inline const float Player::Meter2Pixels(float meters)
{
	return meters * PixelsPerMeter;
}

inline const float Player::Pixels2Meter(float pixels)
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
