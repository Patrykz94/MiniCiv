#include "GameState.h"

#include <sstream>
#include <iostream>

GameState::GameState(GameDataRef data)
	:
	_data(data)
{
	//sf::VideoMode mode = sf::VideoMode::getFullscreenModes()[0];
	//_data->window.create(sf::VideoMode(mode.width, mode.height), "MiniCiv");

	// check if displaying fps
	if (_config.Get(Config::Option::DebugFPS) == 1)
		drawFps = true;
	else
		drawFps = false;

	// check if vsync enabled
	if (_config.Get(Config::Option::Vsync) == 1)
		VSync = true;
	else
		VSync = false;

	// update vsync
	_data->window.setVerticalSyncEnabled(VSync);
}

void GameState::Init()
{
	this->_font = this->_data->assets.GetFont("Helvetica Font");
}

void GameState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			// close window and exit
			this->_data->window.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				// go to main menu
				std::cout << "Go to Main Menu" << std::endl;
				break;
			}
			break;
		}
	}
}

void GameState::Update(float dt)
{
	// update debug object
	_debug.Update(dt);

	// process map scroll
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && _data->window.hasFocus())
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition();
		if (LPressTime > 0.15f || (LPressTime > 0.0f && lastMousePressAt != mousePos))
		{
			world.MoveView(lastMousePressAt - mousePos);
		}
		LPressTime += dt;
		lastMousePressAt = mousePos;
	}
	else
	{
		// if mouse released, reset time back to 0
		LPressTime = 0.0f;
	}
}

void GameState::Draw(float dt)
{
	this->_data->window.clear(_backgroundColor);

	world.Draw();
	if (drawFps)
	{
		_debug.DrawFPS(_data->window);
	}

	this->_data->window.display();
}
