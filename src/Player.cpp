#include "Player.h"
#include "Utils.h"

#include <SFML/Audio.hpp>
#include <memory>
#include "resources/Resources.h"
#include <iostream>

Player::Player(sf::Vector2f pos)
	: BaseActor(ActorType::PLAYER)
	, m_maxSpeed(40.f)
	, m_body(50.f, 1200)
	, m_velocity(sf::Vector2f())
	, m_friction(0.05f)
	, m_isLaunching(false)
	, m_startMousePosition(sf::Vector2f(sf::Mouse::getPosition()))
	, m_endMousePosition(sf::Vector2f(sf::Mouse::getPosition()))
	, m_launchingVelocity(sf::Vector2f())
	, m_arrow(sf::LinesStrip)
{
	Utils::centerOrigin(m_body);
	m_body.setOutlineColor(sf::Color::Black);
	m_body.setOutlineThickness(1.f);
	m_body.setFillColor(sf::Color(D_PLAYER_COLOR));
	setPosition(pos);

	std::string assetPath = Resources::getAssetPath();
	if (!m_soundBuffers[0].loadFromFile(assetPath + "audio/hit.wav")
		|| !m_soundBuffers[1].loadFromFile(assetPath + "audio/PowerUp.wav")
		|| !m_soundBuffers[2].loadFromFile(assetPath + "audio/350873__cabled_mess__coin_c_02.wav"))
	{
		throw std::runtime_error("Failed to initialize player audio!");
	}
	m_sounds[0].setBuffer(m_soundBuffers[0]);
	m_sounds[0].setVolume(100.f);
	m_sounds[1].setBuffer(m_soundBuffers[1]);
	m_sounds[1].setVolume(9.f);
	m_sounds[1].setPitch(0.5);
	m_sounds[2].setBuffer(m_soundBuffers[2]);
	m_sounds[2].setVolume(100.f);
	m_sounds[2].setPitch(0.8);
	m_arrow.append(sf::Vertex(sf::Vector2f(), sf::Color::Transparent));
	m_arrow.append(sf::Vertex(sf::Vector2f(), sf::Color::Transparent));
}

void Player::handleMovement(float deltaTime)
{
	if (sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_isLaunching = true;
		m_endMousePosition = sf::Mouse::getPosition();
		m_launchingVelocity += static_cast<sf::Vector2f>(m_startMousePosition - m_endMousePosition);
		m_launchingVelocity *= 0.15f;
		if (Utils::vecLength(m_launchingVelocity) > m_maxSpeed)
		{
			m_launchingVelocity = Utils::vectorNormalize(m_launchingVelocity) * m_maxSpeed;
		}
		m_arrow[0] = sf::Vertex(sf::Vector2f(getPosition() + Utils::vectorNormalize(m_launchingVelocity) * getBody().getRadius()), sf::Color::Red);
		m_arrow[1] = sf::Vertex(sf::Vector2f(m_position + m_launchingVelocity * 5.f), sf::Color::Red);
	}
	else if (!m_isLaunching)
	{
		m_startMousePosition = sf::Mouse::getPosition();
		m_arrow[0].color = sf::Color::Transparent;
		m_arrow[1].color = sf::Color::Transparent;
	}
	if (m_isLaunching && !sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
	{
		m_velocity = m_launchingVelocity;
		m_isLaunching = false;
	}
	if (m_velocity != sf::Vector2f(0.f, 0.f))
		m_velocity = Utils::lerp(m_velocity, sf::Vector2f(0, 0), m_friction * deltaTime);

	m_position += m_velocity * deltaTime;
	m_body.setPosition(m_position);
}

void Player::enemyCollision(std::shared_ptr<Enemy>& enemyPtr)
{
	if (Utils::isCirclesColliding(getBody(), enemyPtr->getBody()) == false)
		return;

	sf::Vector2f	normalDirection = Utils::vectorNormalize(enemyPtr->getPosition() - getPosition());

	playHitSound();
	setPosition(getPosition() - normalDirection);
	enemyPtr->setPosition(enemyPtr->getPosition() + normalDirection);
	if (Utils::vecLength(getVelocity()) > Utils::vecLength(enemyPtr->getVelocity()) && enemyPtr->getCollisionCooldown() == 0.f)
	{
		setCollisionCooldown(0.5f);
		sf::Vector2f	velocity = getVelocity() + enemyPtr->getVelocity();
		float			scalar = Utils::vecLength(velocity);

		enemyPtr->setVelocity(normalDirection * scalar);
		setVelocity(normalDirection * scalar * -0.2f);
	}
}

void Player::powerUpCollision(std::shared_ptr<PowerUp>& powerUpPtr)
{
	if (Utils::isCirclesColliding(getBody(), powerUpPtr->getBody()) && powerUpPtr->getIsVisible())
	{
		playPowerUpSound();
		powerUpPtr->setIsVisible(false);
		getBody().setRadius(getBody().getRadius() + 100.f);
		m_maxSpeed = 150.f;
	}
}

void Player::coinCollision(std::shared_ptr<Coin>& coinPtr)
{
	if (Utils::isCirclesColliding(getBody(), coinPtr->getBody()) && coinPtr->getIsVisible())
	{
		coinPtr->setIsVisible(false);
		playCoinSound();
		g_globals.score++;
	}
}

void Player::handleCollision(float deltaTime, ActorPtrVec& actors)
{
	setCollisionCooldown(getCollisionCooldown() - deltaTime);
	if (getCollisionCooldown() < 0.f)
		setCollisionCooldown(0.0f);
	for (auto& actor : actors)
	{
		switch (actor->getType())
		{
		case ActorType::ENEMY:
		{
			auto enemyPtr = std::static_pointer_cast<Enemy>(actor);
			enemyCollision(enemyPtr);
			break;
		}
		case ActorType::POWERUP:
		{
			auto powerUpPtr = std::static_pointer_cast<PowerUp>(actor);
			powerUpCollision(powerUpPtr);
			break;
		}
		case ActorType::COIN:
		{
			auto coinPtr = std::static_pointer_cast<Coin>(actor);
			coinCollision(coinPtr);
			break;
		}
		}
	}
}

bool Player::isOutOfBounds() const
{
	if (m_position.x - m_body.getRadius() > g_globals.windowWidth
		|| m_position.x + m_body.getRadius() < 0)
		return true;
	if (m_position.y - m_body.getRadius() > g_globals.windowHeight
		|| m_position.y + m_body.getRadius() < 0)
		return true;
	return false;
}

void Player::update(float deltaTime, ActorPtrVec& actors)
{
	handleMovement(deltaTime);
	handleCollision(deltaTime, actors);
	if (isOutOfBounds())
	{
		g_globals.gameState = GameState::LOSE;
	}
}


void Player::render(sf::RenderWindow& window)
{
	window.draw(m_body);
	window.draw(m_arrow);
}
