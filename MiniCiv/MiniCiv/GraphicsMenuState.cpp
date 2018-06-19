#include "GraphicsMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"

GraphicsMenuState::GraphicsMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{
	std::vector<sf::VideoMode> tempModes = sf::VideoMode::getFullscreenModes();
	std::vector<std::string> modeNames;
	for (sf::VideoMode mode : tempModes)
	{
		modes.emplace(std::to_string(mode.width) + "x" + std::to_string(mode.height), mode);
		modeNames.emplace_back(std::to_string(mode.width) + "x" + std::to_string(mode.height));
	}
	buttonResolution.AddOptions(modeNames);
}

void GraphicsMenuState::Init()
{
	title.setTexture(data->assets.GetTexture("splashStateBackground"));

	// Set correct dimensions and positions of title image
	float titleScaleFactor = (data->window.getSize().x / 2) / title.getGlobalBounds().width;
	title.setScale(titleScaleFactor, titleScaleFactor);
	title.setPosition((float)data->window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, title.getGlobalBounds().height * 0.1f);

	// Set size and positions for buttons
	int buttonFontSize = data->window.getSize().y / 20;

	buttonDisplayMode.SetFontSize(buttonFontSize);
	buttonResolution.SetFontSize(buttonFontSize);
	buttonVSync.SetFontSize(buttonFontSize);
	buttonBack.SetFontSize(buttonFontSize);

	float buttonPosXOffset = data->window.getSize().x / 20.0f;
	float buttonPosY = title.getGlobalBounds().height + title.getPosition().y + (float)buttonFontSize / 5.0f;

	// set button positions
	buttonDisplayMode.SetPosition(buttonPosXOffset, buttonPosY + buttonFontSize * 0 / 0.5f, data->window.getSize().x - buttonPosXOffset, buttonPosY + buttonFontSize * 0 / 0.5f);
	buttonResolution.SetPosition(buttonPosXOffset, buttonPosY + buttonFontSize * 1 / 0.5f, data->window.getSize().x - buttonPosXOffset, buttonPosY + buttonFontSize * 1 / 0.5f);
	buttonVSync.SetPosition(buttonPosXOffset, buttonPosY + buttonFontSize * 2 / 0.5f, data->window.getSize().x - buttonPosXOffset, buttonPosY + buttonFontSize * 2 / 0.5f);
	buttonBack.SetPosition(buttonPosXOffset, buttonPosY + buttonFontSize * 3 / 0.5f);
}

void GraphicsMenuState::HandleInput()
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
				if (buttonResolution.IsSelected())
				{
					std::cout << "Show interface options" << std::endl;
					break;
				}
				else if (buttonVSync.IsSelected())
				{
					std::cout << "Show graphics option" << std::endl;
					break;
				}
				else if (buttonBack.IsSelected())
				{
					std::cout << "Go back to menu" << std::endl;
					data->machine.RemoveState();
					break;
				}
			}
			break;
		}
	}
}

void GraphicsMenuState::Update(float dt)
{}

void GraphicsMenuState::Draw(float dt)
{
	// clear window
	data->window.clear(backgroundColor);

	// draw title/logo
	data->window.draw(title);

	// draw buttons
	buttonDisplayMode.Draw();
	buttonResolution.Draw();
	buttonVSync.Draw();
	buttonBack.Draw();

	// display frame
	data->window.display();
}