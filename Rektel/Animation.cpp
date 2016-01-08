#include "Animation.h"

Animation::Animation() : m_texture(NULL)
{

}

void Animation::addFrame(sf::IntRect rect)
{
	m_frames.push_back(rect);
}

void Animation::setSpriteSheet(const sf::Texture& texture)
{
	m_texture = &texture;
}

const sf::Texture* Animation::getSpriteSheet() const
{
	return m_texture;
}

int Animation::getSize() const
{
	return m_frames.size();
}

const sf::IntRect& Animation::getFrame(int n) const
{
	return m_frames[n];
}
/////////////////////////////''''''''\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\\

AnimatedSprite::AnimatedSprite(sf::Time frameTime, bool paused, bool looped) :
	m_animation(NULL), m_frameTime(frameTime), m_currentFrame(0), m_isPaused(paused), m_isLooped(looped), m_texture(NULL)
{

}

void AnimatedSprite::setAnimation(const Animation& animation)
{
	m_animation = &animation;
	m_texture = m_animation->getSpriteSheet();
	m_currentFrame = 0;
	setFrame(m_currentFrame);
}

void AnimatedSprite::setFrameTime(sf::Time time)
{
	m_frameTime = time;
}

void AnimatedSprite::play()
{
	m_isPaused = false;
}

void AnimatedSprite::play(const Animation& animation)
{
	if (getAnimation() != &animation)
		setAnimation(animation);
	play();
}

void AnimatedSprite::pause()
{
	m_isPaused = true;
}

void AnimatedSprite::stop()
{
	m_isPaused = true;
	m_currentFrame = 0;
	setFrame(m_currentFrame);
}

void AnimatedSprite::setLooped(bool looped)
{
	m_isLooped = looped;
}

const Animation* AnimatedSprite::getAnimation() const
{
	return m_animation;
}

sf::FloatRect AnimatedSprite::getLocalBounds() const
{
	sf::IntRect rect = m_animation->getFrame(m_currentFrame);

	float width = (float)(std::abs(rect.width));
	float height = (float)(std::abs(rect.height));

	return sf::FloatRect(0.f, 0.f, width, height);
}

sf::FloatRect AnimatedSprite::getGlobalBounds() const
{
	return getTransform().transformRect(getLocalBounds());
}

bool AnimatedSprite::isLooped() const
{
	return m_isLooped;
}

bool AnimatedSprite::isPlaying() const
{
	return !m_isPaused;
}

sf::Time AnimatedSprite::getFrameTime() const
{
	return m_frameTime;
}

void AnimatedSprite::setFrame(int newFrame, bool resetTime)
{
	if (m_animation)
	{
		//�������� ������ ������
		sf::IntRect rect = m_animation->getFrame(newFrame);

		//������������ ���������� ������ �����
		m_vertices[0].position = sf::Vector2f(0.f, 0.f);
		m_vertices[1].position = sf::Vector2f(0.f, (float)(rect.height));
		m_vertices[2].position = sf::Vector2f((float)(rect.width), (float)(rect.height));
		m_vertices[3].position = sf::Vector2f((float)(rect.width), 0.f);

		//��������� ���������� (�������� ����)
		float left = (float)(rect.left) + 0.0001f;
		float right = left + (float)(rect.width);
		float top = (float)(rect.top);
		float bottom = top + (float)(rect.height);

		//���������� �������� �� �������
		m_vertices[0].texCoords = sf::Vector2f(left, top);
		m_vertices[1].texCoords = sf::Vector2f(left, bottom);
		m_vertices[2].texCoords = sf::Vector2f(right, bottom);
		m_vertices[3].texCoords = sf::Vector2f(right, top);
	}

	if (resetTime)
		m_currentTime = sf::Time::Zero;
}

void AnimatedSprite::update(sf::Time deltaTime)
{
	// ���� �� �� ����� � �������� ��������� (����������)
	if (!m_isPaused && m_animation)
	{
		// ��������� dt
		m_currentTime += deltaTime;

		// ���� ������� ����� �������� ������ ������� ������ �����
		if (m_currentTime >= m_frameTime)
		{
			// ���������� �����, �� ��������� �������
			m_currentTime = sf::microseconds(m_currentTime.asMicroseconds() % m_frameTime.asMicroseconds());

			// �������� ����� ���������� �����
			if (m_currentFrame + 1 < m_animation->getSize())
				m_currentFrame++;
			else
			{
				// �������� �����������
				m_currentFrame = 0; // ���������� ��������

				if (!m_isLooped)
				{
					m_isPaused = true;
				}

			}
			// ������ ������� ����
			setFrame(m_currentFrame, false);
		}
	}
}

void AnimatedSprite::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	if (m_animation && m_texture)
	{
		states.transform *= getTransform();
		states.texture = m_texture;
		target.draw(m_vertices, 4, sf::Quads, states);
	}
}