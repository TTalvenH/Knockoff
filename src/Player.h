#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <array>

#include "Game.h"
#include "PowerUp.h"
#include "Coin.h"

#define D_PLAYER_COLOR sf::Color(0x2B2D42FF)

class Enemy;

class Player : public BaseActor
{
private:
	sf::CircleShape m_body;
	float			m_maxSpeed;
	float			m_friction;
	bool			m_isLaunching;
	sf::Vector2f	m_velocity;
	sf::Vector2f	m_launchingVelocity;
	sf::Vector2i	m_startMousePosition;
	sf::Vector2i	m_endMousePosition;
	sf::VertexArray m_arrow;

	std::array<sf::SoundBuffer, 3>	m_soundBuffers;
	std::array<sf::Sound, 3>		m_sounds;

	Player();

	void handleMovement(float deltaTime);
	void enemyCollision(std::shared_ptr<Enemy>& enemyPtr);
	void powerUpCollision(std::shared_ptr<PowerUp>& powerUpPtr);
	void coinCollision(std::shared_ptr<Coin>& enemyPtr);
	void handleCollision(float deltaTime, ActorPtrVec& actors);
	bool isOutOfBounds() const;
public:
	Player(sf::Vector2f pos);

	void			playHitSound() { m_sounds[0].play(); }
	void			playPowerUpSound() { m_sounds[1].play(); }
	void			playCoinSound() { m_sounds[2].play(); }
	sf::CircleShape	getBody() { return m_body; }
	sf::Vector2f	getVelocity() { return m_velocity; }
	void			setVelocity(sf::Vector2f velocity) { m_velocity = velocity; }

	void update(float deltaTime, ActorPtrVec& actors) override;
	void render(sf::RenderWindow& window) override;
};

#include "Enemy.h"