#pragma once

#include "SFML/Graphics.hpp"

class Debug
{
public:
	Debug();
	void Update(float dt);
	void DrawFPS(sf::RenderWindow& window) const;
	void DrawGridCoordinates(const sf::Vector2i& gridPos, const sf::Vector2i& scrnPos, sf::RenderWindow& window) const;
private:
	// create the font used to display stuff
	sf::Font font;
	// variables used to keep track of frames drawn per second
	const float fpsRefreshTime = 0.2f;
	float fpsTimeCounter = 0.0f;
	int fpsFramesCounter = 0;
	int framesPerSecond = 0;
	// vraiable to keep track of game play time
	float playTime = 0.0f;
};