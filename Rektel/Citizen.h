#pragma once
#include "Entity.h"
#include "Animation.h"
class Citizen :
	public Entity
{
private:
	//sf::Image image;
	bool isAlife;
	bool isDying;
	float rotationAngle;
	sf::Time dyingTime;
	//sf::Vector2f pos, speedVec;
	sf::Texture textureDying;
	Animation walk;
	Animation dying;
	AnimatedSprite animSprite;
	Animation *currentAnimation;

	

public:
	void collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation);
	Citizen(Level &map);
	void update(sf::Time dt);
	void draw(sf::RenderTarget &renderTarget);
	sf::FloatRect getRect();
	bool checkIsAlife();
	~Citizen();
};

