#include "GameState.h"

#include <sstream>
#include <iostream>

#include "PauseMenuState.h"

GameState::GameState(GameDataRef dataIn)
	:
	data(dataIn)
{
	// check if displaying fps
	if (config.Get(Config::Option::DebugFPS) == 1)
		drawFps = true;
	else
		drawFps = false;

	// check if vsync enabled
	if (config.Get(Config::Option::Vsync) == 1)
		VSync = true;
	else
		VSync = false;

	// update vsync
	data->window.setVerticalSyncEnabled(VSync);
}

void GameState::Init()
{
	font = data->assets.GetFont("menuFont");
}

void GameState::HandleInput()
{
	sf::Event event;

	while (data->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			// close window and exit
			data->window.close();
			break;
		case sf::Event::KeyReleased:
			switch (event.key.code)
			{
			case sf::Keyboard::Escape:
				// go to main menu
				std::cout << "Go to Main Menu" << std::endl;
				data->machine.AddState(StateRef(new PauseMenuState(data)), false);
				break;
			}
			break;
		}
	}
}

void GameState::Update(float dt)
{
	// update debug object
	debug.Update(dt);

	// process map scroll
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && data->window.hasFocus())
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
	data->window.clear(backgroundColor);

	world.Draw();
	if (drawFps)
	{
		debug.DrawFPS(data->window);
	}

	data->window.display();
}
