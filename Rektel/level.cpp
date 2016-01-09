
#include "level.h"

int Object::GetPropertyInt(std::string name)//возвращаем номер свойства в нашем списке
{
	return atoi(properties[name].c_str());
}

float Object::GetPropertyFloat(std::string name)
{
	return strtod(properties[name].c_str(), NULL);
}

std::string Object::GetPropertyString(std::string name)//получить имя в виде строки.вроде понятно
{
	return properties[name];
}

bool Level::LoadFromFile(std::string filename)//двоеточия-обращение к методам класса вне класса 
{
	TiXmlDocument levelFile(filename.c_str());//загружаем файл в TiXmlDocument

											  // загружаем XML-карту
	if (!levelFile.LoadFile()) //если не удалось загрузить карту
	{
		std::cout << "Loading level \"" << filename << "\" failed." << std::endl;//выдаем ошибку
		return false;
	}

	// работаем с контейнером map
	TiXmlElement *map;
	map = levelFile.FirstChildElement("map");

	// пример карты: <map version="1.0" orientation="orthogonal"
	// width="10" height="10" tilewidth="34" tileheight="34">
	width = atoi(map->Attribute("width"));//извлекаем из нашей карты ее свойства
	height = atoi(map->Attribute("height"));//те свойства, которые задавали при работе в 
	tileWidth = atoi(map->Attribute("tilewidth"));//тайлмап редакторе
	tileHeight = atoi(map->Attribute("tileheight"));

	// Берем описание тайлсета и идентификатор первого тайла
	TiXmlElement *tilesetElement;
	tilesetElement = map->FirstChildElement("tileset");
	firstTileID = atoi(tilesetElement->Attribute("firstgid"));

	// source - путь до картинки в контейнере image
	TiXmlElement *image;
	image = tilesetElement->FirstChildElement("image");
	std::string imagepath = image->Attribute("source");

	// пытаемся загрузить тайлсет
	sf::Image img;

	if (!img.loadFromFile(imagepath))
	{
		std::cout << "Failed to load tile sheet." << std::endl;//если не удалось загрузить тайлсет-выводим ошибку в консоль
		return false;
	}


	img.createMaskFromColor(sf::Color(255, 255, 255));//для маски цвета.сейчас нет маски
	tilesetImage.loadFromImage(img);
	tilesetImage.setSmooth(false);//сглаживание

								  // получаем количество столбцов и строк тайлсета
	int columns = tilesetImage.getSize().x / tileWidth;
	int rows = tilesetImage.getSize().y / tileHeight;

	// вектор из прямоугольников изображений (TextureRect)
	std::vector<sf::Rect<int>> subRects;

	for (int y = 0; y < rows; y++)
		for (int x = 0; x < columns; x++)
		{
			sf::Rect<int> rect;

			rect.top = y * tileHeight;
			rect.height = tileHeight;
			rect.left = x * tileWidth;
			rect.width = tileWidth;

			subRects.push_back(rect);
		}

	// работа со слоями
	TiXmlElement *layerElement;
	layerElement = map->FirstChildElement("layer");
	while (layerElement)
	{
		Layer layer;
		layer.m_vertices.setPrimitiveType(sf::Quads);
		layer.m_vertices.resize(width * height * 4);
		// если присутствует opacity, то задаем прозрачность слоя, иначе он полностью непрозрачен
		if (layerElement->Attribute("opacity") != NULL)
		{
			float opacity = strtod(layerElement->Attribute("opacity"), NULL);
			layer.opacity = 255 * opacity;
		}
		else
		{
			layer.opacity = 255;
		}

		//  контейнер <data> 
		TiXmlElement *layerDataElement;
		layerDataElement = layerElement->FirstChildElement("data");

		if (layerDataElement == NULL)
		{
			std::cout << "Bad map. No layer information found." << std::endl;
		}

		//  контейнер <tile> - описание тайлов каждого слоя
		TiXmlElement *tileElement;
		tileElement = layerDataElement->FirstChildElement("tile");

		if (tileElement == NULL)
		{
			std::cout << "Bad map. No tile information found." << std::endl;
			return false;
		}

		int x = 0;
		int y = 0;

		while (tileElement)
		{
			int tileGID = atoi(tileElement->Attribute("gid"));
			int subRectToUse = tileGID - firstTileID;

			// Устанавливаем TextureRect каждого тайла
			if (subRectToUse >= 0)
			{
				// get a pointer to the current tile's quad
				sf::Vertex* quad = &layer.m_vertices[(x + y * 100) * 4];

				// define its 4 corners
				quad[0].position = sf::Vector2f(x * tileWidth, y * tileHeight);
				quad[1].position = sf::Vector2f((x + 1) * tileWidth, y * tileHeight);
				quad[2].position = sf::Vector2f((x + 1) * tileWidth, (y + 1) * tileHeight);
				quad[3].position = sf::Vector2f(x * tileWidth, (y + 1) * tileHeight);

				// define its 4 texture coordinates
				quad[0].texCoords = sf::Vector2f(subRects[subRectToUse].left, subRects[subRectToUse].top);
				quad[1].texCoords = sf::Vector2f(subRects[subRectToUse].left + subRects[subRectToUse].width, subRects[subRectToUse].top);
				quad[2].texCoords = sf::Vector2f(subRects[subRectToUse].left + subRects[subRectToUse].width, subRects[subRectToUse].top + subRects[subRectToUse].height);
				quad[3].texCoords = sf::Vector2f(subRects[subRectToUse].left, subRects[subRectToUse].top + subRects[subRectToUse].height);


				//закидываем в слой спрайты тайлов
			}

			tileElement = tileElement->NextSiblingElement("tile");

			x++;
			if (x >= width)
			{
				x = 0;
				y++;
				if (y >= height)
					y = 0;
			}
		}

		layers.push_back(layer);

		layerElement = layerElement->NextSiblingElement("layer");
	}

	// работа с объектами
	TiXmlElement *objectGroupElement;

	// если есть слои объектов
	if (map->FirstChildElement("objectgroup") != NULL)
	{
		objectGroupElement = map->FirstChildElement("objectgroup");
		while (objectGroupElement)
		{
			//  контейнер <object>
			TiXmlElement *objectElement;
			objectElement = objectGroupElement->FirstChildElement("object");

			while (objectElement)
			{
				// получаем все данные - тип, имя, позиция, и тд
				std::string objectType;
				if (objectElement->Attribute("type") != NULL)
				{
					objectType = objectElement->Attribute("type");
				}
				std::string objectName;
				if (objectElement->Attribute("name") != NULL)
				{
					objectName = objectElement->Attribute("name");
				}
				int x = atoi(objectElement->Attribute("x"));
				int y = atoi(objectElement->Attribute("y"));

				int width, height;

				sf::Sprite sprite;
				sprite.setTexture(tilesetImage);
				sprite.setTextureRect(sf::Rect<int>(0, 0, 0, 0));
				sprite.setPosition(x, y);

				if (objectElement->Attribute("width") != NULL)
				{
					width = atoi(objectElement->Attribute("width"));
					height = atoi(objectElement->Attribute("height"));
				}
				else
				{
					width = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].width;
					height = subRects[atoi(objectElement->Attribute("gid")) - firstTileID].height;
					sprite.setTextureRect(subRects[atoi(objectElement->Attribute("gid")) - firstTileID]);
				}

				// экземпляр объекта
				Object object;
				object.name = objectName;
				object.type = objectType;
				object.sprite = sprite;

				sf::Rect <float> objectRect;
				objectRect.top = y;
				objectRect.left = x;
				objectRect.height = height;
				objectRect.width = width;
				object.rect = objectRect;

				// "переменные" объекта
				TiXmlElement *properties;
				properties = objectElement->FirstChildElement("properties");
				if (properties != NULL)
				{
					TiXmlElement *prop;
					prop = properties->FirstChildElement("property");
					if (prop != NULL)
					{
						while (prop)
						{
							std::string propertyName = prop->Attribute("name");
							std::string propertyValue = prop->Attribute("value");

							object.properties[propertyName] = propertyValue;

							prop = prop->NextSiblingElement("property");
						}
					}
				}


				objects.push_back(object);

				objectElement = objectElement->NextSiblingElement("object");
			}
			objectGroupElement = objectGroupElement->NextSiblingElement("objectgroup");
		}
	}
	else
	{
		std::cout << "No object layers found..." << std::endl;
	}

	for (int i = 0; i < objects.size(); i++)
	{
		if (objects[i].name == "building")
			buildings.push_back(objects[i].rect);
	}

	return true;
}

