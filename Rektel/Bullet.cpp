#include "Bullet.h"

Bullet::Bullet(sf::Sprite &bullet, sf::Vector2f stPos, std::vector<Object>& mapObjects, float rotation)
{
	this->mapObjects = mapObjects;
	sprite = bullet;
	position = stPos;
	speed = 15;
	speedVec = speed * sf::Vector2f(sinf(rotation * 3.14159265f / 180.0f), -cosf(rotation* 3.14159265f / 180.0f));
	isAlife = true;
	sprite.setRotation(rotation);
}

void Bullet::draw(sf::RenderTarget & renderTarget)
{
	renderTarget.draw(sprite);
}



void Bullet::update(sf::Time dt)
{
	position += speedVec;
	sprite.setPosition(position);
	if (checkCollisionsWithWall())
		isAlife = false;
}

bool Bullet::checkIsAlife()
{
	return isAlife;
}

bool Bullet::checkCollisionsWithWall()
{
	for (int i = 0; i < mapObjects.size(); i++) {
		if (sprite.getGlobalBounds().intersects(mapObjects[i].rect))
			if (mapObjects[i].name == "solid")
				return true;
	}
	return false;
}
