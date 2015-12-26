#include "Citizen.h"
#include "level.h"

Citizen::Citizen(int type, Level &map, sf::Texture *walking, sf::Texture *dying, sf::Texture *exploding)
{
	textureWalking = walking;
	textureDying = dying;
	textureBoom = exploding;
	mapObjects = map.GetAllObjects();

	walk.setSpriteSheet(*textureWalking);
	die.setSpriteSheet(*textureDying);
	boom.setSpriteSheet(*textureBoom);

	if (type == 0)
	{
		walk.addFrame(sf::IntRect(0, 0, 61, 81));
		walk.addFrame(sf::IntRect(62, 13, 50, 45));
		walk.addFrame(sf::IntRect(112, 0, 63, 80));
		walk.addFrame(sf::IntRect(175, 13, 51, 45));

		die.addFrame(sf::IntRect(0, 7, 62, 64));
		die.addFrame(sf::IntRect(63, 0, 68, 63));
		animSprite.setScale(0.6, 0.6);
	}
	if (type == 1)
	{
		walk.addFrame(sf::IntRect(0, 0, 45, 60));
		walk.addFrame(sf::IntRect(45, 16, 37, 34));
		walk.addFrame(sf::IntRect(82, 0, 46, 60));
		walk.addFrame(sf::IntRect(128, 16, 37, 34));

		die.addFrame(sf::IntRect(0, 0, 43, 88));
		die.addFrame(sf::IntRect(43, 9, 79, 70));
		animSprite.setScale(0.7, 0.7);
	}
	if (type == 2)
	{
		walk.addFrame(sf::IntRect(0, 0, 47, 60));
		walk.addFrame(sf::IntRect(47, 15, 38, 34));
		walk.addFrame(sf::IntRect(85, 0, 45, 60));
		walk.addFrame(sf::IntRect(130, 17, 38, 34));

		die.addFrame(sf::IntRect(0, 0, 54, 68));
		die.addFrame(sf::IntRect(54, 0, 76, 75));
		animSprite.setScale(0.7, 0.7);
	}

	boom.addFrame(sf::IntRect(134, 3, 23, 21));
	boom.addFrame(sf::IntRect(224, 27, 37, 33));
	boom.addFrame(sf::IntRect(312, 21, 54, 48));
	boom.addFrame(sf::IntRect(18, 111, 69, 61));
	boom.addFrame(sf::IntRect(108, 106, 82, 74));
	boom.addFrame(sf::IntRect(201, 102, 90, 80));
	boom.addFrame(sf::IntRect(302, 102, 67, 75));
	boom.addFrame(sf::IntRect(21, 193, 54, 74));
	boom.addFrame(sf::IntRect(113, 188, 54, 85));
	boom.addFrame(sf::IntRect(202, 103, 64, 57));
	boom.addFrame(sf::IntRect(300, 205, 66, 59));

	std::vector<Object> vec = map.GetObjects("spawnArea");

	int num = rand() % vec.size();
	position = sf::Vector2f((float)(rand() % (int)vec[num].rect.width + vec[num].rect.left), (float)(rand() % (int)vec[num].rect.height + vec[num].rect.top));

	speedVec = sf::Vector2f(0, 0);
	currentAnimation = &walk;
	rotationAngle = 0;
	isAlife = true;

	dyingTime = sf::seconds(0.4);
	explodingTime = sf::milliseconds(11 * 70);
	unwalkable = map.GetObjects("unwalkable");
}

void Citizen::update(sf::Time dt) {


	position += speedVec * dt.asSeconds();
	updateAnimation(dt);

}

Citizen::~Citizen()
{
}

void Citizen::draw(sf::RenderTarget &renderTarget) {
	renderTarget.draw(animSprite);
}

sf::FloatRect Citizen::getRect()
{
	return sf::FloatRect(animSprite.getGlobalBounds());
}

bool Citizen::checkIsAlife()
{
	if (isAlife) return true;
	return false;
}


void Citizen::updateAnimation(sf::Time dt)
{
	if (isExploding)
	{
		currentAnimation = &boom;
		animSprite.setFrameTime(sf::milliseconds(70));
		animSprite.setScale(1, 1);
		explodingTime -= dt;
		if (explodingTime <= sf::Time::Zero)
		{
			isAlife = false;
		}
	}
	else if (isDying)
	{
		currentAnimation = &die;
		animSprite.setLooped(false);
		dyingTime -= dt;
		if (dyingTime <= sf::Time::Zero)
		{
			isExploding = true;
		}
	}
	else if (isAlife)
	{
		currentAnimation = &walk;
	}

	animSprite.setOrigin(40, 40);
	animSprite.setPosition(position);
	animSprite.setRotation(rotationAngle);
	animSprite.update(dt);
	animSprite.play(*currentAnimation);
}

void Citizen::collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation) {
	if (animSprite.getGlobalBounds().intersects(playerRect) && !isDying)
	{
		rotationAngle = playerRotation;
		speedVec.x = 50 * playerSpeedVec.x + 5;
		speedVec.y = 50 * playerSpeedVec.y + 5;
		isDying = true;
	}
}

