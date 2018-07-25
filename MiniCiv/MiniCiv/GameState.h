#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

#include "Map_Continents.h"

class GameState : public State
{
public:
	GameState(GameDataRef dataIn);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef data;
	sf::Font font;
	sf::Clock clock;
	sf::Color backgroundColor = sf::Color::Color(38, 38, 38);

	sf::View mapView;
	sf::View minimapView;

	Map_Continents world = { data, mapView };
	
	sf::Vector2i lastMousePressAt = { 0, 0 };
	float LPressTime = 0.0f;
};