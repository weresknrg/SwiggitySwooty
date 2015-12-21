#include "Citizen.h"



Citizen::Citizen(sf::Image Image, std::vector<sf::IntRect> &coordForFrames, Level &lev, sf::Vector2f stPos)
{
	Image.createMaskFromColor(sf::Color(255, 255, 255));
	texture.loadFromImage(Image);
	
	walk.setSpriteSheet(texture);
	rotationAngle = 0;
	std::vector<sf::IntRect>::iterator iter; 
	int counter = 0;
	for (iter = coordForFrames.begin(); iter != coordForFrames.end(); iter++)
	{
		walk.addFrame(coordForFrames[counter]);
		counter++;
	}
	position = stPos;
	isAlife = true;
	speed = 1;
	//animatedSprite = AnimatedSprite(sf::seconds(0.2), true, false);
	animatedSprite.setAnimation(walk);
	currentAnimation = &walk;
}

void Citizen::update(sf::Time dt){
	speedVec = speed * sf::Vector2f(sinf(rotationAngle* 3.14159265f / 180.0f), cosf(rotationAngle * 3.14159265f / 180.0f));
	position += speedVec;
	animatedSprite.play(*currentAnimation);
	animatedSprite.setPosition(position);
	animatedSprite.setRotation(rotationAngle);
	animatedSprite.update(dt);
}

Citizen::~Citizen()
{
}

void Citizen::draw(sf::RenderTarget &renderTarget) {
	renderTarget.draw(animatedSprite);
}