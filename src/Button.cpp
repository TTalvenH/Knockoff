#include "Button.h"
#include "Utils.h"

Button::Button()
{
	float	buttonWidth = 120.f;
	float	buttonHeight = 40.f;

	m_color = D_BUTTON_COLOR;

	m_text.setString("default_text");
	m_text.setFillColor(D_TEXT_COLOR);
	m_text.setCharacterSize(21);

	m_rect.setSize(sf::Vector2f(buttonWidth, buttonHeight));
	m_rect.setFillColor(m_color);
	m_rect.setOutlineColor(D_BUTTON_OUTLINE_COLOR);
	m_rect.setOutlineThickness(5.f);

	Utils::centerOrigin(m_rect);
}

void Button::setText(std::string text)
{
	m_text.setString(text);
	Utils::centerOrigin(m_text);
}

void Button::setPosition(sf::Vector2f position)
{
	m_rect.setPosition(position);
	m_text.setPosition(position);
}

void Button::renderButton(sf::RenderWindow& window)
{
	m_rect.setFillColor(sf::Color(m_color));
	window.draw(m_rect);
	window.draw(m_text);
}

bool Button::isPressed(sf::Vector2f mousePosition)
{
	if (isHovered(mousePosition))
	{
		m_color = D_BUTTON_PRESSED_COLOR;
		if (sf::Mouse::isButtonPressed(sf::Mouse::Left))
			return true;
		return false;
	}
	else
	{
		m_color = D_BUTTON_COLOR;
		return false;
	}
}

bool Button::isHovered(sf::Vector2f mousePosition)
{
	if (getGlobalBounds().contains(mousePosition))
		return true;
	return false;
}

