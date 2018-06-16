#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuOption.h"

class MainMenuState : public State
{
public:
	MainMenuState(GameDataRef dataIn);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef data;

	sf::Color backgroundColor = sf::Color::Color(38, 38, 38);
	sf::Sprite title;

	// Buttons visible in main menu
	MenuOption buttonGenerateMap = { MenuOption::Type::Button, "Generate Map", data };
	MenuOption buttonOptions = { MenuOption::Type::Button, "Options", data, false };
	MenuOption buttonExit = { MenuOption::Type::Button, "Exit", data };
};