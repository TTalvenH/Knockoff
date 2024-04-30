#pragma once

#include <SFML/Graphics.hpp>

#include "BaseActor.h"
#include "Game.h"

class PowerUp : public BaseActor
{
private:
	sf::CircleShape m_body;
	bool			m_isVisible;
public:
	PowerUp();

	void			setIsVisible(bool is) { m_isVisible = is; }
	sf::CircleShape	getBody() { return m_body; }
	bool			getIsVisible() { return m_isVisible; }


	void			update(float deltaTime, ActorPtrVec& actors) override;
	void			render(sf::RenderWindow& window) override;
};