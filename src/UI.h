#pragma once

#include <SFML/Graphics.hpp>

#include "Button.h"

enum ButtonType
{
	START = 0,
	CONTINUE,
	RESTART,
	TITLE,
	QUIT,
	TYPE_COUNT
};

class UI
{
private:
	Button		m_buttons[ButtonType::TYPE_COUNT];
	sf::Text	m_scoreText;
	sf::Font	m_font;

public:
	UI();
	~UI() {};


	void update(sf::Vector2f mousePosition);

	void renderMainMenu(sf::RenderWindow& window);
	void renderPauseMenu(sf::RenderWindow& window);
	void renderWinMenu(sf::RenderWindow& window);
	void renderLoseMenu(sf::RenderWindow& window);
	void render(sf::RenderWindow& window);
};