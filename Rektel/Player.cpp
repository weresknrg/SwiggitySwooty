#pragma once
#include "Player.h"
#include <iostream> 
#include <math.h>

#define SCALE 0.6

Player::Player(sf::Image &image, Level &lev, float X, float Y, sf::String Name) :Entity(image, X, Y, Name) {
	obj = lev.GetAllObjects();
	steer = 0;
	position.x = X;
	position.y = Y;
	sprite.setScale(SCALE, SCALE);
	width = texture.getSize().x;
	height = texture.getSize().y;
	sprite.setOrigin(width / 2, height / 2);
}

void Player::update(float dt) {
	driving(dt);
	float forceFrict;
	float forceResist;
	float totalForce;

	speedVec = Pixels2Meter(speed) * sf::Vector2f(sinf(sprite.getRotation()* 3.14159265f / 180.0f), cosf( sprite.getRotation() * 3.14159265f / 180.0f));
	rotationAngle += steer;
	forceFrict = -10.5f  * speed;
	forceResist = -2.3f  * sgn(speed) * speed;
	if (breaking) {
		totalForce = -breakForce + forceFrict + forceResist;
	}
	else {
		totalForce = engineForce + forceFrict + forceResist;
	}
	
	std::cout << "acc " << acceleration <<"\t speed \t "<< speed <<"\t frc \t"<< totalForce <<std::endl;
	acceleration = totalForce / mass;


	speed += acceleration * dt;
	position.x += Meter2Pixels(speedVec.x);
	position.y -= Meter2Pixels(speedVec.y);
	sprite.setPosition(position);
	sprite.setRotation(rotationAngle);
}

void Player::driving(float dt) //Управление
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //Газ
	{
		engineForce+= 0.1/dt;
		if (engineForce > 1000)
		{
			engineForce = 1000;
		}
	}
	else
	{
		engineForce -= 10/dt;
		if (engineForce < 0)
		{
			engineForce = 0;
		}
	}
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //Тормоз
	{
		breakForce +=  0.7/ dt;
		breaking = 1;
		if (speed < 0)
			breakForce = 0;
	}
	else {
		breaking = 0;
	}

	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //Влево
	{
		steer -= 0.05/dt *1/sqrt(abs(speed)) ;
		if (steer < -1.0 )
		{
			steer = -1.0 ;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //Вправо
	{
		steer += 0.05/dt * 1/sqrt(abs(speed));
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
			steer = steer - 2 * dt * sgn(steer);
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
	return sf::Vector2f(position.x, position.y);
}

void Player::draw(sf::RenderTarget &window)
{
	window.draw(sprite);
}
