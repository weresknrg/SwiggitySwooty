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
	//��������� ��������
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
	
	lifes = 6; // ���-�� ������
	homisides = 0; 
	lifeTime = sf::seconds(30); //����� �����
}

void Player::update(sf::Time dt)
{
	sprite.setPosition(rect.getPosition());
	sprite.setRotation(rect.getRotation());

	lifeTime -= dt; // ��������� ����� �����
	driving(dt); 
	
	float forceFrict; // ���� ������ �����
	float forceResist; // ���� ������������� �������
	float totalForce; //����� ����

	speedVec = Pixels2Meter(speed) * sf::Vector2f(sinf(rotationAngle* 3.14159265f / 180.0f), cosf(rotationAngle * 3.14159265f / 180.0f)); // ������ ��������
	rotationAngle += steer; //������ ������������ �� ����
	// ������� ����
	forceFrict = -250.5f  * speed; 
	forceResist = -10.3f  * sgn(speed) * speed *speed;

	if (breaking) //���� ��������
	{
		totalForce = -sgn(speed) * breakForce + forceFrict + forceResist;
	}
	else if (reverce) //���� ����� �����
	{
		totalForce = -backward + forceFrict + forceResist;
	}
	else
	{
		totalForce = engineForce + forceFrict + forceResist;
	}
	acceleration = totalForce / mass; //���������
	collisionWithMap(); // ����������� � ������

	speed += acceleration * dt.asSeconds(); //��������
	position.x += Meter2Pixels(speedVec.x);
	position.y -= Meter2Pixels(speedVec.y);
	rect.setPosition(position); // ���������� ������
	rect.setRotation(rotationAngle);
}

void Player::driving(sf::Time dt)  //����������
{
	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //���
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

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) //������
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


	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //�����
	{
		if(speed > 0.5)
			steer -= 0.05 / dt.asSeconds();
		if (steer < -1.0)
		{
			steer = -1.0;
		}
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //������
	{
		if(speed > 0.5)
			steer += 0.05 / dt.asSeconds();
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
			steer = steer - 2 * dt.asSeconds() * sgn(steer);
	}
}

void Player::collisionWithMap() // ��������� ������������ � ��������� �� �����
{
	int counter = 0;
	for (std::vector<Object>::iterator mapObjectIter = mapObjects.begin(); mapObjectIter != mapObjects.end(); mapObjectIter++) {
		if (Collision::checkCollision(sprite, mapObjects[counter].rect)) 
		{
			if (mapObjects[counter].name == "solid") // ���� �����������
			{
				// ����������� �� �����
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
