#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation();

	void addFrame(sf::IntRect rect); // добавить кадр анимации
	void setSpriteSheet(const sf::Texture& texture); // задать спрайт-лист
	const sf::Texture* getSpriteSheet() const; // вернуть спрайт лист
	int getSize() const; // размер аниммации (кол - во кадров)
	const sf::IntRect& getFrame(int n) const; // получить конкретный кадр

private:
	std::vector<sf::IntRect> m_frames; // массив кадров
	const sf::Texture* m_texture; // текстура
};

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
	AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true); //конструктор с аргументами по умолчанию

	void update(sf::Time deltaTime);
	void setAnimation(const Animation& animation); //задать анимацию
	void setFrameTime(sf::Time time); // задать время одного кадра (определяет скорость анимации)
	void play(); // проиграть текущую анимацию
	void play(const Animation& animation); //проиграть заданную анимацию
	void pause(); //приостановить анимацию
	void stop(); //остановить анимацию
	void setLooped(bool looped); // задатаь повторение анимации

	const Animation* getAnimation() const; // получить текущую анимацию
	sf::FloatRect getLocalBounds() const; // получить локальные координаты текущего кадра анимации
	sf::FloatRect getGlobalBounds() const; // получить экранные координаты
	bool isLooped() const;
	bool isPlaying() const;
	sf::Time getFrameTime() const;
	void setFrame(int newFrame, bool resetTime = true); // задать номер кадра анимации

private:
	const Animation* m_animation;
	sf::Time m_frameTime;
	sf::Time m_currentTime;
	int m_currentFrame;
	bool m_isPaused;
	bool m_isLooped;
	const sf::Texture* m_texture;
	sf::Vertex m_vertices[4]; // массив из 4 вершин, координаты углов кажра анимации

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //отрисовка

};