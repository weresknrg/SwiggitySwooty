#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Collision.h"
class Citizen :
	public Entity
{
private:

	bool isAlife; // ��� ��? ���� ��� - �� ��������
	bool isFalling; // ���� �� ������?
	float speed; // ��������
	bool isExploding; // ���������� �� ��������
	float rotationAngle; // ���� ��������
	int direction; // �����������
	int type; // ��� ���������� (0 - 2)�������� �������, 3 - ���
	sf::FloatRect rect; // ������������� (��� ��������)

	sf::Time dyingTime; // ������� ������� ������ �������� ������
	sf::Time explodingTime; // ������� ������� ����������� �������
	sf::Texture *textureWalking; // ������ ���� ������
	sf::Texture *textureFalling; // ������ ���� �������
	sf::Texture *textureBoom; // ������ ���� ������
	Animation walk; // �������� ������
	Animation falling; // ������
	Animation boom; // ������
	AnimatedSprite animSprite; // ������, ��������������� ��������
	Animation *currentAnimation; // ������� ��������


	void walkingDownTheStreet();
	void updateAnimation(sf::Time dt);
	bool collisionWithWall();

public:
	// ��������� ������������ � ������� (������������� ������, �������� ������, ���� �������� ������)
	bool collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation);
	// �����������( ��� �������, ���, ������ �� �����, ������ ���� ������, �������, ������)
	Citizen(sf::Vector2f pos, int type, Level &map, sf::Texture *Walking, sf::Texture *fall, sf::Texture *exploding);
	// ����������
	void update(sf::Time dt);
	// ���������
	void draw(sf::RenderTarget &renderTarget);
	//������������� (��� ������������)
	sf::FloatRect getRect();
	// ������� ������
	bool checkIsAlife();
	// ������� ���
	int getType();
	~Citizen();
};

