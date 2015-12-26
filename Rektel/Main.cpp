#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include "level.h"
#include "Citizen.h"
#include "Camera.h"

void initTextures(sf::Texture* guyWalkTypes, sf::Texture* guyDyingTypes, sf::Texture& guyExploding);

sf::RenderWindow window(sf::VideoMode(1280, 800), "rektel");
int main()
{
	sf::Clock clock;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 60.f);
	
	Level map;
	map.LoadFromFile("levels/testMap.tmx");
	
	sf::Image heroImage;
	heroImage.loadFromFile("images/car_tex.png");
	
	Object player = map.GetObject("player");
	Player p(heroImage, map, sf::Vector2f(player.rect.left+player.rect.width/2, player.rect.top+player.rect.height/2), "player");
	camera.reset(sf::FloatRect(0, 0, 1280, 800));

	std::list<Citizen*> citizensList;

	sf::Texture guyWalkTypes[3];
	sf::Texture guyDyingTypes[3];
	sf::Texture guyExploding;
	initTextures(guyWalkTypes, guyDyingTypes, guyExploding);

	for (int i = 0; i < 100; i++) {
		int type = rand() % 2;
		citizensList.push_back(new Citizen(type, map, &guyWalkTypes[type], &guyDyingTypes[type], &guyExploding));
	}
	
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}
		
		while (accumulator > ups)
		{
			accumulator -= ups;
			p.update(ups);
			window.setView(camera);
			setUpCamera(p.getPosition().x, p.getPosition().y);
			std::list<Citizen*>::iterator It;
			for (It = citizensList.begin(); It != citizensList.end();) 
			{	
				if ((*It)->checkIsAlife()) {
					(*It)->update(ups);
					(*It)->collisionWithPlayer(p.getRect(), p.getSpeedVec(), p.getRotation());
					It++;
				} 
				else
				{
					It = citizensList.erase(It);
				}
			}

		}

		map.Draw(window);
		p.draw(window);
		for (std::list<Citizen*>::iterator It = citizensList.begin(); It != citizensList.end(); It++)
		{
			(*It)->draw(window);
		}
		window.display();
		window.clear();
		accumulator += clock.restart();
	}

	return 0;
}

void initTextures(sf::Texture* guyWalkTypes, sf::Texture* guyDyingTypes, sf::Texture& guyExploding) {

	sf::Image image;
	
	image.createMaskFromColor(sf::Color(255, 255, 255));

	image.loadFromFile("images/Citizens/guy1walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[0].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy2walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[1].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy3walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[2].loadFromImage(image);

	image.loadFromFile("images/Citizens/guy1death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[0].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy2death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[1].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy3death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[2].loadFromImage(image);

	image.loadFromFile("images/Citizens/BSS.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyExploding.loadFromImage(image);

}