#pragma once

#include <SFML/Graphics.hpp>
//#include "level.h"


class Entity
{
public:
	float width, height;
	//sf::Image image;
	sf::Texture texture;
	sf::Sprite sprite;
	sf::String name;
	Entity(sf::Image &image, float X, float Y, sf::String Name);
	~Entity();
	//sf::FloatRect getRect();
};



/*
#define SCALE 0.5

void Moving();
void Driving();
template <typename T> int sgn(T val);


float Accel = 0.0F; //������� ��������� ������ ����
float Break = 0.0F; //������� ��������� ������ �������
float Steer = 0.0F; //������� ��������� ����
long long KPPstep = 0; //������� ��������
const int KPPMaxstep = 4; //����� �������� ������� ������
//VB TO C++ CONVERTER WARNING: VB to C++ Converter has converted this array to a pointer. You will need to call 'delete[]' where appropriate:
//ORIGINAL LINE: Dim KPP(-1 To KPPMaxstep) As sinfgle //������������ ���
//float *KPP = new float[(-1 To KPPMaxstep) + 1]{}; //������������ ���

float Speed = 0.0F; //��������
float CarX; //���������� X ������ ������� ������
float CarY; //���������� Y ������ ������� ������
float Alpha = 0.0F; //���� �������� ������� ������ ������������ ��� Z
float SpeedAlpha = 0.0F; //�������� ��������
float SpeedX = 0.0F; //X ���������� ������� ��������
float SpeedY = 0.0F; //Y ���������� ������� ��������
float DTime = 0.0F; //�����, ��������� � ����������� ����� � ���.

//��������� ���� ������� ����� ������ � 3D
float carDiffFB = 0.0F; //������ ������ ������-�����
float carDiffLR = 0.0F; //������ ������ ������-�����
const int kYung = 15; //��������� �������������
const double carWidth = 1.85; //������ ������ � �.
const double carHeight = 2.3; //����� ������ ����� ����� � �.
const double resLR = 3.5; //���������� ����������� ������� ������� � ��.
const int resFB = 2; //���������� ����������� ������� ������� � ��.

const double kAF = 0.5; //����������� ������ � ������ � � ��������
const int MaxFRCY = 20; //������������ ���������� ���� ������, ���� - �����������
const int MaxFRCX = 20; //������������ ���������� ���� ������, ���� - �����
const int MaxRotFRC = 10; //������������ ���� ������ ��������
//��������� �������� MaxFRCY, MaxFRCX � MaxRotFRC:
//��� �������� - 20, 20, 10
//��� ���� - 5, 5, 3
//��� ������ - 10,15, 5

const double Power = 0.12; //�������� ������ � ���, �������� �� ����� � ��(1 �.�. = 736 ����)
//��� �������� 165 �.�. � ����� 1 �. Power = 165 * 0.736 / 1000
const int CarSpeed = 90; //������������ �������� ��� ����� ������ � �/� (1 ��/� = 0.278 �/�)
const int Radius = 4; //������ �������� � �.
const double CtrlSens = 1.2; //���������������� ����������
const double carExBack = 1.25; //���������� �� ������ ��� �� ������ ������� � �.



int main()
{
sf::RenderWindow window(sf::VideoMode(1000, 1000), "SFML works!");
sf::Clock clock;
float width, height;
sf::Vector2f position;
sf::String File;
sf::Image image;
sf::Texture texture;
sf::Sprite sprite;

File = "car_tex.png";
image.loadFromFile("images/" + File);
image.createMaskFromColor(sf::Color(255, 255, 255));
texture.loadFromImage(image);

sprite.setTexture(texture);
sprite.setScale(SCALE, SCALE);
width = texture.getSize().x * SCALE;
height = texture.getSize().y * SCALE;
position.x = 500;
position.y = 500;

sprite.setOrigin(width, height);

sprite.setPosition(position);
window.setFramerateLimit(60);

while (window.isOpen())
{
DTime = clock.getElapsedTime().asMilliseconds();
clock.restart();

sf::Event event;

while (window.pollEvent(event))
{
if (event.type == sf::Event::Closed)
window.close();
}
Driving();
Moving();
sprite.setPosition(CarX, CarY);
window.clear();
window.draw(sprite);
window.display();
}

return 0;
}




void Moving() //��������
{
float SpX = 0.0F; //�������� �� ��������� ��� X
float SpY = 0.0F; //�������� �� ��������� ��� Y
float FrcX = 0.0F; //���� �� ��������� ��� X
float FrcY = 0.0F; //���� �� ��������� ��� Y
float ForceX = 0.0F; //���� �� ��� X
float ForceY = 0.0F; //���� �� ��� Y
float RotFRC = 0.0F; //������ ���� ��������
float tmp = 0.0F;

//������� ������� � ��������� ������� ��������� ����������
if (SpeedY > 0.0000001)
{
tmp = atanf(SpeedX / SpeedY);
}
else if (SpeedY < -0.0000001)
{
tmp = 3.141593 + atanf(SpeedX / SpeedY);
}
else
{
tmp = 1.570796 * sgn(SpeedX);
}
tmp = tmp - Alpha;
Speed = powf((SpeedX * SpeedX + SpeedY * SpeedY),2);
SpX = -Speed * sinf(tmp);
SpY = Speed * cosf(tmp);
//������ ���
if (KPPstep == 0)
{
FrcY = 0.0F;
}
else
{
FrcY = (float)(CarSpeed * Accel - SpY) * Power ;
}

FrcY = FrcY - (Break / DTime + kAF) * sgn(SpY);
FrcX = (float)((-SpY * Steer * carExBack / Radius - SpX) / DTime);

if (abs(FrcX) > MaxFRCX) //�����
{
FrcX = MaxFRCX * sgn(FrcX);
}
if (abs(FrcY) > MaxFRCY) //�����������
{
FrcY = MaxFRCY * sgn(FrcY);
}

if (KPPstep == 0)
{
tmp = 0.0F;
}
else
{
tmp = (float)(2 / CarSpeed);
}
RotFRC = (float)((Steer * SpY / Radius - SpeedAlpha + SpX * Accel * tmp) / DTime);
tmp = (float)(MaxRotFRC * (CarSpeed - SpY) / CarSpeed);
if (abs(RotFRC) > tmp)
{
RotFRC = sgn(RotFRC) * tmp;
}

//���������� ��������
tmp = DTime * resFB;
carDiffFB = carDiffFB * (1 - tmp) + FrcY / (MaxFRCY * kYung * carHeight) * tmp;
tmp = DTime * resLR;
carDiffLR = carDiffLR * (1 - tmp) + FrcX / (MaxFRCX * kYung * carWidth) * tmp;

//������� ������� � ������� ������� ���������
ForceY = FrcY * cosf(Alpha) + FrcX * sinf(Alpha);
ForceX = FrcY * sinf(Alpha) - FrcX * cosf(Alpha);

SpeedAlpha = SpeedAlpha + RotFRC * DTime;
SpeedX = SpeedX + ForceX * DTime;
SpeedY = SpeedY + ForceY * DTime;
Alpha = Alpha + SpeedAlpha * DTime;
CarX = CarX + SpeedX * DTime;
CarY = CarY + SpeedY * DTime;
}

void Driving() //����������
{
if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) //���
{
Accel = Accel + DTime * CtrlSens;
if (Accel > 1)
{
Accel = 1;
}
}
else
{
Accel = Accel - DTime * CtrlSens; //�������� �������
if (Accel < 0.1)
{
Accel = 0.1;
}
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) //������
{
Break = Break + DTime * CtrlSens;
if (Break > 1)
{
Break = 1;
}
}
else
{
Break = Break - DTime * CtrlSens;
if (Break < 0)
{
Break = 0;
}
}

if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) //�����
{
Steer = Steer - DTime * CtrlSens;
if (Steer < -0.5)
{
Steer = -0.5;
}
}
else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) //������
{
Steer = Steer + DTime * CtrlSens;
if (Steer > 0.5)
{
Steer = 0.5;
}
}
else //���������
{
if (abs(Steer) < CtrlSens)
{
Steer = 0;
}
else
{
Steer = Steer - DTime * CtrlSens * sgn(Steer);
}
}
}

template <typename T> int sgn(T val) {
return (T(0) < val) - (val < T(0));
}
*/

