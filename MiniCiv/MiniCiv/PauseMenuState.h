#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuOption.h"

class PauseMenuState : public State
{
public:
	PauseMenuState(GameDataRef dataIn);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
private:
	GameDataRef data;

	sf::Color backgroundColor = sf::Color::Color(38, 38, 38);
	sf::Sprite title;

	// Buttons visible in main menu
	MenuOption buttonResume = { MenuOption::Type::Button, "Resume Game", data };
	MenuOption buttonOptions = { MenuOption::Type::Button, "Options", data, false };
	MenuOption buttonMainMenu = { MenuOption::Type::Button, "Exit to Main Menu", data };
};