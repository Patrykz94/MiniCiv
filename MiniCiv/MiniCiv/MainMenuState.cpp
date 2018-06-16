#include "MainMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"

MainMenuState::MainMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{
	// get full screen modes
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	for (std::size_t i = 0; i < modes.size(); ++i)
	{
		sf::VideoMode mode = modes[i];
		std::cout	<< "Mode #" << i << ": "
					<< mode.width << "x" << mode.height << " - "
					<< mode.bitsPerPixel << " bpp" << std::endl;
	}
}

void MainMenuState::Init()
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
	buttonGenerateMap.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonSize.y * 0 * 1.2f));
	buttonOptions.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonSize.y * 1 * 1.2f));
	buttonExit.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonSize.y * 2 * 1.2f));
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
					data->machine.AddState(StateRef(new GameState(data)), true);
					break;
				}
				else if (buttonOptions.IsSelected())
				{
					std::cout << "Show Options Menu" << std::endl;
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