#pragma once
#include <SFML\Graphics.hpp>

sf::View camera;


sf::FloatRect getVisibleArea()
{ 
	return sf::FloatRect(sf::Vector2f(camera.getCenter().x - camera.getSize().x / 2, camera.getCenter().y - camera.getSize().y / 2), camera.getSize());
}

void setUpCamera(float x, float y) {
	float tempX = x; float tempY = y;

	if (x < 640) tempX = 640;	//������� �� ���� ����� �������
	if (x > 2560) tempX = 2560;	//������� �� ���� ������ �������
	if (y < 400) tempY = 400;	//������� �������
	if (y > 2800) tempY = 2800;	//������ �������	

	camera.setCenter(tempX, tempY); //������ �� �������, ��������� ��� ����������. 
}