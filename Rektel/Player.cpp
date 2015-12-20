#pragma once
#include "Player.h"
#include <iostream> 
#include <math.h>
#include  "Collision.h"


#define SCALE 0.6

Player::Player(sf::Image &image, Level &lev, float X, float Y, sf::String Name) :Entity(image, X, Y, Name) {
	objVector = lev.GetAllObjects();
	steer = 0;
	position.x = X;
	position.y = Y;
	sprite.setScale(SCALE, SCALE);
	width = texture.getSize().x;
	height = texture.getSize().y;
	sprite.setOrigin(width / 2, height / 2);
	rect.setSize((sf::Vector2f) texture.getSize());
	rect.setScale(sprite.getScale());
	rect.setOrigin(sprite.getOrigin());
	rect.setFillColor(sf::Color::Magenta);
}

void Player::update(float dt) {
	sprite.setPosition(rect.getPosition());
	sprite.setRotation(rect.getRotation());
	
	driving(dt);
	float forceFrict;
	float forceResist;
	float totalForce;

	speedVec = Pixels2Meter(speed) * sf::Vector2f(sinf(rotationAngle* 3.14159265f / 180.0f), cosf( rotationAngle * 3.14159265f / 180.0f));
	rotationAngle += steer;
	forceFrict = -10.5f  * speed;
	forceResist = -112.3f  * sgn(speed) * speed;
	if (breaking) {
		totalForce = -breakForce + forceFrict + forceResist;
	}
	else {
		totalForce = engineForce + forceFrict + forceResist;
	}
	//std::cout << "w " << rect.getSize().x << "\t h \t " << height*SCALE << std::endl;
	//std::cout << "acc " << acceleration <<"\t speed \t "<< speed <<"\t frc \t"<< totalForce <<std::endl;
	acceleration = totalForce / mass;
	collisionWithMap();

	speed += acceleration * dt;
	position.x += Meter2Pixels(speedVec.x);
	position.y -= Meter2Pixels(speedVec.y);
	rect.setPosition(position);
	rect.setRotation(rotationAngle);
}

void Player::driving(float dt) //����������
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //���
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
	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //������
	{
		breakForce +=  0.7/ dt;
		breaking = 1;
		if (speed < 0)
			breakForce = 0;
	}
	else {
		breaking = 0;
	}

	
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //�����
	{
		steer -= 0.05/dt *1/sqrt(abs(speed)) ;
		if (steer < -1.0 )
		{
			steer = -1.0 ;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //������
	{
		steer += 0.05/dt * 1/sqrt(abs(speed));
		if (steer > 1.0)
		{
			steer = 1.0;
		}
	}
	else //���������
	{
		if (abs(steer) < 0.05) 
			steer = 0;
		else
			steer = steer - 2 * dt * sgn(steer);
	}
}

void Player::collisionWithMap()
{
	SAT sat;
	int counter = 0;
	for (ObjIter = objVector.begin(); ObjIter != objVector.end(); ObjIter++) {
		if (sat.checkCollision(sprite, objVector[counter].rect)) {
			if (objVector[counter].name == "solid") {
				speedVec = -speedVec;
				speed = -0.4f * speed;
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

