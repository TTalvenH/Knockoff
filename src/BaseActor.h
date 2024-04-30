#pragma once

#include <SFML/Graphics.hpp>
#include "Game.h"

enum ActorType
{
	PLAYER = 0,
	ENEMY,
	POWERUP,
	COIN
};

class BaseActor
{
protected:
	unsigned int	m_type;
	sf::Vector2f	m_position;
	float			m_collisionCooldown;

public:
	BaseActor(unsigned int type) : m_type(type), m_collisionCooldown(0) {}
	virtual ~BaseActor() {}


	unsigned int	getType() { return m_type; }
	void			setPosition(sf::Vector2f pos) { m_position = pos; }
	sf::Vector2f	getPosition() const { return m_position; }
	void			setCollisionCooldown(float second) { m_collisionCooldown = second; }
	float			getCollisionCooldown() { return m_collisionCooldown; }

	virtual void update(float deltaTime, ActorPtrVec& actors) = 0;
	virtual void render(sf::RenderWindow& window) = 0;
};
