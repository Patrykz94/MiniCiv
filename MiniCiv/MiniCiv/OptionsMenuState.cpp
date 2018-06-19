#include "OptionsMenuState.h"

#include <sstream>
#include <iostream>

#include "GraphicsMenuState.h"

OptionsMenuState::OptionsMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{}

void OptionsMenuState::Init()
{
	title.setTexture(data->assets.GetTexture("splashStateBackground"));

	// Set correct dimensions and positions of title image
	float titleScaleFactor = (data->window.getSize().x / 2) / title.getGlobalBounds().width;
	title.setScale(titleScaleFactor, titleScaleFactor);
	title.setPosition((float)data->window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, title.getGlobalBounds().height * 0.1f);

	// Set size and positions for buttons
	int buttonFontSize = (int)((data->window.getSize().y - title.getGlobalBounds().height - title.getPosition().y) * 0.5f / numButtons);

	buttonInterface.SetFontSize(buttonFontSize);
	buttonGraphics.SetFontSize(buttonFontSize);
	buttonBack.SetFontSize(buttonFontSize);

	sf::Vector2f buttonPos = sf::Vector2f(title.getPosition().x, (float)title.getGlobalBounds().height + title.getPosition().y + (float)buttonFontSize / 5.0f);

	// set button positions
	buttonInterface.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 0 / 0.5f));
	buttonGraphics.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 1 / 0.5f));
	buttonBack.SetPosition(buttonPos + sf::Vector2f(0.0f, buttonFontSize * 2 / 0.5f));
}

void OptionsMenuState::HandleInput()
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
				if (buttonInterface.IsSelected())
				{
					std::cout << "Show interface options" << std::endl;
					break;
				}
				else if (buttonGraphics.IsSelected())
				{
					std::cout << "Show graphics option" << std::endl;
					data->machine.AddState(StateRef(new GraphicsMenuState(data)), false);
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

void OptionsMenuState::Update(float dt)
{}

void OptionsMenuState::Draw(float dt)
{
	// clear window
	data->window.clear(backgroundColor);

	// draw title/logo
	data->window.draw(title);

	// draw buttons
	buttonInterface.Draw();
	buttonGraphics.Draw();
	buttonBack.Draw();

	// display frame
	data->window.display();
}