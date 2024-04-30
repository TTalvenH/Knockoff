#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <random>

#include "Game.h"
#include "PowerUp.h"

#define D_ENEMY_COLOR sf::Color(0xEF233CFF)

class Player;

class Enemy : public BaseActor
{
private:
	sf::CircleShape m_body;
	float			m_maxSpeed;
	sf::Time		m_launchTimeStamp;
	float			m_launchDelay;
	float			m_randomLaunchMod;
	float			m_friction;
	sf::Vector2f	m_velocity;
	BaseActor&		m_playerRef;
	bool			m_isSimulated;

	Enemy();

public:
	Enemy(sf::Vector2f pos, BaseActor& playerRef);

	sf::CircleShape	getBody() { return m_body; }
	sf::Vector2f	getVelocity() { return m_velocity; }
	void			setVelocity(sf::Vector2f velocity) { m_velocity = velocity; }

	void handleMovement(float deltaTime);
	void playerCollision(std::shared_ptr<Player>& playerPtr);
	void handleCollision(float deltaTime, ActorPtrVec& actors);
	bool isOutOfBounds() const;

	void update(float deltaTime, ActorPtrVec& actors) override;
	void render(sf::RenderWindow& window) override;
};

#include "Player.h"