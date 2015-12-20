#ifndef Collision_
#define Collision_

#include <SFML/Graphics.hpp>

class SAT {
public:
	bool checkCollision(sf::Sprite &s1, sf::FloatRect &s2);
private:
    void  SAT::project(sf::Vector2f& axis, sf::VertexArray &_rectangle, float &min, float &max);
    void normalize(sf::Vector2f& vector);
    float dot(sf::Vector2f& vector1, sf::Vector2f& vector2);
    float SAT::distance(float minA, float maxA, float minB, float maxB) ;
};


#endif