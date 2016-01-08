#include "Tank.h"

Tank::Tank(sf::Vector2f stPos, sf::Texture* texture, sf::Texture* bullet, Level &map)
{
	//setup sprites
	this->bullet.setTexture(*bullet);
	base.setTexture(texture[0]);
	turret.setTexture(texture[1]);
	mapObjects = map.GetAllObjects();
	//select random position

	position = stPos;
	
	base.setScale(0.8, 0.8); // размер корпуса
	base.setOrigin(base.getLocalBounds().width / 2, base.getLocalBounds().height / 2);
	base.setPosition(position);
	baseRotation = rand() % 360;
	base.setRotation(baseRotation);
	
	// положение башни относительно корпуса
	turret.setPosition(position);
	turret.setOrigin(turret.getLocalBounds().width / 2, turret.getLocalBounds().height / 4 * 3); 
	lifeTime = sf::seconds(16);
	shoot.restart();
}

void Tank::update(sf::Time dt)
{
	if (shoot.getElapsedTime() >= sf::seconds(5)) //стреляет раз в 5 секунд
	{
		shoot.restart();
		bullets.push_back(new Bullet(bullet, position, mapObjects, turretRotation)); // создание пули
	}
	turret.setRotation(turretRotation);
	lifeTime -= dt;

	for (std::list<Bullet *>::iterator iter = bullets.begin(); iter != bullets.end(); ) //пробегаемся по всем пулям
	{
		if ((*iter)->checkIsAlife()) //если пуля существует
		{
			(*iter)->update(dt); //обновляем
			iter++;
		}
		else 
		{
			iter = bullets.erase(iter); //если нет то удаляем
		}
	}

	if (lifeTime <= sf::Time::Zero && bullets.size() == 0) // если время жизни танка истекло и пуль нет на карте
	{
		isGone = true; //убираем танк
	}

}

void Tank::traceThePlayer(sf::Vector2f playerPos)
{
	sf::Vector2f dVec = position - playerPos; // вектор, соединяющий башню и игрока
	turretRotation = (atan2f(-dVec.x, dVec.y)) * 180.0f / 3.14159265f; // вращаем на угол наклона вектора
}

bool Tank::isTankGone()
{
	return isGone;
}

sf::FloatRect Tank::getRect()
{
	return base.getGlobalBounds();
}

int Tank::collisionWithPlayer(sf::Sprite playerSprite)
{
	for (std::list<Bullet *>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) //столкновение с пулей
	{
		if (Collision::checkCollision((*iter)->getSprite(), playerSprite))
		{
			(*iter)->setBulletLife(false);//если игрок столкнулся с пулей
			return 2; //возвращаем 2
		}
	}
	if (Collision::checkCollision(playerSprite, base)) // если с корпусом 
	{
		return 1; // 1
	}
	return 0; // в других случаях 0
}

Tank::~Tank()
{
}

void Tank::draw(sf::RenderTarget & renderTarget) // рисуем сам танк и его пули
{
	renderTarget.draw(base);
	
	for (std::list<Bullet *>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
		(*iter)->draw(renderTarget);
	}
	renderTarget.draw(turret);
}

///////////////////////////////////**********\\\\\\\\\\\\\\\\\\\\\



