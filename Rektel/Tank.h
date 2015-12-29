#pragma once
#include "Entity.h"
#include <list>
#include "level.h"
#include "Bullet.h"
#include "Collision.h"


class Tank :
	public Entity
{
public:
	Tank(sf::Vector2f stPos, sf::Texture* texture, sf::Texture* bullet, Level &map);
	~Tank();
	void draw(sf::RenderTarget& renderTarget);
	void update(sf::Time dt);
	void traceThePlayer(sf::Vector2f playerPos);
	bool isTankGone();
	sf::FloatRect getRect();
	int collisionWithPlayer(sf::Sprite playerRect);

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

