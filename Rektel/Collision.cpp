#include <math.h>
#include "Collision.h"

// реализация SAT алгоритма
void Collision::normalize(sf::Vector2f& vector)  //нормирование вектора (делим компоненты на длину)
{
	const float length = sqrt(vector.x * vector.x + vector.y * vector.y);
	if (length == 0)
		return;
	vector.x = vector.x / length;
	vector.y = vector.y / length;
}

float Collision::dot(sf::Vector2f& vector1, sf::Vector2f& vector2) // Скалярное произведение
{
	return vector1.x*vector2.x + vector1.y*vector2.y;
}

float Collision::distance(float minA, float maxA, float minB, float maxB) // расстояние, если + то пересечений нет
{
	if (minB > maxA) return minB - maxA;
	else return minA - maxB;
}


void  Collision::project(sf::Vector2f& axis, sf::VertexArray &_rectangle, float &min, float &max) // проекция одного вектора на другой
{
	float tempDOT = dot(axis, _rectangle[0].position);

	min = tempDOT;
	max = tempDOT;

	for (short i = 1; i < 4; i++)
	{
		tempDOT = dot(_rectangle[i].position, axis);

		if (tempDOT<min)
			min = tempDOT;
		else if (tempDOT>max)
			max = tempDOT;
	}
}

bool Collision::checkCollision(sf::Sprite &s1, sf::FloatRect &s2)
{
	sf::VertexArray one(sf::Quads, 4);
	sf::VertexArray two(sf::Quads, 4);

	sf::Transform transform = s1.getTransform();
	sf::FloatRect rect = s1.getLocalBounds();

	//one[0] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
	one[0] = transform.transformPoint(sf::Vector2f(rect.left, rect.top + rect.height));
	one[1] = transform.transformPoint(sf::Vector2f(rect.left, rect.top));
	one[2] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top));
	one[3] = transform.transformPoint(sf::Vector2f(rect.left + rect.width, rect.top + rect.height));

	two[0] = sf::Vector2f(s2.left, s2.top + s2.height);
	two[1] = sf::Vector2f(s2.left, s2.top);
	two[2] = sf::Vector2f(s2.left + s2.width, s2.top);
	two[3] = sf::Vector2f(s2.left + s2.width, s2.top + s2.height);


	sf::Vector2f axis;
	float minA;
	float minB;
	float maxA;
	float maxB;


	// Проверка 1 оси
	axis.x = one[1].position.x - one[0].position.x;
	axis.y = one[1].position.y - one[0].position.y;
	Collision::normalize(axis);

	project(axis, one, minA, maxA);
	project(axis, two, minB, maxB);

	if (distance(minA, maxA, minB, maxB)>0.f)
		return false;


	// 2
	axis.x = one[3].position.x - one[0].position.x;
	axis.y = one[3].position.y - one[0].position.y;
	Collision::normalize(axis);

	project(axis, one, minA, maxA);
	project(axis, two, minB, maxB);

	if (distance(minA, maxA, minB, maxB)>0.f)
		return false;


	// 3
	axis.x = two[1].position.x - two[0].position.x;
	axis.y = two[1].position.y - two[0].position.y;
	Collision::normalize(axis);

	project(axis, one, minA, maxA);
	project(axis, two, minB, maxB);

	if (distance(minA, maxA, minB, maxB)>0.f)
		return false;


	// 4
	axis.x = two[3].position.x - two[0].position.x;
	axis.y = two[3].position.y - two[0].position.y;
	Collision::normalize(axis);

	project(axis, one, minA, maxA);
	project(axis, two, minB, maxB);

	if (distance(minA, maxA, minB, maxB)>0.f)
		return false;

	return true;

}


