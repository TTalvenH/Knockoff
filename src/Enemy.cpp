#include "Enemy.h"
#include "Utils.h"
#include <iostream>
#include <random>

Enemy::Enemy(sf::Vector2f pos, BaseActor& playerRef)
	: BaseActor(ActorType::ENEMY)
	, m_maxSpeed(40.f)
	, m_launchDelay(1.5f - (g_globals.enemyLevel * 0.05f))
	, m_randomLaunchMod(1.f)
	, m_launchTimeStamp(g_globals.clock.getElapsedTime())
	, m_friction(0.05f)
	, m_body(50.f)
	, m_velocity(0.f, 0.f)
	, m_playerRef(playerRef)
	, m_isSimulated(true)
{
	m_launchDelay = Utils::clamp(m_launchDelay, 0.3f, 10.f);
	Utils::centerOrigin(m_body);
	m_body.setOutlineColor(sf::Color::Black);
	m_body.setOutlineThickness(1.f);
	m_body.setFillColor(sf::Color(D_ENEMY_COLOR));
	m_randomLaunchMod = Utils::randomValueRange(0.5f, 3.f, g_globals.clock.getElapsedTime().asMicroseconds());
	setPosition(pos);
}

void Enemy::handleMovement(float deltaTime)
{
	if (Utils::hasTimeElapsed(m_launchTimeStamp, g_globals.clock.getElapsedTime(), m_launchDelay * m_randomLaunchMod))
	{
		sf::Vector2f directionToPlayer = m_playerRef.getPosition() - this->getPosition();

		m_velocity = directionToPlayer;
		m_launchTimeStamp = g_globals.clock.getElapsedTime();
		m_randomLaunchMod = Utils::randomValueRange(0.7f, 3.f, g_globals.clock.getElapsedTime().asMicroseconds());
	}

	if (Utils::vecLength(m_velocity) > m_maxSpeed)
	{
		m_velocity = Utils::vectorNormalize(m_velocity);
		m_velocity = m_velocity * m_maxSpeed;
	}

	if (m_velocity != sf::Vector2f(0.f, 0.f))
		m_velocity = Utils::lerp(m_velocity, sf::Vector2f(0, 0), m_friction * deltaTime);

	m_position += m_velocity * deltaTime;
	m_body.setPosition(m_position);

}

void Enemy::playerCollision(std::shared_ptr<Player>& playerPtr)
{
	if (Utils::isCirclesColliding(getBody(), playerPtr->getBody()) == false)
		return;

	playerPtr->playHitSound();

	sf::Vector2f	normalDirection = Utils::vectorNormalize(playerPtr->getPosition() - getPosition());
	setPosition(getPosition() - normalDirection);
	playerPtr->setPosition(playerPtr->getPosition() + normalDirection);
	if (Utils::vecLength(getVelocity()) > Utils::vecLength(playerPtr->getVelocity()) && playerPtr->getCollisionCooldown() == 0.f)
	{
		setCollisionCooldown(0.5f);
		sf::Vector2f	velocity = getVelocity() + playerPtr->getVelocity();
		float			bounceBack = Utils::vecLength(velocity);

		playerPtr->setVelocity(normalDirection * bounceBack);
		setVelocity(normalDirection * bounceBack * -0.2f);
	}
}

void Enemy::handleCollision(float deltaTime, ActorPtrVec& actors)
{
	setCollisionCooldown(getCollisionCooldown() - deltaTime);
	if (getCollisionCooldown() < 0.f)
		setCollisionCooldown(0.0f);
	for (auto& actor : actors)
	{
		switch (actor->getType())
		{
		case ActorType::PLAYER:
			auto playerPtr = std::static_pointer_cast<Player>(actor);
			playerCollision(playerPtr);
			break;
		}
	}
}

void Enemy::update(float deltaTime, ActorPtrVec& actors)
{
	if (m_isSimulated == true)
	{
		handleMovement(deltaTime);
		handleCollision(deltaTime, actors);
	}
	if (m_isSimulated == true && isOutOfBounds())
	{
		m_isSimulated = false;
		g_globals.enemyCount--;
	}
	if (g_globals.enemyCount == 0)
	{
		g_globals.gameState = GameState::WIN;
	}
}

bool Enemy::isOutOfBounds() const
{
	if (m_position.x - m_body.getRadius() > g_globals.windowWidth
		|| m_position.x + m_body.getRadius() < 0)
		return true;
	if (m_position.y - m_body.getRadius() > g_globals.windowHeight
		|| m_position.y + m_body.getRadius() < 0)
		return true;
	return false;
}

void Enemy::render(sf::RenderWindow& window)
{
	if (m_isSimulated == true)
		window.draw(m_body);
}
