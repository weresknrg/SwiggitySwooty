#pragma once
#include "Entity.h"
#include "Animation.h"
#include "Collision.h"
class Citizen :
	public Entity
{
private:

	bool isAlife; // жив ли? если нет - на удаление
	bool isFalling; // сбит ли тачкой?
	float speed; // скорость
	bool isExploding; // взрывается ли кровищей
	float rotationAngle; // угол поворота
	int direction; // направление
	int type; // тип горожанина (0 - 2)косметич разница, 3 - коп
	sf::FloatRect rect; // прямоугольник (для коллизий)

	sf::Time dyingTime; // сколько времени длится анимация смерти
	sf::Time explodingTime; // сколько времени разлетается кровища
	sf::Texture *textureWalking; // спрайт лист ходьбы
	sf::Texture *textureFalling; // спрайт лист падения
	sf::Texture *textureBoom; // спрайт лист взрыва
	Animation walk; // анимация ходьбы
	Animation falling; // смерти
	Animation boom; // взрыва
	AnimatedSprite animSprite; // объект, воспроизводящий анимацию
	Animation *currentAnimation; // текущая анимация


	void walkingDownTheStreet();
	void updateAnimation(sf::Time dt);
	bool collisionWithWall();

public:
	// обработка столкновения с игроком (прямоугольник игрока, скорость игрока, угол поворота игрока)
	bool collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation);
	// конструктор( нач позиция, тип, ссылка на карту, спрайт лист ходьбы, падения, взрыва)
	Citizen(sf::Vector2f pos, int type, Level &map, sf::Texture *Walking, sf::Texture *fall, sf::Texture *exploding);
	// обновление
	void update(sf::Time dt);
	// отрисовка
	void draw(sf::RenderTarget &renderTarget);
	//прямоугольник (для столкновений)
	sf::FloatRect getRect();
	// вернуть статус
	bool checkIsAlife();
	// вернуть тип
	int getType();
	~Citizen();
};

