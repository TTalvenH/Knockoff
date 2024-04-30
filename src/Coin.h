#pragma once

#include <SFML/Graphics/Drawable.hpp>
#include <SFML/Graphics/Transformable.hpp>

#include "BaseActor.h"

class Rectangle;

class Coin : public BaseActor
{
private:
	sf::CircleShape m_body;
	bool			m_isVisible;
public:
	Coin();

	void			setIsVisible(bool is) { m_isVisible = is; }
	sf::CircleShape	getBody() { return m_body; }
	bool			getIsVisible() { return m_isVisible; }


	void			update(float deltaTime, ActorPtrVec& actors) override;
	void			render(sf::RenderWindow& window) override;
};
