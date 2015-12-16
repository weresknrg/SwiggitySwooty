
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <math.h>
#include "Camera.h"
#include "Map.h"

void interactionWithMap(Player p);

int main()
{
	sf::RenderWindow window(sf::VideoMode(1280, 800), "SFML works!");
	sf::Clock clock;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 60.f);
	Player p("car_tex.png");

	sf::Image map_image;
	map_image.loadFromFile("images/map.png");
	sf::Texture map;
	map.loadFromImage(map_image);
	sf::Sprite s_map;
	s_map.setTexture(map);
	camera.reset(sf::FloatRect(0, 0, 640, 400));

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
		
		for (int i = 0; i < HEIGHT_MAP; i++)
			for (int j = 0; j < WIDTH_MAP; j++)
			{
				if (TileMap[i][j] == 'd')  
				{
					sf::RectangleShape rect(sf::Vector2f(56, 56));
					rect.setPosition(56 * j, 56 * i);
					rect.setFillColor(sf::Color(101,67,33));
					window.draw(rect);
				}
				if (TileMap[i][j] == ' ') {
					s_map.setTextureRect(sf::IntRect(178, 1, 56, 56));
					s_map.setPosition(j * 56, i * 56);
					window.draw(s_map);
				}
			}
		window.draw(p.sprite);
		window.display();
		window.clear();
		accumulator += clock.restart();
	}

	return 0;
}