#pragma once

#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuButton.h"
#include "MenuSelection.h"

class InterfaceMenuState : public State
{
public:
	InterfaceMenuState(GameDataRef dataIn);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);
private:
	GameDataRef data;

	sf::Color backgroundColor = sf::Color::Color(38, 38, 38);
	sf::Sprite title;

	// 1 = Left side, 2 = Middle, 3 = Right side
	const int buttonsPositionLeft = 1;
	// 1 = Left side, 2 = Middle, 3 = Right side
	const int buttonsPositionRight = 3;

	// Button pannel
	sf::RectangleShape buttonAreaRect;
	sf::RectangleShape buttonAreaRect2;

	// Buttons/Selections visible in main menu
	MenuSelection selectionShowFPS = { data };
	MenuSelection selectionShowGridCoords = { data };
	MenuButton buttonApply = { data };
	MenuButton buttonBack = { data };

	// Selection option names
	std::vector<std::string> fpsOptionNames;
	std::vector<std::string> gridCoordsOptionNames;

	// Selection option values
	std::vector<bool> fpsOptions;
	std::vector<bool> gridCoordsOptions;

	// Default selection options
	int defaultFpsOption;
	int defaultGridCoordOption;

	// helper functions
	sf::Vector2i getButtonPosition(int buttonNum, int side = 2);
	sf::Vector2i getButtonSize();

	// Window resolution at the moment of menu initialization
	sf::Vector2u renderRes;
};