#include "Citizen.h"
#include "level.h"

Citizen::Citizen(sf::Vector2f pos, int type, Level &map, sf::Texture *walking, sf::Texture *fall, sf::Texture *exploding)
{
	//передачса указателей на спрайт листы
	textureWalking = walking;
	textureFalling = fall;
	textureBoom = exploding;
	
	mapObjects = map.GetObjects("solid"); //здания
	std::vector<Object> temp = map.GetObjects("building");
	mapObjects.insert(mapObjects.end(), temp.begin(), temp.end());
	
	//инициализация анимаций
	walk.setSpriteSheet(*textureWalking);
	falling.setSpriteSheet(*textureFalling);
	boom.setSpriteSheet(*textureBoom);
	// разные текстуры для разного
	// типа горожан
	if (type == 0)
	{
		walk.addFrame(sf::IntRect(0, 0, 61, 81));
		walk.addFrame(sf::IntRect(62, 13, 50, 45));
		walk.addFrame(sf::IntRect(112, 0, 63, 80));
		walk.addFrame(sf::IntRect(175, 13, 51, 45));

		falling.addFrame(sf::IntRect(0, 7, 62, 64));
		falling.addFrame(sf::IntRect(63, 0, 68, 63));
		animSprite.setScale(0.6, 0.6);
	}
	if (type == 1)
	{
		walk.addFrame(sf::IntRect(0, 0, 45, 60));
		walk.addFrame(sf::IntRect(45, 16, 37, 34));
		walk.addFrame(sf::IntRect(82, 0, 46, 60));
		walk.addFrame(sf::IntRect(128, 16, 37, 34));

		falling.addFrame(sf::IntRect(0, 0, 43, 88));
		falling.addFrame(sf::IntRect(43, 9, 79, 70));
		animSprite.setScale(0.7, 0.7);
	}
	if (type == 2)
	{
		walk.addFrame(sf::IntRect(0, 0, 47, 60));
		walk.addFrame(sf::IntRect(47, 15, 38, 34));
		walk.addFrame(sf::IntRect(85, 0, 45, 60));
		walk.addFrame(sf::IntRect(130, 17, 38, 34));

		falling.addFrame(sf::IntRect(0, 0, 54, 68));
		falling.addFrame(sf::IntRect(54, 0, 76, 75));
		animSprite.setScale(0.7, 0.7);
	}
	if (type == 3)  //коп
	{
		walk.addFrame(sf::IntRect(0, 0, 64, 82));
		walk.addFrame(sf::IntRect(64, 24, 50, 45));
		walk.addFrame(sf::IntRect(114, 0, 64, 82));
		walk.addFrame(sf::IntRect(178, 24, 49, 45));

		falling.addFrame(sf::IntRect(0, 7, 62, 64));
		falling.addFrame(sf::IntRect(63, 0, 68, 63));
		animSprite.setScale(0.6, 0.6);
	}
	//взрыв для всех общий
	boom.addFrame(sf::IntRect(134, 3, 23, 21));
	boom.addFrame(sf::IntRect(224, 27, 37, 33));
	boom.addFrame(sf::IntRect(312, 21, 54, 48));
	boom.addFrame(sf::IntRect(18, 111, 69, 61));
	boom.addFrame(sf::IntRect(108, 106, 82, 74));
	boom.addFrame(sf::IntRect(201, 102, 90, 80));
	boom.addFrame(sf::IntRect(302, 102, 67, 75));
	boom.addFrame(sf::IntRect(21, 193, 54, 74));
	boom.addFrame(sf::IntRect(113, 188, 54, 85));
	boom.addFrame(sf::IntRect(202, 103, 64, 57));
	boom.addFrame(sf::IntRect(300, 205, 66, 59));
	this->type = type;

	position = pos;
	
	//начальные значения
	speedVec = sf::Vector2f(0, 0); // вектор скорости (х, у)
	currentAnimation = &walk;// анимация
	rotationAngle = 0;// угол поворота
	isAlife = true; // живой

	// продолжительность анимаций
	dyingTime = sf::seconds(0.4);
	explodingTime = sf::milliseconds(11 * 70);

	//скорость (длина вектора)
	speed = 30;
	//случайное направление (вниз вверх влево вправо)
	direction = rand() % 4;
}

