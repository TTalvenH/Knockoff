#pragma once

#include <math.h>
#include <random>
#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

class Utils
{
public:
	static float vecLength(sf::Vector2f a)
	{
		return sqrtf(a.x * a.x + a.y * a.y);
	}

	static sf::Vector2f vectorNormalize(const sf::Vector2f vec)
	{
		return sf::Vector2f(vec.x / vecLength(vec), vec.y / vecLength(vec));
	}

	static float clamp(float value, float min, float max)
	{
		value = value < min ? min : value;
		value = value > max ? max : value;
		return value;
	}

	static sf::Vector2f lerp(const sf::Vector2f& start, const sf::Vector2f& end, float t)
	{
		t = clamp(t, 0.f, 1.f);
		return start + t * (end - start);
	}

	static bool isCirclesColliding(const sf::CircleShape& a, const sf::CircleShape& b)
	{
		sf::Vector2f	deltaVec = a.getPosition() - b.getPosition();
		float			distanceBetween = Utils::vecLength(deltaVec);
		float			radiusSum = a.getRadius() + b.getRadius();

		if (distanceBetween <= radiusSum)
			return true;
		return false;
	}

	template <typename T>
	static void centerOrigin(T& shape)
	{
		sf::FloatRect	bounds = shape.getLocalBounds();
		float			centerX = bounds.left + bounds.width / 2.0f;
		float			centerY = bounds.top + bounds.height / 2.0f;
		shape.setOrigin(centerX, centerY);
	}

	static bool hasTimeElapsed(sf::Time beginTime, sf::Time endTime, float timeElapsedSeconds)
	{
		float beginInSeconds = beginTime.asSeconds();
		float endInSeconds = endTime.asSeconds();

		if (endInSeconds - beginInSeconds > timeElapsedSeconds)
			return true;
		return false;
	}

	static float oscillateInRange(float minRange, float maxRange, float speed, float clockTime)
	{
		float range = maxRange - minRange;
		float normalizedValue = 0.5f * std::sinf(clockTime * speed) + 0.5f;
		return minRange + range * normalizedValue;
	}

	//TODO fix so don't need different seed everytime?
	static float randomValueRange(float minRange, float maxRange, unsigned int seed)
	{
		std::mt19937 generator(seed);
		std::uniform_real_distribution randomValue(minRange, maxRange);
		return randomValue(generator);
	}
};
