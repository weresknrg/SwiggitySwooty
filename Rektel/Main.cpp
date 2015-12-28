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
#include "Camera.h"

void initTextures(sf::Texture* guyWalkTypes, sf::Texture* guyDyingTypes, sf::Texture& guyExploding);

sf::RenderWindow window(sf::VideoMode(1280, 800), "rektel");
int main()
{
	srand(time(NULL));
	sf::Clock clock;
	sf::Clock spawnTankTimer;
	spawnTankTimer.restart();
	sf::Time accumulator = sf::Time::Zero;
	sf::Time ups = sf::seconds(1.f / 60.f);
	
	Level map;
	map.LoadFromFile("levels/testMap.tmx");
	// �������� ������
	sf::Image heroImage;
	heroImage.loadFromFile("images/car_tex.png");
	
	//�������� �����
	sf::Texture tankTex[2];
	sf::Texture bullet;
	sf::Image image;
	// ������
	image.loadFromFile("images/tankBase.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	tankTex[0].loadFromImage(image);
	//�����
	image.loadFromFile("images/tankTurret.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	tankTex[1].loadFromImage(image);
	//�������
	image.loadFromFile("images/bullet.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	bullet.loadFromImage(image);

	Tank* tank = 0;
	

	//������� ������
	Object player = map.GetObject("player");
	Player p(heroImage, map, sf::Vector2f(player.rect.left+player.rect.width/2, player.rect.top+player.rect.height/2), "player");
	camera.reset(sf::FloatRect(0, 0, 1280, 800));
	
	//������� ������
	std::list<Citizen*> citizensList;
	sf::Texture guyWalkTypes[4];
	sf::Texture guyDyingTypes[4];
	sf::Texture guyExploding;
	initTextures(guyWalkTypes, guyDyingTypes, guyExploding);

	for (int i = 0; i < 60; i++) 
	{
		//�������� ��������� ������� ������
		std::vector<Object> vec = map.GetObjects("spawnArea");
		int num = rand() % vec.size();
		//��������� ����� � �������
		sf::Vector2f pos = sf::Vector2f((float)(rand() % (int)vec[num].rect.width + vec[num].rect.left), (float)(rand() % (int)vec[num].rect.height + vec[num].rect.top));
		//�������� ��� ������, 4 ��� - ���
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
		
		while (accumulator > ups)
		{
			accumulator -= ups;
			//��������� ������
			p.update(ups);
			//��������� ��������� ������
			window.setView(camera);
			setUpCamera(p.getPosition().x, p.getPosition().y);
			
			//��������� ����	
			if (tank != 0) // ���� ���� ����������
			{					
				if (tank->isTankGone()) //������ �� ���� ������
				{
					if (!getVisibleArea().intersects(tank->getRect())) // �� ����� �� ��� ������
					{													//���� ��� �� �������
						delete tank;
						tank = 0;
						spawnTankTimer.restart();
					}
				}
				else // ����� ����� ����� �� ������� - ���������
				{
					tank->traceThePlayer(p.getPosition());
					tank->update(ups);
				}
			} 
			if(spawnTankTimer.getElapsedTime() >= sf::seconds(10) && tank == 0) //���� ���� �� ����������
			{                                                                // � ������ 10 ������ � ������� �������� - �������
				std::vector <Object> vec = map.GetObjects("tankSpawns");
				int tmp = rand() % vec.size(); // �������� ��������� �������
				sf::Vector2f stPos = sf::Vector2f((float)(rand() % (int)vec[tmp].rect.width + vec[tmp].rect.left), (float)(rand() % (int)vec[tmp].rect.height + vec[tmp].rect.top));
				if(!getVisibleArea().contains(stPos)) // ����� �� ������ ����� ������
					tank = new Tank(stPos, tankTex, &bullet, map); // ��� - ������� ����
			}

			// ��������� �������
			for (std::list<Citizen*>::iterator It = citizensList.begin(); It != citizensList.end();) 
			{	
				if ((*It)->checkIsAlife()) //���� ����� - ���������
				{
					(*It)->update(ups);
					(*It)->collisionWithPlayer(p.getRect(), p.getSpeedVec(), p.getRotation());
					It++;
				} 
				else //���� ��� - �������
				{
					It = citizensList.erase(It);
					// � ����� ������� ������ (��� �� ��� ����)
					int type = rand() % 3; // + �������� ���
					std::vector<Object> vec = map.GetObjects("spawnArea");
					int num = rand() % vec.size();
					sf::Vector2f pos = sf::Vector2f((float)(rand() % (int)vec[num].rect.width + vec[num].rect.left), (float)(rand() % (int)vec[num].rect.height + vec[num].rect.top));
					if (rand() % 2 == 0 && rand() % 2 == 0 && rand() % 2 == 0) type = 3;
					if(!getVisibleArea().contains(pos))
						citizensList.push_back(new Citizen(pos, type, map, &guyWalkTypes[type], &guyDyingTypes[type], &guyExploding));
				}
			}
		}

		//������ �����
		window.draw(map);
		//������
		p.draw(window);
		//�������
		for (std::list<Citizen*>::iterator It = citizensList.begin(); It != citizensList.end(); It++)
		{
				(*It)->draw(window);
		}
		//���� � ��� ����
		if(tank != 0)
			tank->draw(window);
		
		window.display();
		window.clear();
		accumulator += clock.restart();
	}

	return 0;
}

//��������� �������
void initTextures(sf::Texture* guyWalkTypes, sf::Texture* guyDyingTypes, sf::Texture& guyExploding) {

	sf::Image image;
	//�������� ������
	image.loadFromFile("images/Citizens/guy1walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[0].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy2walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[1].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy3walk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[2].loadFromImage(image);
	//���
	image.loadFromFile("images/Citizens/copWalk.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyWalkTypes[3].loadFromImage(image);
	
	//�������� ������ ��� �������
	image.loadFromFile("images/Citizens/guy1death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[0].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy2death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[1].loadFromImage(image);
	image.loadFromFile("images/Citizens/guy3death.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[2].loadFromImage(image);
	//��� (�������)
	image.loadFromFile("images/Citizens/copDeath.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyDyingTypes[3].loadFromImage(image);

	//�������� �����
	image.loadFromFile("images/Citizens/BSS.png");
	image.createMaskFromColor(sf::Color(255, 255, 255));
	guyExploding.loadFromImage(image);

}