#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

class SplashState : public State
{
public:
	SplashState(GameDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef data;
	sf::Clock clock;
	sf::Sprite background;
};