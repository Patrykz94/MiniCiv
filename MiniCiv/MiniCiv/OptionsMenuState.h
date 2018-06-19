#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuOption.h"

class OptionsMenuState : public State
{
public:
	OptionsMenuState(GameDataRef dataIn);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
private:
	GameDataRef data;

	sf::Color backgroundColor = sf::Color::Color(38, 38, 38);
	sf::Sprite title;

	// Buttons visible in main menu
	MenuOption buttonInterface = { MenuOption::Type::Button, "Gameplay", data, false };
	MenuOption buttonGraphics = { MenuOption::Type::Button, "Graphics", data, false };
	MenuOption buttonBack = { MenuOption::Type::Button, "Back", data };

	// number of buttons on this menu
	int numButtons = 3;
};