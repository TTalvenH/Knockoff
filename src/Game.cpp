#include <iostream>

#include "Game.h"
#include "Player.h"
#include "Enemy.h"
#include "PowerUp.h"
#include "Coin.h"
#include "Utils.h"
#include "resources/Resources.h"

GameGlobal g_globals;

Game::Game()
	: m_window(sf::VideoMode(g_globals.windowWidth, g_globals.windowHeight), "KnockOff!!", sf::Style::Close, sf::ContextSettings(0, 0, 8))
	, m_actors()
	, m_deltaTime(0)
	, m_ui()
	, m_collisionCooldown(g_globals.clock.getElapsedTime().asSeconds())
{
	std::string assetPath = Resources::getAssetPath();
	if (!m_tunes.openFromFile(assetPath + "audio/320963__littlerobotsoundfactory__loop_computer_feeling_good_00.wav"))
	{
		throw std::runtime_error("Failed to initialize the game!");
	}
	m_tunes.setLoop(true);
	m_tunes.setVolume(15);
	m_tunes.play();
	//m_window.setFramerateLimit(144);
	m_view = m_window.getView();
}

void Game::emptyGame()
{
	if (m_actors.empty() == true)
		return;
	m_actors.clear();
}

void Game::addScore()
{
	if (m_actors.empty() == false)
	{
		g_globals.enemyLevel++;
		g_globals.score++;
		if (g_globals.highScore < g_globals.score)
			g_globals.highScore++;
	}
}

void Game::resetGame()
{
	g_globals.score = 0;
	g_globals.enemyLevel = 0;
	g_globals.enemyCount = 0;
}

void Game::addPlayer()
{
	sf::Vector2f	spawnPosition = sf::Vector2f(g_globals.windowWidth / 4, g_globals.windowHeight / 2);

	m_actors.push_back(std::make_shared<Player>(spawnPosition));
}

void Game::addEnemies()
{
	for (int i = 0; i <= g_globals.enemyLevel; i++)
	{
		if (i % 4 == 0 || g_globals.enemyCount == 0)
		{
			g_globals.enemyCount++;
		}
	}
	for (unsigned int i = 0; i < g_globals.enemyCount; i++)
	{
		float			spaceSpawnPoints = (i - (g_globals.enemyCount - 1) / 2.f) * 300.f;
		float			centerHeight = spaceSpawnPoints + g_globals.windowHeight / 2;
		sf::Vector2f	spawnPosition = sf::Vector2f(g_globals.windowWidth - g_globals.windowWidth / 4, centerHeight);

		m_actors.push_back(std::make_shared<Enemy>(spawnPosition, *m_actors.at(0)));
	}
}

void Game::addPowerUp()
{
	m_actors.push_back(std::make_shared<PowerUp>());
}

void Game::addCoin()
{
	m_actors.push_back(std::make_shared<Coin>());
}

void Game::initActors()
{
	if (m_actors.empty() == false)
		return;
	addPlayer();
	addEnemies();
	addPowerUp();
	addCoin();
}

void Game::handleKeyBoardInput(const sf::Event::KeyEvent& keyEvent)
{
	if (keyEvent.code == sf::Keyboard::Escape)
	{
		switch (g_globals.gameState)
		{
		case GameState::PLAYING:
			g_globals.gameState = GameState::PAUSE;
			break;
		case GameState::PAUSE:
			g_globals.gameState = GameState::PLAYING;
		}
	}
	if (keyEvent.code == sf::Keyboard::PageDown)
	{
		g_globals.enemyCount = 0;
		g_globals.gameState = GameState::WIN;
	}
	if (keyEvent.code == sf::Keyboard::M)
	{
		switch (m_tunes.getStatus())
		{
		case sf::Music::Playing:
			m_tunes.pause();
			break;
		case sf::Music::Paused:
			m_tunes.play();
			break;
		}
	}
}

void Game::handleEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
		{
			m_window.close();
			break;
		}
		case sf::Event::KeyPressed:
		{
			handleKeyBoardInput(event.key);
			break;
		}
		}
	}
}

void Game::updateActors()
{
	for (auto& actor : m_actors)
	{
		actor->update(m_deltaTime, m_actors);
	}
}

void Game::update()
{
	switch (g_globals.gameState)
	{
	case GameState::PLAYING:
	{
		initActors();
		updateActors();
		break;
	}
	case GameState::MAIN_MENU:
	{
		resetGame();
		emptyGame();
		break;
	}
	case GameState::WIN:
	{
		addScore();
		emptyGame();
		break;
	}
	case GameState::LOSE:
	{
		resetGame();
		emptyGame();
		break;
	}
	}
	m_ui.update(static_cast<sf::Vector2f>(sf::Mouse::getPosition(m_window)));
	m_window.setView(m_view);
}

void Game::render()
{
	m_window.clear(D_BACKGROUND_COLOR);
	for (auto& actor : m_actors)
	{
		actor->render(m_window);
	}
	m_ui.render(m_window);
	m_window.display();
}

void Game::start()
{
	sf::Clock	clock;
	const float	deltaScalar = 50.f;
	while (m_window.isOpen())
	{
		m_deltaTime = clock.restart().asSeconds() * deltaScalar;
		handleEvents();
		update();
		render();
		if (g_globals.gameState == GameState::QUITTING)
			m_window.close();
	}
}