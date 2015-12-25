#pragma once
#include "Entity.h"


class Player :
	public Entity
{
private:

	
	float		steer, speed;
	const float	mass = 1695;
	float		engineForce, backward;
	float		breakForce;
	int			breaking, reverce;

	float		rotationAngle;
	float		acceleration;
	const float PixelsPerMeter = 18;
	
	
	void driving(sf::Time dt);
	void collisionWithMap();

	template <typename T> int sgn(T val);
	inline const float Meter2Pixels(float meters);
	inline const float Pixels2Meter(float pixels);

	public:
	Player(sf::Image &image, Level &lev, sf::Vector2f startPosition, sf::String Name);
	void update(sf::Time dt);
	sf::Vector2f getPosition();
	void draw(sf::RenderTarget &renderTarget);
	sf::FloatRect getRect();
	sf::Vector2f getSpeedVec();
	float getRotation();
};

