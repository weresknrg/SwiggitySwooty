#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Collision.h"
class Citizen :
	public Entity
{
private:

	bool isAlife;
	bool isDying;
	float speed;
	bool isExploding;
	float rotationAngle;
	int direction;
	int a;
	sf::FloatRect rect;

	sf::Time dyingTime;
	sf::Time explodingTime;
	sf::Texture *textureWalking;
	sf::Texture *textureDying;
	sf::Texture *textureBoom;
	Animation walk;
	Animation die;
	Animation boom;
	AnimatedSprite animSprite;
	Animation *currentAnimation;

	void walkingDownTheStreet(sf::Time dt);
	void updateAnimation(sf::Time dt);
	void collisionWithWall();

public:
	void collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation);
	Citizen(int type, Level &map, sf::Texture *Walking, sf::Texture *die, sf::Texture *exploding);
	void update(sf::Time dt);
	void draw(sf::RenderTarget &renderTarget, sf::View &view);
	sf::FloatRect getRect();
	bool checkIsAlife();
	~Citizen();
};

