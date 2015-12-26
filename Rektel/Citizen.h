#pragma once
#include "Entity.h"
#include "Animation.h"
class Citizen :
	public Entity
{
private:

	bool isAlife;
	bool isDying;
	bool isExploding;
	float rotationAngle;
	sf::Vector2f direction;

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

	std::vector<Object> unwalkable;

	void updateAnimation(sf::Time dt);


public:
	void collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation);
	Citizen(int type, Level &map, sf::Texture *Walking, sf::Texture *die, sf::Texture *exploding);
	void update(sf::Time dt);
	void draw(sf::RenderTarget &renderTarget);
	sf::FloatRect getRect();
	bool checkIsAlife();
	~Citizen();
};

