#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuButton.h"

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

	// 1 = Left side, 2 = Middle, 3 = Right side
	const int buttonsPosition = 2;

	// Button pannel
	sf::RectangleShape buttonAreaRect;

	// Buttons visible in main menu
	MenuButton buttonResume = { data };
	MenuButton buttonSettings = { data };
	MenuButton buttonMainMenu = { data };

	// helper functions
	sf::Vector2i getButtonPosition(int buttonNum, int side = 2);
	sf::Vector2i getButtonSize();

	// Window resolution at the moment of menu initialization
	sf::Vector2u renderRes;
};