#pragma once
#include <SFML\Graphics.hpp>

sf::View camera;
void setUpCamera(float x, float y) { 
	float tempX = x; float tempY = y;

	if (x < 640) tempX = 640;	//убираем из вида левую сторону
	if (x > 2560) tempX = 2560;	//убираем из вида правую сторону
	if (y < 400) tempY = 400;	//верхнюю сторону
	if (y > 2800) tempY = 2800;	//нижнюю сторону	

	camera.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
}