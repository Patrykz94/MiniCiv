#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuButton.h"
#include "MenuSelection.h"

class GraphicsMenuState : public State
{
public:
	GraphicsMenuState(GameDataRef dataIn);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
private:
	GameDataRef data;

	sf::Color backgroundColor = sf::Color::Color(38, 38, 38);
	sf::Sprite title;

	// Full screen mode names
	std::vector<std::string> modeNames;

	// 1 = Left side, 2 = Middle, 3 = Right side
	const int buttonsPositionLeft = 1;
	// 1 = Left side, 2 = Middle, 3 = Right side
	const int buttonsPositionRight = 3;

	// Button pannel
	sf::RectangleShape buttonAreaRect;
	sf::RectangleShape buttonAreaRect2;

	// Buttons visible in main menu
	MenuSelection selectionDisplayMode = { data };
	MenuSelection selectionResolution = { data };
	MenuSelection selectionVSync = { data };
	MenuButton buttonApply = { data };
	MenuButton buttonBack = { data };

	// helper functions
	sf::Vector2i getButtonPosition(int buttonNum, int side = 2);
	sf::Vector2i getButtonSize();
};