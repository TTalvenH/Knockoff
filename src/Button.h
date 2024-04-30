#pragma once

#include <SFML/Graphics.hpp>

#define D_TEXT_COLOR sf::Color(0xEDF2F4FF) 
#define D_BUTTON_COLOR sf::Color(0xd90429FF) 
#define D_BUTTON_PRESSED_COLOR sf::Color(0x22737b) 
#define D_BUTTON_OUTLINE_COLOR sf::Color(0x2b2d42FF) 

class Button
{
private:
	sf::Text			m_text;
	sf::RectangleShape	m_rect;
	sf::Color			m_color;

public:
	Button();
	void			setText(std::string text);
	void			setPosition(sf::Vector2f position);
	sf::Text& getText() { return m_text; }
	sf::FloatRect	getGlobalBounds() { return sf::FloatRect(m_rect.getGlobalBounds()); }

	bool			isPressed(sf::Vector2f mousePosition);
	bool			isHovered(sf::Vector2f mousePosition);
	void			renderButton(sf::RenderWindow& window);
};