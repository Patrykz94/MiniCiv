#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

#include "Map.h"

class GameState : public State
{
public:
	GameState(GameDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;
	sf::Font _font;
	sf::Clock _clock;
	sf::Color _backgroundColor = sf::Color::Color(38, 38, 38);
	Debug _debug;
	Config _config = { CONFIG_DIR };

	bool VSync;
	bool drawFps;
	Map world = { _data->window, _debug, _config };
	
	sf::Vector2i lastMousePressAt = { 0, 0 };
	float LPressTime = 0.0f;
};