#include "GameState.h"

#include <sstream>
#include <iostream>

#include "PauseMenuState.h"

GameState::GameState(GameDataRef dataIn)
	:
	data(dataIn)
{}

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
				std::cout << "Go to pause menu" << std::endl;
				data->machine.AddState(StateRef(new PauseMenuState(data)), false);
				break;
			}
			break;
		case sf::Event::MouseWheelScrolled:
			float scrolDelta = event.mouseWheelScroll.delta;
			if (scrolDelta > 0.0f)
			{
				world.ZoomIn();
			}
			else
			{
				world.ZoomOut();
			}
			break;
		}
	}
}

void GameState::Update(float dt)
{
	// update debug object
	data->debug.Update(dt);

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

	// At the end, call update function on map which makes sure everything will be displayed correctly
	world.UpdateView();
}

void GameState::Draw(float dt)
{
	data->window.clear(backgroundColor);
	// set map view for drawing all the things in the world
	data->window.setView(mapView);
	world.Draw();
	// set generic view for drawing UI
	data->window.setView(data->window.getDefaultView());
	if (data->settings.GetShowFPS()) data->debug.DrawFPS(data->window);

	data->window.display();
}
