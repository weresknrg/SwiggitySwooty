#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <math.h>
#include "level.h"
#include <vector>
#include <list>



int main()
{
	
	sf::View camera;
	sf::RenderWindow window(sf::VideoMode(1280, 800), "rektel");
	sf::Clock clock;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 60.f);
	
	Level lvl;
	lvl.LoadFromFile("lev1.tmx");
	sf::Image heroImage;
	heroImage.loadFromFile("images/car_tex.png");
	Player p(heroImage, 300, 300, "player");

	
	

	/*
	sf::Image map_image;
	map_image.loadFromFile("images/map.png");
	sf::Texture map;
	map.loadFromImage(map_image);
	sf::Sprite s_map;
	s_map.setTexture(map);
	*/
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
			camera.setCenter(p.sprite.getPosition());
			//AI();
			//Physics();
		}

		lvl.Draw(window);
		window.draw(p.sprite);
		window.display();
		window.clear();
		accumulator += clock.restart();
	}

	return 0;
}