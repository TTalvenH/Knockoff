#pragma once

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <vector>
#include <memory>
#include <random>

#include "UI.h"

#define D_BACKGROUND_COLOR sf::Color(0x8d99aeFF)

class BaseActor;

typedef std::vector<std::shared_ptr<BaseActor>> ActorPtrVec;

enum GameState
{
	MAIN_MENU = 0,
	PAUSE,
	PLAYING,
	LOSE,
	WIN,
	QUITTING
};

struct GameGlobal
{
	unsigned int gameState = GameState::MAIN_MENU;
	unsigned int windowWidth = 1440;
	unsigned int windowHeight = 900;
	unsigned int score = 0;
	unsigned int highScore = 0;
	unsigned int enemyCount = 0;
	unsigned int enemyLevel = 0;
	sf::Clock	 clock;
};

extern GameGlobal g_globals;

class Game
{
private:
	sf::RenderWindow	m_window;
	sf::View			m_view;
	sf::Music			m_tunes;
	UI					m_ui;
	ActorPtrVec			m_actors;
	float				m_deltaTime;
	float				m_collisionCooldown;

	
	void emptyGame();
	void addScore();
	void resetGame();
	void addPlayer();
	void addEnemies();
	void addPowerUp();
	void addCoin();
	void initActors();
	void handleKeyBoardInput(const sf::Event::KeyEvent& keyEvent);
	void handleEvents();
	void updateActors();
	void update();
	void render();
public:
	Game();

	void start();
};

#include "BaseActor.h"