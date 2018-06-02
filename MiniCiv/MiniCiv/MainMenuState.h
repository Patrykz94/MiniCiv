#pragma once

#include <SFML/Graphics.hpp>
#include "State.h"
#include "Game.h"

class MainMenuState : public State
{
public:
	MainMenuState(GameDataRef data);

	void Init();

	void HandleInput();
	void Update(float dt);
	void Draw(float dt);

private:
	GameDataRef _data;

	sf::Font _menuFont;

	sf::Color _backgroundColor = sf::Color::Color(38, 38, 38);
	sf::Sprite _title;

	// Buttons visible in main menu
	sf::Sprite _generateMapButton;
	sf::Text _generateMapButtonText;

	sf::Sprite _optionsButton;
	sf::Text _optionsButtonText;

	sf::Sprite _exitButton;
	sf::Text _exitButtonText;
};