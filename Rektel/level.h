#pragma once


#include <string>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <iostream>
#include "TinyXML/tinyxml.h"

struct Object
{
	int GetPropertyInt(std::string name);//����� �������� ������� � ����� ������
	float GetPropertyFloat(std::string name);
	std::string GetPropertyString(std::string name);

	std::string name;//�������� ���������� name ���� string
	std::string type;//� ����� ���������� type ���� string
	sf::Rect<float> rect;//��� Rect � �������� ����������
	std::map<std::string, std::string> properties;//������ ������������ ������. ���� - ��������� ���, �������� - ���������

	sf::Sprite sprite;//�������� ������
};

struct Layer//����
{
	int opacity;//�������������� ����
	sf::VertexArray m_vertices;//���������� � ������ �����
};

class Level : public sf::Drawable, public sf::Transformable //������� ����� - �������
{
public:
	bool LoadFromFile(std::string filename);//���������� false ���� �� ���������� ���������
	Object GetObject(std::string name);
	std::vector<Object> GetObjects(std::string name);//������ ������ � ��� �������
	std::vector<Object> GetAllObjects();//������ ��� ������� � ��� �������
	virtual void Level::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
			// apply the transform
		states.transform *= getTransform();

		// apply the tileset texture
		states.texture = &tilesetImage;

			// draw the vertex array
		target.draw(layers[0].m_vertices, states);
		}//������ � ����
		
		sf::Vector2i GetTileSize();//�������� ������ �����

	private:
		int width, height, tileWidth, tileHeight;//� tmx ����� width height � ������,����� ������ �����
		int firstTileID;//�������� ���� ������� �����
		sf::Rect<float> drawingBounds;//������ ����� ����� ������� ������
		sf::Texture tilesetImage;//�������� �����
		std::vector<Object> objects;//������ ���� �������, ������� �� �������
		std::vector<Layer> layers;
	};

	///////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

