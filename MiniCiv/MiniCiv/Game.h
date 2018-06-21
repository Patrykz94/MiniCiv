#pragma once

#include <memory>
#include <string>
#include <SFML/Graphics.hpp>

#include "StateMachine.h"
#include "AssetManager.h"
#include "InputManager.h"
#include "SettingsManager.h"
#include "DEFINITIONS.h"

struct GameData
{
	StateMachine machine;
	sf::RenderWindow window;
	AssetManager assets;
	InputManager input;
	SettingsManager settings;
};

typedef std::shared_ptr<GameData> GameDataRef;

class Game
{
public:
	Game(std::string title);

private:
	const float dt = 1.0f / 60.0f;
	sf::Clock clock;

	GameDataRef data = std::make_shared<GameData>();

	void Run();
};