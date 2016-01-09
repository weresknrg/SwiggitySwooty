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
	sprite.setPosition(position);// перемещаем пулю
	if (checkCollisionsWithWall())// если столкнулась со стеной 
		isAlife = false; // убиваем
}

bool Bullet::checkIsAlife()
{
	return isAlife;
}

bool Bullet::checkCollisionsWithWall()
{
	for (int i = 0; i < mapObjects.size(); i++)  // бежим по всем объектам на карте
	{
		if (sprite.getGlobalBounds().intersects(mapObjects[i].rect)) //если есть пересечение
			if (mapObjects[i].name == "solid" || mapObjects[i].name == "building") //и это здание
				return true; // возвращаем true
	}
	return false; 
}

void Bullet::setBulletLife(bool isAlife)
{
	this->isAlife = isAlife;
}

sf::Sprite Bullet::getSprite()
{
	return sprite;
}
