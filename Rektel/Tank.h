#pragma once
#include "Entity.h"
#include <list>
#include "level.h"
#include "Bullet.h"


class Tank :
	public Entity
{
public:
	Tank(sf::Texture* texture, sf::Texture* bullet, Level &map);
	~Tank();
	void draw(sf::RenderTarget& renderTarget);
	void update(sf::Time dt);
	void traceThePlayer(sf::Vector2f playerPos);
	bool isTankGone();

private:
	float baseRotation;
	float turretRotation;
	sf::Sprite bullet;
	sf::Sprite base;
	sf::Sprite turret;
	sf::Clock shoot;
	sf::Time lifeTime;
	std::list <Bullet *> bullets;
	bool isGone;
};

