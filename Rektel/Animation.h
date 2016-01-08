#pragma once
#include <vector>
#include <SFML/Graphics.hpp>

class Animation
{
public:
	Animation();

	void addFrame(sf::IntRect rect); // �������� ���� ��������
	void setSpriteSheet(const sf::Texture& texture); // ������ ������-����
	const sf::Texture* getSpriteSheet() const; // ������� ������ ����
	int getSize() const; // ������ ��������� (��� - �� ������)
	const sf::IntRect& getFrame(int n) const; // �������� ���������� ����

private:
	std::vector<sf::IntRect> m_frames; // ������ ������
	const sf::Texture* m_texture; // ��������
};

class AnimatedSprite : public sf::Drawable, public sf::Transformable
{
public:
	AnimatedSprite(sf::Time frameTime = sf::seconds(0.2f), bool paused = false, bool looped = true); //����������� � ����������� �� ���������

	void update(sf::Time deltaTime);
	void setAnimation(const Animation& animation); //������ ��������
	void setFrameTime(sf::Time time); // ������ ����� ������ ����� (���������� �������� ��������)
	void play(); // ��������� ������� ��������
	void play(const Animation& animation); //��������� �������� ��������
	void pause(); //������������� ��������
	void stop(); //���������� ��������
	void setLooped(bool looped); // ������� ���������� ��������

	const Animation* getAnimation() const; // �������� ������� ��������
	sf::FloatRect getLocalBounds() const; // �������� ��������� ���������� �������� ����� ��������
	sf::FloatRect getGlobalBounds() const; // �������� �������� ����������
	bool isLooped() const;
	bool isPlaying() const;
	sf::Time getFrameTime() const;
	void setFrame(int newFrame, bool resetTime = true); // ������ ����� ����� ��������

private:
	const Animation* m_animation;
	sf::Time m_frameTime;
	sf::Time m_currentTime;
	int m_currentFrame;
	bool m_isPaused;
	bool m_isLooped;
	const sf::Texture* m_texture;
	sf::Vertex m_vertices[4]; // ������ �� 4 ������, ���������� ����� ����� ��������

	virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const; //���������

};