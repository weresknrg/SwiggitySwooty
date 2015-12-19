#pragma once
#include "Entity.h"
//#include "level.h"

class Player :
	public Entity
{
private:
	sf::Vector2f position;
	sf::Vector2f speedVec;
	
	float steer, speed;
	float carMaxSpeed;
	const float mass = 1695;
	float engineForce;
	float breakForce;
	int breaking;

	float rotationAngle;
	float acceleration;
	const float PixelsPerMeter = 18;
	void driving(float dt);

	template <typename T> int sgn(T val);
	inline const float Meter2Pixels(float meters);
	inline const float Pixels2Meter(float pixels);

	public:
	Player(sf::Image &image, Level &lev, float X, float Y, sf::String Name);
	void update(float dt);
	sf::Vector2f getPosition();
	void draw(sf::RenderTarget &renderTarget);
};