void Citizen::update(sf::Time dt) {
	//обновляем анимацию
	updateAnimation(dt);
	
	if (!isFalling) // если не сбит игроком
	{
		// гуляет по улице
		walkingDownTheStreet();
	}

	position += speedVec * dt.asSeconds();
}

Citizen::~Citizen()
{
}

void Citizen::draw(sf::RenderTarget &renderTarget) {
		renderTarget.draw(animSprite);
}

sf::FloatRect Citizen::getRect()
{
	return sf::FloatRect(animSprite.getGlobalBounds());
}

bool Citizen::checkIsAlife()
{
	if (isAlife) return true;
	return false;
}

int Citizen::getType()
{
	return type;
}


void Citizen::walkingDownTheStreet()
{
	if(collisionWithWall()) //если на пути стена
	{
		int dir = rand() % 2; //выбираем куда повернуть 
		if (dir == 0) direction++; // в одну сторону
		else direction--; // в другую
		if (direction > 3) direction = 0; 
		if (direction < 0) direction = 3;
	}


	if (direction == 0) //вверх
	{
		speedVec.x = 0;
		speedVec.y = -1;
		rotationAngle = 0;
	}
	if (direction == 1) //вправо
	{
		speedVec.x = 1;
		speedVec.y = 0;
		rotationAngle = 90;
	}
	if (direction == 2) //вниз
	{
		speedVec.x = 0;
		speedVec.y = 1;
		rotationAngle = 180;
	}
	if (direction == 3) //влнво
	{
		speedVec.x = -1;
		speedVec.y = 0;
		rotationAngle = 270;
	}
	speedVec = speed * speedVec;

}

void Citizen::updateAnimation(sf::Time dt)
{
	if (isExploding) // если взрывается
	{
		currentAnimation = &boom; // включаем анимацию взрыва
		animSprite.setFrameTime(sf::milliseconds(70)); //скорость анимации
		animSprite.setScale(1, 1);
		explodingTime -= dt; //обратнный отсчет
		if (explodingTime <= sf::Time::Zero)
		{
			isAlife = false; //убиваем
		}
	}
	else if (isFalling) // если сбит
	{
		currentAnimation = &falling; // включаем анимация падения
		animSprite.setLooped(false);
		dyingTime -= dt; // обратный отсчет
		if (dyingTime <= sf::Time::Zero)
		{
			isExploding = true; // после кторого переходим к взрыву
		}
	}
	else
	{
		currentAnimation = &walk; // иначе анимация ходьбы
	}

	animSprite.setOrigin(40, 40); // задать центр
	animSprite.setPosition(position);
	animSprite.setRotation(rotationAngle);
	
	animSprite.update(dt);  // обновление анимации
	animSprite.play(*currentAnimation);
}
 //  столкновени со стеной
bool Citizen::collisionWithWall()
{
	for (int i = 0; i < mapObjects.size(); i++) //пробегаем все здания на карте
	{
		if (mapObjects[i].rect.intersects(animSprite.getGlobalBounds())) // если сть с каким то из них есть пересечение
		{
			return true; // возвращаем true
		}
	}
	return false; // иначе false
}

// столкновение с игроком
bool Citizen::collisionWithPlayer(sf::FloatRect playerRect, sf::Vector2f playerSpeedVec, float playerRotation) 
{
	if (animSprite.getGlobalBounds().intersects(playerRect) && !isFalling) // если игрок коснулся жителя и еще не сбит
	{
		rotationAngle = playerRotation; 
		speedVec.x = 50 * playerSpeedVec.x + 10;
		speedVec.y = 50 * playerSpeedVec.y + 10;
		isFalling = true; // сбиваем
		return true;
	}
	return false;
}

