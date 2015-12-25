#include "Citizen.h"
#include "level.h"

Citizen::Citizen(Level &map)
{
	sf::Image image;
	mapObjects = map.GetAllObjects();
	int type = rand() % 3 + 1;
	if (type == 1) 
	{
		image.loadFromFile("images/Citizens/guy1walk.png");
		image.createMaskFromColor(sf::Color(255, 255, 255));
		texture.loadFromImage(image);
		
		walk.setSpriteSheet(texture);
		walk.addFrame(sf::IntRect(0, 0, 61, 81));
		walk.addFrame(sf::IntRect(62, 13, 50, 45));
		walk.addFrame(sf::IntRect(112, 0, 63, 80));
		walk.addFrame(sf::IntRect(175, 13, 51, 45));
		

		image.loadFromFile("images/Citizens/guy1death.png");
		image.createMaskFromColor(sf::Color(255, 255, 255));
		textureDying.loadFromImage(image);

		dying.setSpriteSheet(textureDying);
		dying.addFrame(sf::IntRect(0, 7, 131, 84));
		dying.addFrame(sf::IntRect(63, 0, 131, 84));
		animSprite.setScale(0.6, 0.6);
	}
	if (type == 2)
	{
		image.loadFromFile("images/Citizens/guy2walk.png");
		image.createMaskFromColor(sf::Color(255, 255, 255));
		texture.loadFromImage(image);

		walk.setSpriteSheet(texture);
		walk.addFrame(sf::IntRect(0, 0, 45, 60));
		walk.addFrame(sf::IntRect(45, 16, 37, 34));
		walk.addFrame(sf::IntRect(82, 0, 46, 60));
		walk.addFrame(sf::IntRect(128, 16, 37, 34));
		animSprite.setScale(0.7, 0.7);

		image.loadFromFile("images/Citizens/guy2death.png");
		image.createMaskFromColor(sf::Color(255, 255, 255));
		textureDying.loadFromImage(image);

		dying.setSpriteSheet(textureDying);
		dying.addFrame(sf::IntRect(0, 0, 43, 88));
		dying.addFrame(sf::IntRect(43, 9, 79, 70));
		animSprite.setScale(0.7, 0.7);
	}
	if (type == 3)
	{
		image.loadFromFile("images/Citizens/guy3walk.png");
		image.createMaskFromColor(sf::Color(255, 255, 255));
		texture.loadFromImage(image);

		walk.setSpriteSheet(texture);
		walk.addFrame(sf::IntRect(0, 0, 47, 60));
		walk.addFrame(sf::IntRect(47, 15, 38, 34));
		walk.addFrame(sf::IntRect(85, 0, 45, 60));
		walk.addFrame(sf::IntRect(130, 17, 38, 34));

		image.loadFromFile("images/Citizens/guy3death.png");
		image.createMaskFromColor(sf::Color(255, 255, 255));
		textureDying.loadFromImage(image);

		dying.setSpriteSheet(textureDying);
		dying.addFrame(sf::IntRect(0, 0, 54, 68));
		dying.addFrame(sf::IntRect(54, 0, 76, 75));
		animSprite.setScale(0.7, 0.7);
	}
		
	
	std::vector<Object> vec = map.GetObjects("spawnArea");

	int num = rand() % vec.size();
	position = sf::Vector2f( (float) (rand() % (int)vec[num].rect.width + vec[num].rect.left), (float) (rand() % (int)vec[num].rect.height + vec[num].rect.top));
	
	//position = sf::Vector2f(300, 300);
	speedVec = sf::Vector2f(0, 0);
	currentAnimation = &walk;
	rotationAngle = 0;
	//animSprite.setFrame(rand() % 4, true);
	isAlife = true;
	isDying = false;

	dyingTime = sf::seconds(0.3);
}

void Citizen::update(sf::Time dt){
	if (isAlife) 
	{
		position += speedVec * dt.asSeconds();
		currentAnimation = &walk;

	}
	if (isDying)
	{
		currentAnimation = &dying;
		animSprite.setLooped(false);
		dyingTime -= dt;
		if (dyingTime <= sf::Time::Zero)
		{
			animSprite.stop();
			isAlife = false;
		}
	}
	
	animSprite.setPosition(position);
	animSprite.setRotation(rotationAngle);
	animSprite.update(dt);
	animSprite.play(*currentAnimation);
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


void Citizen::collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation) {
	if (animSprite.getGlobalBounds().intersects(playerRect)) 
	{
		rotationAngle = playerRotation;
		speedVec = playerSpeedVec;
		isDying = true;
	}
}

