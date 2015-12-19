#pragma once
#include <SFML\Graphics.hpp>

sf::View camera;
void setCameraPosition(float x, float y) { //функция для считывания координат игрока
	float tempX = x; float tempY = y;//считываем коорд игрока и проверяем их, чтобы убрать края

	if (x < 640) tempX = 640;	//убираем из вида левую сторону
	if (x > 2560) tempX = 2560;	//убираем из вида левую сторону
	if (y < 480) tempY = 480;	//верхнюю сторону
	if (y > 2800) tempY = 2800;	//нижнюю сторону	

	camera.setCenter(tempX, tempY); //следим за игроком, передавая его координаты. 
}