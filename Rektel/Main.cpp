#pragma once
#include <SFML/Graphics.hpp>
#include "Player.h"
#include <iostream>
#include <math.h>
#include <vector>
#include <list>
#include "level.h"
#include "Citizen.h"
#include "Tank.h"
#include "GUI.h"
#include "Camera.h"


void initTextures(sf::Texture* guyWalkTypes, sf::Texture* guyDyingTypes, sf::Texture& guyExploding);

sf::RenderWindow window(sf::VideoMode(1280, 800), "rektel");
int main()
{
	bool isGameOver = false;
	srand(time(NULL));
	sf::Clock clock;
	sf::Font font;

	// таймер появления танка
	sf::Clock spawnTankTimer;
	sf::Time gameTime = sf::Time::Zero;
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 60.f);
	//создаем GUI
	GUI gui(font);
	//создаем карту
	Level map;
	map.LoadFromFile("levels/testMap.tmx");
	// Текстуры игрока
	sf::Image heroImage;
	heroImage.loadFromFile("images/car_tex.png");
	
	//Текстуры танка
	sf::Texture tankTex[2];
	sf::Texture bullet;
	sf::Image image;
	// корпус
	image.loadFromFile("images/tankBase.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	tankTex[0].loadFromImage(image);
	//башня
	image.loadFromFile("images/tankTurret.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	tankTex[1].loadFromImage(image);
	//снаряды
	image.loadFromFile("images/bullet.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	bullet.loadFromImage(image);

	Tank* tank = 0;
	

	//создаем игрока
	Object player = map.GetObject("player");
	Player p(heroImage, map, sf::Vector2f(player.rect.left+player.rect.width/2, player.rect.top+player.rect.height/2));
	camera.reset(sf::FloatRect(0, 0, 1280, 800));
	
	//создаем жителей
	std::list<Citizen*> citizensList;
	sf::Texture guyWalkTypes[4];
	sf::Texture guyDyingTypes[4];
	sf::Texture guyExploding;
	initTextures(guyWalkTypes, guyDyingTypes, guyExploding);
	for (int i = 0; i < 40; i++) 
	{
		//выбираем случайные области спавна
		std::vector<Object> vec = map.GetObjects("spawnArea");
		int num = rand() % vec.size();
		//случайную точку в области
		sf::Vector2f pos = sf::Vector2f((float)(rand() % (int)vec[num].rect.width + vec[num].rect.left), (float)(rand() % (int)vec[num].rect.height + vec[num].rect.top));
		//выбираем тип жителя, 4 тип - коп
		int type = rand() % 3;
		if (rand() % 2 == 0 && rand() % 2 == 0 && rand() % 2 == 0) type = 3; // spawn cop 1/2 * 1/2 * 1/2 = 16.6% chance
		citizensList.push_back(new Citizen(pos, type, map, &guyWalkTypes[type], &guyDyingTypes[type], &guyExploding));
	}
	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		if(!isGameOver)
		{ 
			while (accumulator > ups)
			{
				accumulator -= ups;
				gameTime += ups;

				gui.updateData(getVisibleArea(), p.getRemainingTime(), p.getHomisides(), p.getLifes(), gameTime); // обновляем информацию в интерфейсе 

				//обновляем игрока
				p.update(ups);
				//обновляем положение камеры
				window.setView(camera);
				setUpCamera(p.getPosition().x, p.getPosition().y);

				//обновляем танк	
				if (tank != 0) // если танк существует
				{
					if (tank->isTankGone()) //должен ли танк уехать
					{
						if (!getVisibleArea().intersects(tank->getRect())) // не видит ли его камера
						{													//если нет то удаляем
							delete tank;
							tank = 0; 
							spawnTankTimer.restart();
						}
					}
					else // время жизни танка не истекло - обновляем
					{
						switch (tank->collisionWithPlayer(p.getSprite()))
						{
						case (2) : //столкнулся с пулей
						{
							p.decreaseLifes(1);
							break;
						}
						case (1) : // столкнулся с танком
						{
							p.decreaseLifes(2);
							break;
						}
						}

						tank->traceThePlayer(p.getPosition());
						tank->update(ups);
					}
				}
				if (spawnTankTimer.getElapsedTime() >= sf::seconds(10) && tank == 0) //если танк не существует
				{                                                                // и прошло 10 секунд с момента удаления - создаем
					std::vector <Object> vec = map.GetObjects("tankSpawns");
					int tmp = rand() % vec.size(); // выбираем случайную позицию
					sf::Vector2f stPos = sf::Vector2f((float)(rand() % (int)vec[tmp].rect.width + vec[tmp].rect.left), (float)(rand() % (int)vec[tmp].rect.height + vec[tmp].rect.top));
					if (!getVisibleArea().contains(stPos)) // видит ли камера точку спавна
						tank = new Tank(stPos, tankTex, &bullet, map); // нет - создаем танк
				}

				// обновляем жителей
				for (std::list<Citizen*>::iterator It = citizensList.begin(); It != citizensList.end();)
				{
					if ((*It)->checkIsAlife()) //если живой 
					{
						(*It)->update(ups); // обновляем
						if ((*It)->collisionWithPlayer(p.getRect(), p.getSpeedVec(), p.getRotation())) //если сбит игроком 
						{
							p.increaseHomisideCounter(); //увеличиваем счетчик убийств
							if ((*It)->getType() == 3) p.increaseLifeTime(sf::seconds(-2)); //если коп - отнимаем оставшееся время
							else p.increaseLifeTime(sf::seconds(3)); // если житель - добавляем
						}
						It++; //переходим к следующему
					}
					else //если нет - удаляем
					{
						It = citizensList.erase(It);
						// и сразу создаем нового (так же как танк)
						int type = rand() % 3; // + выбираем тип
						std::vector<Object> vec = map.GetObjects("spawnArea");
						int num = rand() % vec.size();
						sf::Vector2f pos = sf::Vector2f((float)(rand() % (int)vec[num].rect.width + vec[num].rect.left), (float)(rand() % (int)vec[num].rect.height + vec[num].rect.top));
						if (rand() % 2 == 0 && rand() % 2 == 0 && rand() % 2 == 0) type = 3;
						if (!getVisibleArea().contains(pos))
							citizensList.push_back(new Citizen(pos, type, map, &guyWalkTypes[type], &guyDyingTypes[type], &guyExploding));
					}
				}
				if (p.getRemainingTime() <= sf::Time::Zero || p.getLifes() <= 0) // если время у игрока кончилось
					isGameOver = true; // заканчиваем игру
			}

			//рисуем карту
			window.draw(map);
			//игрока
			p.draw(window);
			//жителей
			for (std::list<Citizen*>::iterator It = citizensList.begin(); It != citizensList.end(); It++)
			{
				(*It)->draw(window);
			}
			//танк если существует и его пули
			if (tank != 0)
				tank->draw(window);
			gui.draw(window);
			accumulator += clock.restart();
		} 
		else // если игра закончилась
		{ //выводим успехи игрока
			sf::Text gameOver("Game Over", font, 50);
			gameOver.setPosition(camera.getCenter().x - 250, camera.getCenter().y - 100); 
			sf::Text killed("Killed        \t" + std::to_string(p.getHomisides()), font, 20);
			killed.setPosition(camera.getCenter().x - 200, camera.getCenter().y - 20 );
			sf::Text gameTime("Game Time \t " + std::to_string((int)gameTime.asSeconds()), font, 20);
			gameTime.setPosition(camera.getCenter().x - 200, camera.getCenter().y+20);
			window.draw(gameOver); // "конец игры"
			window.draw(killed); // кол-во убитых
			window.draw(gameTime); //время в игре
		}
		window.display();
		window.clear();
	}

	return 0;
}

//Установка текстур
void initTextures(sf::Texture* guyWalkTypes, sf::Texture* guyFallingTypes, sf::Texture& guyExploding) {

	sf::Image image;
	//текстуры ходьбы
	image.loadFromFile("images/Citizens/guy1walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[0].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy2walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[1].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy3walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[2].loadFromImage(image);
	//коп
	image.loadFromFile("images/Citizens/copWalk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[3].loadFromImage(image);
	
	//текстуры смерти для житилей
	image.loadFromFile("images/Citizens/guy1death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyFallingTypes[0].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy2death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyFallingTypes[1].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy3death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyFallingTypes[2].loadFromImage(image);
	//коп (смертть)
	image.loadFromFile("images/Citizens/copDeath.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyFallingTypes[3].loadFromImage(image);

	//кровавый взрыв
	image.loadFromFile("images/Citizens/BSS.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyExploding.loadFromImage(image);

}