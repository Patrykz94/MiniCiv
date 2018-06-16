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
	int buttonFontSize = (int)((data->window.getSize().y - title.getGlobalBounds().height - title.getPosition().y) * 0.5f / numButtons);
	buttonResume.SetFontSize(buttonFontSize);
	buttonOptions.SetFontSize(buttonFontSize);
	buttonMainMenu.SetFontSize(buttonFontSize);

	sf::Vector2f buttonPos = sf::Vector2f(title.getPosition().x, (float)title.getGlobalBounds().height + title.getPosition().y + (float)buttonFontSize / 5.0f);

	// set button positions
	buttonResume.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 0 / 0.5f));
	buttonOptions.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 1 / 0.5f));
	buttonMainMenu.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 2 / 0.5f));
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
					std::cout << "Return to map" << std::endl;
					data->machine.RemoveState();
					break;
				}
				else if (buttonOptions.IsSelected())
				{
					std::cout << "Show options menu" << std::endl;
					break;
				}
				else if (buttonMainMenu.IsSelected())
				{
					std::cout << "Go back to main menu" << std::endl;
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