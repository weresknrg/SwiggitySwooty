#pragma once
#include "Entity.h"
//#include "level.h"

class Player :
	public Entity
{
public:
	
	sf::Vector2f position;
	sf::Vector2f speedVec;
	
	float steer, speed = 0;
	float carMaxSpeed = 200;
	const float mass = 1596;
	float engineForce = 0;
	float breakForce = 0;
	int breaking = 0;

	float rotationAngle = 0;
	float acceleration = 0;
	const float PixelsPerMeter = 18;
	
	//const float driveSens = 2;

	Player(sf::Image &image, float X, float Y, sf::String Name);
	void update(float dt);
	void driving(float dt);
	template <typename T> int sgn(T val);
	float Player::vecLength(sf::Vector2f vec);
	inline const float Meter2Pixels(float meters);
	inline const float Pixels2Meter(float pixels);
};

