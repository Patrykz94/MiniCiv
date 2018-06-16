#include "MainMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"

MainMenuState::MainMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{}

void MainMenuState::Init()
{
	title.setTexture(data->assets.GetTexture("splashStateBackground"));

	// Set correct dimensions and positions of title image
	float titleScaleFactor = (data->window.getSize().x / 2) / title.getGlobalBounds().width;
	title.setScale(titleScaleFactor, titleScaleFactor);
	title.setPosition((float)data->window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, title.getGlobalBounds().height * 0.1f);

	// Set size and positions for buttons
	int buttonFontSize = (int)((data->window.getSize().y - title.getGlobalBounds().height - title.getPosition().y) * 0.5f / numButtons);
	buttonGenerateMap.SetFontSize(buttonFontSize);
	buttonOptions.SetFontSize(buttonFontSize);
	buttonExit.SetFontSize(buttonFontSize);

	sf::Vector2f buttonPos = sf::Vector2f(title.getPosition().x, (float)title.getGlobalBounds().height + title.getPosition().y + (float)buttonFontSize / 5.0f);

	// set button positions
	buttonGenerateMap.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 0 / 0.5f));
	buttonOptions.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 1 / 0.5f));
	buttonExit.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 2 / 0.5f));
}

void MainMenuState::HandleInput()
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
				if (buttonGenerateMap.IsSelected())
				{
					std::cout << "Go to game and generate map" << std::endl;
					data->machine.AddState(StateRef(new GameState(data)), true);
					break;
				}
				else if (buttonOptions.IsSelected())
				{
					std::cout << "Show options menu" << std::endl;
					break;
				}
				else if (buttonExit.IsSelected())
				{
					data->window.close();
					break;
				}
			}
			break;
		}
	}
}

void MainMenuState::Update(float dt)
{}

void MainMenuState::Draw(float dt)
{
	// clear window
	data->window.clear(backgroundColor);

	// draw title/logo
	data->window.draw(title);

	// draw buttons
	buttonGenerateMap.Draw();
	buttonOptions.Draw();
	buttonExit.Draw();
	
	// display frame
	data->window.display();
}