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
	
	for (int i = 0; i < 100; i++) {
		citizensList.push_back(new Citizen(map));
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