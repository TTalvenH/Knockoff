#include "resources/Resources.h"

#include "UI.h"
#include "Game.h"
#include "Utils.h"


UI::UI()
{
	std::string assetPath = Resources::getAssetPath();
	if (!m_font.loadFromFile(assetPath + "fonts\\Montserrat-Regular.ttf"))
	{
		throw std::runtime_error("Failed to initialize the game!");
	}
	for (auto& button : m_buttons)
	{
		button.getText().setFont(m_font);
	}

	m_buttons[ButtonType::START].setText("Start");
	m_buttons[ButtonType::CONTINUE].setText("Continue");
	m_buttons[ButtonType::RESTART].setText("Restart");
	m_buttons[ButtonType::TITLE].setText("Title screen");
	m_buttons[ButtonType::QUIT].setText("Quit");

	m_buttons[ButtonType::START].setPosition(sf::Vector2f(g_globals.windowWidth / 2, g_globals.windowHeight / 2));
	m_buttons[ButtonType::CONTINUE].setPosition(sf::Vector2f(g_globals.windowWidth / 2, g_globals.windowHeight / 2));
	m_buttons[ButtonType::RESTART].setPosition(sf::Vector2f(g_globals.windowWidth / 2, g_globals.windowHeight / 2));
	m_buttons[ButtonType::TITLE].setPosition(sf::Vector2f(g_globals.windowWidth / 2, g_globals.windowHeight / 1.4));
	m_buttons[ButtonType::QUIT].setPosition(sf::Vector2f(g_globals.windowWidth / 2, g_globals.windowHeight / 1.2));

	std::string scoreStr = "CURRENT SCORE:  ";
	scoreStr += std::to_string(g_globals.score);
	scoreStr += "HIGH SCORE : ";
	scoreStr += std::to_string(g_globals.highScore);
	m_scoreText.setFont(m_font);
	m_scoreText.setFillColor(sf::Color::White);
	m_scoreText.setString(scoreStr);
	Utils::centerOrigin(m_scoreText);
	m_scoreText.setPosition(sf::Vector2f(g_globals.windowWidth / 2, g_globals.windowHeight / 30));
}

void UI::update(sf::Vector2f mousePosition)
{
	//TODO Clean this.
	std::string scoreStr = "CURRENT SCORE:  ";
	scoreStr += std::to_string(g_globals.score);
	scoreStr += "           HIGH SCORE : ";
	scoreStr += std::to_string(g_globals.highScore);
	m_scoreText.setString(scoreStr);
	Utils::centerOrigin(m_scoreText);

	switch (g_globals.gameState)
	{
	case GameState::MAIN_MENU:
		if (m_buttons[ButtonType::START].isPressed(mousePosition))
			g_globals.gameState = GameState::PLAYING;
		else if (m_buttons[ButtonType::QUIT].isPressed(mousePosition))
			g_globals.gameState = GameState::QUITTING;
		break;
	case GameState::PAUSE:
		if (m_buttons[ButtonType::CONTINUE].isPressed(mousePosition))
			g_globals.gameState = GameState::PLAYING;
		else if (m_buttons[ButtonType::TITLE].isPressed(mousePosition))
			g_globals.gameState = GameState::MAIN_MENU;
		else if (m_buttons[ButtonType::QUIT].isPressed(mousePosition))
			g_globals.gameState = GameState::QUITTING;
		break;
	case GameState::LOSE:
		if (m_buttons[ButtonType::RESTART].isPressed(mousePosition))
			g_globals.gameState = GameState::PLAYING;
		else if (m_buttons[ButtonType::TITLE].isPressed(mousePosition))
			g_globals.gameState = GameState::MAIN_MENU;
		else if (m_buttons[ButtonType::QUIT].isPressed(mousePosition))
			g_globals.gameState = GameState::QUITTING;
		break;
	case GameState::WIN:
		if (m_buttons[ButtonType::CONTINUE].isPressed(mousePosition))
			g_globals.gameState = GameState::PLAYING;
		else if (m_buttons[ButtonType::TITLE].isPressed(mousePosition))
			g_globals.gameState = GameState::MAIN_MENU;
		else if (m_buttons[ButtonType::QUIT].isPressed(mousePosition))
			g_globals.gameState = GameState::QUITTING;
		break;
	}
}

void UI::renderMainMenu(sf::RenderWindow& window)
{
	m_buttons[ButtonType::START].renderButton(window);
	m_buttons[ButtonType::QUIT].renderButton(window);
}

void UI::renderPauseMenu(sf::RenderWindow& window)
{
	m_buttons[ButtonType::CONTINUE].renderButton(window);
	m_buttons[ButtonType::TITLE].renderButton(window);
	m_buttons[ButtonType::QUIT].renderButton(window);
}

void UI::renderWinMenu(sf::RenderWindow& window)
{
	m_buttons[ButtonType::CONTINUE].renderButton(window);
	m_buttons[ButtonType::TITLE].renderButton(window);
	m_buttons[ButtonType::QUIT].renderButton(window);
}

void UI::renderLoseMenu(sf::RenderWindow& window)
{
	m_buttons[ButtonType::RESTART].renderButton(window);
	m_buttons[ButtonType::TITLE].renderButton(window);
	m_buttons[ButtonType::QUIT].renderButton(window);
}

void UI::render(sf::RenderWindow& window)
{
	window.draw(m_scoreText);
	switch (g_globals.gameState)
	{
	case GameState::MAIN_MENU:
		renderMainMenu(window);
		break;
	case GameState::PAUSE:
		renderPauseMenu(window);
		break;
	case GameState::WIN:
		renderWinMenu(window);
		break;
	case GameState::LOSE:
		renderLoseMenu(window);
		break;
	}
}
