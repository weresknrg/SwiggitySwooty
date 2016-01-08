#pragma once
#include "Entity.h"
#include <list>
#include "level.h"
#include "Bullet.h"
#include "Collision.h"


class Tank :
	public Entity
{
public:
	Tank(sf::Vector2f stPos, sf::Texture* texture, sf::Texture* bullet, Level &map); //конструктор ( координаты на карте, указаьель на текстуру, указатель на текстуру снаряда, ссылка на карту)
	~Tank();
	void draw(sf::RenderTarget& renderTarget);
	void update(sf::Time dt);
	void traceThePlayer(sf::Vector2f playerPos); //следить за игроком(координаты игрока)
	bool isTankGone(); // статус танка
	sf::FloatRect getRect();
	int collisionWithPlayer(sf::Sprite playerRect); // столкновение с игроком

private:
	float baseRotation; //угол поворота корпуса
	float turretRotation; //угол поворота башни
	sf::Sprite bullet; // спрайт пули
	sf::Sprite base; // спрайт корпуса
	sf::Sprite turret; // спрайт башни
	sf::Clock shoot; // таймер выстрела
	sf::Time lifeTime; // время жизни
	std::list <Bullet *> bullets; // список снарядов
	bool isGone;
	
};

