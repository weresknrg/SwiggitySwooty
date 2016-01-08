#pragma once
#include "Entity.h"
#include "Collision.h"
class Bullet :
	public Entity {
public:
	Bullet(sf::Sprite &bullet, sf::Vector2f stPos, std::vector<Object> &mapObjects, float rotation);
	~Bullet() {};
	void draw(sf::RenderTarget& renderTarget);
	void update(sf::Time dt);
	bool checkIsAlife();
	
	void setBulletLife(bool isAlife);
	sf::Sprite getSprite();
private:
	bool checkCollisionsWithWall();
	float speed;
	bool isAlife;
};