Object Level::GetObject(std::string name)
{
	// только первый объект с заданным именем
	for (int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			return objects[i];
}

std::vector<Object> Level::GetObjects(std::string name)
{
	// все объекты с заданным именем
	std::vector<Object> vec;
	for (int i = 0; i < objects.size(); i++)
		if (objects[i].name == name)
			vec.push_back(objects[i]);

	return vec;
}


std::vector<Object> Level::GetAllObjects()
{
	return objects;
};


sf::Vector2i Level::GetTileSize()
{
	return sf::Vector2i(tileWidth, tileHeight);
}

void Level::drawBuildings(sf::Vector2f cameraCenter, sf::RenderTarget &window)
{
	for (int i = 0; i < buildings.size(); i++)
	{
		sf::Vector2f distance(cameraCenter.x - buildings[i].left - buildings[i].width / 2, cameraCenter.y - buildings[i].top - buildings[i].height / 2);
		sf::Vector2f buildShift;
		buildShift.x = 1.5 * sqrt(abs(distance.x)) * sgn(distance.x);
		buildShift.y = 1.5 * sqrt(abs(distance.y)) * sgn(distance.y);

		sf::VertexArray edgeRight(sf::Quads, 4);

		edgeRight[0].position = sf::Vector2f(buildings[i].left + buildings[i].width - buildShift.x, buildings[i].top - buildShift.y);
		edgeRight[1].position = sf::Vector2f(buildings[i].left + buildings[i].width - buildShift.x, buildings[i].top + buildings[i].height - buildShift.y);
		edgeRight[3].position = sf::Vector2f(buildings[i].left + buildings[i].width, buildings[i].top);
		edgeRight[2].position = sf::Vector2f(buildings[i].left + buildings[i].width, buildings[i].top + buildings[i].height);

		edgeRight[0].color = sf::Color(75,58,42);
		edgeRight[1].color = edgeRight[2].color = edgeRight[3].color = edgeRight[0].color;


		sf::VertexArray edgeDown(sf::Quads, 4);

		edgeDown[0].position = sf::Vector2f(buildings[i].left, buildings[i].top + buildings[i].height);
		edgeDown[1].position = sf::Vector2f(buildings[i].left - buildShift.x, buildings[i].top + buildings[i].height - buildShift.y);
		edgeDown[2].position = sf::Vector2f(buildings[i].left + buildings[i].width - buildShift.x, buildings[i].top + buildings[i].height - buildShift.y);
		edgeDown[3].position = sf::Vector2f(buildings[i].left + buildings[i].width, buildings[i].top + buildings[i].height);

		edgeDown[0].color = sf::Color(26, 20, 14);
		edgeDown[1].color = edgeDown[2].color = edgeDown[3].color = edgeDown[0].color;

		sf::VertexArray edgeLeft(sf::Quads, 4);

		edgeLeft[0].position = sf::Vector2f(buildings[i].left, buildings[i].top + buildings[i].height);
		edgeLeft[1].position = sf::Vector2f(buildings[i].left - buildShift.x, buildings[i].top + buildings[i].height - buildShift.y);
		edgeLeft[2].position = sf::Vector2f(buildings[i].left - buildShift.x, buildings[i].top - buildShift.y);
		edgeLeft[3].position = sf::Vector2f(buildings[i].left, buildings[i].top);

		edgeLeft[0].color = sf::Color(20, 7, 5);
		edgeLeft[1].color = edgeLeft[2].color = edgeLeft[3].color = edgeLeft[0].color;

		sf::VertexArray edgeUp(sf::Quads, 4);

		edgeUp[0].position = sf::Vector2f(buildings[i].left, buildings[i].top);
		edgeUp[1].position = sf::Vector2f(buildings[i].left - buildShift.x, buildings[i].top - buildShift.y);
		edgeUp[2].position = sf::Vector2f(buildings[i].left + buildings[i].width - buildShift.x, buildings[i].top - buildShift.y);
		edgeUp[3].position = sf::Vector2f(buildings[i].left + buildings[i].width, buildings[i].top);

		edgeUp[0].color = sf::Color(59, 45, 33);;
		edgeUp[1].color = edgeUp[2].color = edgeUp[3].color = edgeUp[0].color;

		sf::VertexArray edgeTop(sf::Quads, 4);

		edgeTop[0].position = sf::Vector2f(buildings[i].left - buildShift.x, buildings[i].top + buildings[i].height - buildShift.y);
		edgeTop[1].position = sf::Vector2f(buildings[i].left - buildShift.x, buildings[i].top - buildShift.y);
		edgeTop[2].position = sf::Vector2f(buildings[i].left + buildings[i].width - buildShift.x, buildings[i].top - buildShift.y);
		edgeTop[3].position = sf::Vector2f(buildings[i].left + buildings[i].width - buildShift.x, buildings[i].top + buildings[i].height - buildShift.y);

		edgeTop[0].color = sf::Color(43, 33, 24);
		edgeTop[1].color = edgeTop[2].color = edgeTop[3].color = edgeTop[0].color;

		sf::VertexArray edgeTopCenter(sf::Quads, 4);

		edgeTopCenter[0].position = sf::Vector2f(buildings[i].left + 5 - buildShift.x, buildings[i].top + buildings[i].height - 5 - buildShift.y);
		edgeTopCenter[1].position = sf::Vector2f(buildings[i].left + 5 - buildShift.x, buildings[i].top + 5 - buildShift.y);
		edgeTopCenter[2].position = sf::Vector2f(buildings[i].left - 5 + buildings[i].width - buildShift.x, buildings[i].top + 5 - buildShift.y);
		edgeTopCenter[3].position = sf::Vector2f(buildings[i].left - 5 + buildings[i].width - buildShift.x, buildings[i].top - 5 + buildings[i].height - buildShift.y);

		edgeTopCenter[0].color = sf::Color(90, 85, 80);
		edgeTopCenter[1].color = edgeTopCenter[2].color = edgeTopCenter[3].color = edgeTopCenter[0].color;

		if (distance.y > 0) {
			window.draw(edgeUp);
			window.draw(edgeRight);
			window.draw(edgeLeft);
			window.draw(edgeDown);
			window.draw(edgeTop);
			window.draw(edgeTopCenter);
		}
		else
		{
			window.draw(edgeDown);
			window.draw(edgeRight);
			window.draw(edgeLeft);
			window.draw(edgeUp);
			window.draw(edgeTop);
			window.draw(edgeTopCenter);
		}
	}
}

int Level::sgn(float val) {
	return (0.f < val) - (val < 0.f);
}


/////////////////////////////////////////\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\