/*
void Player::update(float dt) {
driving(dt);

sf::Vector2f localSpeed, localForce;
float tmp;
float rotationForce;

if (speedVec.y > 0.000001)
{
tmp = atanf(speedVec.x / speedVec.y);
}
else if (speedVec.y < -0.000001)
{
tmp = 3.141593 + atanf(speedVec.x / speedVec.y);
}
else
{
tmp = 1.570796 * sgn(speedVec.x);
}


tmp = tmp - rotationAngle;
//speed = sqrtf(speedVec.x * speedVec.x + speedVec.y * speedVec.y);
localSpeed.x = -speed * sinf(tmp);
localSpeed.y = speed * cosf(tmp);
//������ ���
localForce.y = (speedLimit * acceleration - localSpeed.y) * power;

localForce.y = localForce.y - (breaking / dt + 0.5)* sgn(localSpeed.y);
localForce.x = ((-localSpeed.y * steer * 1.5 / 4 - localSpeed.x) / dt);

if (abs(localForce.x) > 20.0f) //�����
{
localForce.x = 20.0f * sgn(localForce.x);
}
if (abs(localForce.y) > 20.0f) //�����������
{
localForce.y = 20.0f * sgn(localForce.y);
}

tmp = 2 / speedLimit;

rotationForce = (steer * speedVec.y / 4.0f - rotationSpeed + speedVec.x * acceleration * tmp) / dt;
tmp = 20.0f * (speedLimit - speedVec.y) / speedLimit;
if (abs(rotationForce) > tmp)
{
rotationForce = sgn(rotationForce) * tmp;
}

//������� ������� � ������� ������� ���������
force.y = localForce.y * cosf(rotationAngle) + localForce.x * sinf(rotationAngle);
force.x = localForce.y * sinf(rotationAngle) - localForce.x * cosf(rotationAngle);

rotationSpeed = rotationSpeed + rotationForce * dt;
speedVec.x += force.x * dt;
speedVec.y += force.y * dt;
rotationAngle += rotationSpeed * dt;
position += speedVec * dt;


//speedVec = speed * sf::Vector2f(sinf(sprite.getRotation() * 3.14159265f / 180.0f), cosf(sprite.getRotation() * 3.14159265f / 180.0f));

std::cout << "speedVec.x  \t" << speedVec.x <<" \t  speedVec.y \t" << speedVec.y << std::endl;



//speed += acceleration * dt;
sprite.setPosition(position.y, position.x);
sprite.setRotation(-rotationAngle);
}


*/