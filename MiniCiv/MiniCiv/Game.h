#pragma once

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include "Map.h"

class Game
{
public:
	Game(sf::RenderWindow& window, sf::Clock& clock, bool& VSyncState);
	void Go();
private:
	void ComposeFrame();
	void UpdateModel(float dt);
	/************************/
	/* User Functions       */
	/************************/
private:
	sf::RenderWindow& window;
	sf::Clock& clock;
	bool& VSync;
	/************************/
	/* User Variables       */
	/************************/
	Debug debug;
	Config config = { "Settings\\config.txt" };
	sf::Font font;
	Map world = { window, debug, config };
	sf::Vector2i lastMousePressAt = { 0, 0 };
	bool drawFps;
	float LPressTime = 0.0f;
};