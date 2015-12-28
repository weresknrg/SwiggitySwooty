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
	
	base.setScale(0.8, 0.8);
	base.setOrigin(base.getLocalBounds().width / 2, base.getLocalBounds().height / 2);
	base.setPosition(position);
	baseRotation = rand() % 360;
	base.setRotation(baseRotation);
	turret.setPosition(position);
	turret.setOrigin(turret.getLocalBounds().width / 2, turret.getLocalBounds().height / 4 * 3);
	lifeTime = sf::seconds(16);
	shoot.restart();
}

void Tank::update(sf::Time dt)
{
	if (shoot.getElapsedTime() >= sf::seconds(5)) //shoot once in 5 seconds
	{
		std::cout << "shoot" << std::endl;
		shoot.restart();
		bullets.push_back(new Bullet(bullet, position, mapObjects, turretRotation));
	}
	turret.setRotation(turretRotation);
	lifeTime -= dt;

	for (std::list<Bullet *>::iterator iter = bullets.begin(); iter != bullets.end(); ) 
	{
		if ((*iter)->checkIsAlife()) 
		{
			(*iter)->update(dt);
			iter++;
		}
		else 
		{
			iter = bullets.erase(iter);
		}
	}

	if (lifeTime <= sf::Time::Zero && bullets.size() == 0)
	{
		isGone = true;
	}

}

void Tank::traceThePlayer(sf::Vector2f playerPos)
{
	sf::Vector2f dVec = position - playerPos;
	turretRotation = (atan2f(-dVec.x, dVec.y)) * 180.0f / 3.14159265f;
}

bool Tank::isTankGone()
{
	return isGone;
}

sf::FloatRect Tank::getRect()
{
	return base.getGlobalBounds();
}

Tank::~Tank()
{
}

void Tank::draw(sf::RenderTarget & renderTarget)
{
	renderTarget.draw(base);
	
	for (std::list<Bullet *>::iterator iter = bullets.begin(); iter != bullets.end(); iter++) {
		(*iter)->draw(renderTarget);
	}
	renderTarget.draw(turret);
}

///////////////////////////////////**********\\\\\\\\\\\\\\\\\\\\\



