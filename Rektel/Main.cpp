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
	map.LoadFromFile("levels/lev1.tmx");
	
	sf::Image heroImage;
	heroImage.loadFromFile("images/car_tex.png");
	
	Object player = map.GetObject("player");
	Player p(heroImage, map, sf::Vector2f(player.rect.left+player.rect.width/2, player.rect.top+player.rect.height/2), "player");
	camera.reset(sf::FloatRect(0, 0, 1280, 800));

	std::list<Citizen> citizensList;
	std::list<Citizen>::iterator citizensIter;
	std::vector<sf::IntRect> coordForAnim;
	std::vector<sf::Image> animImages;

	sf::Image citizen;
	citizen.createMaskFromColor(sf::Color(255, 255, 255));
	citizen.loadFromFile("images/Citizens/1walk.png");
	animImages.push_back(citizen);
	coordForAnim.push_back(sf::IntRect(0, 0, 61, 81));
	coordForAnim.push_back(sf::IntRect(62, 13, 50, 45));
	coordForAnim.push_back(sf::IntRect(112, 0, 63, 80));
	coordForAnim.push_back(sf::IntRect(175, 13, 51, 45));
	
	citizensList.push_back(Citizen(animImages[0], coordForAnim, map, sf::Vector2f(1000, 2500)));
	

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
			for (citizensIter = citizensList.begin(); citizensIter != citizensList.end(); citizensIter++) {
				citizensIter->update(ups);
			}
		}
		map.Draw(window);
		p.draw(window);
		
		for (citizensIter = citizensList.begin(); citizensIter != citizensList.end(); citizensIter++) {
			citizensIter->draw(window);
		}

		window.display();
		window.clear();
		accumulator += clock.restart();
	}

	return 0;
}