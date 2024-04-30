#include <random>

#include "Game.h"
#include "Utils.h"
#include "PowerUp.h"

PowerUp::PowerUp()
	: BaseActor(ActorType::POWERUP)
	, m_body(10.f)
	, m_isVisible(true)
{
	float randomWidth = Utils::randomValueRange(g_globals.windowWidth / 2, g_globals.windowWidth - 20.f, g_globals.clock.getElapsedTime().asMilliseconds());
	float randomHeight = Utils::randomValueRange(200.f, g_globals.windowHeight - 200.f, g_globals.clock.getElapsedTime().asMilliseconds());
	m_body.setFillColor(sf::Color::Green);
	m_body.setOutlineColor(sf::Color::Black);
	m_body.setOutlineThickness(2.f);
	setPosition(sf::Vector2f(randomWidth, randomHeight));
	m_body.setPosition(getPosition());
}

void PowerUp::update(float deltaTime, ActorPtrVec& actors)
{
	if (getIsVisible())
	{
		float oscillatingFloatY = Utils::oscillateInRange(getPosition().y + 5.f, getPosition().y - 5.f, 5.f, g_globals.clock.getElapsedTime().asSeconds());
		float oscillatingFloatX = Utils::oscillateInRange(getPosition().x + 5.f, getPosition().x - 5.f, 7.5f, g_globals.clock.getElapsedTime().asSeconds());
		float oscillatingFloatScaleX = Utils::oscillateInRange(-1.f, 1.f, 10.f, g_globals.clock.getElapsedTime().asSeconds());
		m_body.setPosition(oscillatingFloatX, oscillatingFloatY);
		m_body.setScale(oscillatingFloatScaleX, m_body.getScale().y);
	}
}

void PowerUp::render(sf::RenderWindow& window)
{
	if (m_isVisible)
		window.draw(m_body);
}
