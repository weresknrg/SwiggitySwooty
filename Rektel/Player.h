#pragma once
#include "Entity.h"


class Player :
	public Entity
{
private:

	float		speed; // ��������
	float		steer; // ����������
	const float	mass = 1695; // �����
	float		engineForce; // ������ ��������� 
	float		backward; // ������ ���
	float		breakForce; // ������
	bool		breaking, reverce; // ���������, 1 - ��������, 2 - ������ ���
	int			lifes; // ���-������
	int			homisides; // ��� - �� �������

	float		rotationAngle; //���� ��������
	float		acceleration; //���������
	const float PixelsPerMeter = 18; // �������� �� ����
	sf::Time lifeTime; //���������� �����
	
	void driving(sf::Time dt); //��������
	void collisionWithMap(); //����������� �� ��������

	int sgn(float val); // �-��� ������ �����
	const float Meter2Pixels(float meters);
	const float Pixels2Meter(float pixels);

	public:
	sf::Sprite getSprite();
	
	Player(sf::Image &image, Level &lev, sf::Vector2f startPosition);
	void update(sf::Time dt);
	sf::Vector2f getPosition();
	void draw(sf::RenderTarget &renderTarget);
	sf::FloatRect getRect();
	sf::Vector2f getSpeedVec();
	float getRotation();
	void increaseHomisideCounter();
	int getHomisides();
	void decreaseLifes(int lifes);
	int getLifes();
	void increaseLifeTime(sf::Time time);
	sf::Time getRemainingTime();
};

