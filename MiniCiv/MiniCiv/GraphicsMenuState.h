#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"
#include "MenuOption.h"

#include <map>

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

	std::map<std::string, sf::VideoMode> modes;

	// Buttons visible in main menu
	MenuOption buttonDisplayMode = { MenuOption::Type::Selection, "Display Mode",{ "Fullscreen", "Window", "Borderless Window" }, data, false };
	MenuOption buttonResolution = { MenuOption::Type::Selection, "Resolution", data, false };
	MenuOption buttonVSync = { MenuOption::Type::Selection, "Enable Vsync",{ "Yes", "No" }, data, false };
	MenuOption buttonBack = { MenuOption::Type::Button, "Back", data };
};