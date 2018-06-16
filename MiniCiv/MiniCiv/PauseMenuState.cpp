#include "PauseMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"
#include "MainMenuState.h"

PauseMenuState::PauseMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{}

void PauseMenuState::Init()
{
	title.setTexture(data->assets.GetTexture("splashStateBackground"));

	// Set correct dimensions and positions of title image
	float titleScaleFactor = (data->window.getSize().x / 2) / title.getGlobalBounds().width;
	title.setScale(titleScaleFactor, titleScaleFactor);
	title.setPosition((float)data->window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, title.getGlobalBounds().height * 0.1f);

	// Set size and positions for buttons
	sf::Vector2f buttonSize = sf::Vector2f((sf::Vector2f)data->assets.GetTexture("Main Menu Button").getSize());
	sf::Vector2f buttonPos = sf::Vector2f((float)data->window.getSize().x / 2.0f - buttonSize.x / 2.0f, (float)title.getGlobalBounds().height * 1.2f);

	// set button positions
	buttonResume.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonSize.y * 0 * 1.2f));
	buttonOptions.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonSize.y * 1 * 1.2f));
	buttonMainMenu.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonSize.y * 2 * 1.2f));
}

void PauseMenuState::HandleInput()
{
	sf::Event event;

	while (data->window.pollEvent(event))
	{
		switch (event.type)
		{
		case sf::Event::Closed:
			data->window.close();
			break;
		case sf::Event::MouseButtonReleased:
			if (event.mouseButton.button == 0)
			{
				if (buttonResume.IsSelected())
				{
					// This goes to main menu instead...
					data->machine.RemoveState();
					break;
				}
				else if (buttonOptions.IsSelected())
				{
					std::cout << "Show Options Menu" << std::endl;
					break;
				}
				else if (buttonMainMenu.IsSelected())
				{
					data->machine.RemoveState();
					data->machine.AddState(StateRef(new MainMenuState(data)), true);
					break;
				}
			}
			break;
		}
	}
}

void PauseMenuState::Update(float dt)
{}

void PauseMenuState::Draw(float dt)
{
	// clear window
	data->window.clear(backgroundColor);

	// draw title/logo
	data->window.draw(title);

	// draw buttons
	buttonResume.Draw();
	buttonOptions.Draw();
	buttonMainMenu.Draw();

	// display frame
	data->window.display();
}