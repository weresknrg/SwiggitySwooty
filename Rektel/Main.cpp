#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include "level.h"
#include "Camera.h"


int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 800), "rektel");
	sf::Clock clock;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 60.f);
	
	Level map;
	map.LoadFromFile("levels/lev1.tmx");
	
	sf::Image heroImage;
	heroImage.loadFromFile("images/car_tex.png");
	
	Object player = map.GetObject("player");
	Player p(heroImage, map, player.rect.left+player.rect.width/2, player.rect.top+player.rect.height/2, "player");


	camera.reset(sf::FloatRect(0, 0, 1280, 800));

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
			p.update(ups.asSeconds());
			window.setView(camera);
			setCameraPosition(p.getPosition().x, p.getPosition().y);
		}
		map.Draw(window);
		p.draw(window);
		window.display();
		window.clear();
		accumulator += clock.restart();
	}

	return 0;
}