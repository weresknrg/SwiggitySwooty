#pragma once
#include "Entity.h"
#include "Animation.h"
class Citizen :
	public Entity
{
private:
	bool isAlife;
	float speed;
	float rotationAngle;
	Animation walk;
	AnimatedSprite animatedSprite;
	Animation *currentAnimation;

public:
	Citizen(sf::Image Image, std::vector<sf::IntRect> &coordForFrames, Level &lev, sf::Vector2f stPos);
	void update(sf::Time dt);
	void draw(sf::RenderTarget &renderTarget);
	~Citizen();
};

