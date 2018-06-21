#include "PauseMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"
#include "MainMenuState.h"
#include "SettingsMenuState.h"

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

	buttonResume = MenuButton("Resume Game", getButtonPosition(0, buttonsPosition), getButtonSize(), data);
	buttonSettings = MenuButton("Settings", getButtonPosition(1, buttonsPosition), getButtonSize(), data);
	buttonMainMenu = MenuButton("Back to Main Menu", getButtonPosition(-1, buttonsPosition), getButtonSize(), data);

	buttonAreaRect = sf::RectangleShape(sf::Vector2f((float)getButtonSize().x + 20.0f, (float)data->window.getSize().y - title.getPosition().y * 2.0f - title.getGlobalBounds().height - (float)getButtonSize().y * 1.2f + 20.0f));
	buttonAreaRect.setPosition(sf::Vector2f((float)getButtonPosition(0, buttonsPosition).x - 10.0f, (float)getButtonPosition(0, buttonsPosition).y - 10.0f));
	buttonAreaRect.setFillColor(sf::Color(20, 20, 20));

	renderRes = data->window.getSize();
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
				}
				else if (buttonSettings.IsSelected())
				{
					std::cout << "Show options menu" << std::endl;
					data->machine.AddState(StateRef(new SettingsMenuState(data)), false);
				}
				else if (buttonMainMenu.IsSelected())
				{
					std::cout << "Go back to main menu" << std::endl;
					data->machine.RemoveState();
					data->machine.AddState(StateRef(new MainMenuState(data)), true);
				}
			}
			break;
		}
	}
}

void PauseMenuState::Update(float dt)
{
	// If resolution has changed, re-initialize the menu
	if (renderRes != data->window.getSize()) data->machine.AddState(StateRef(new PauseMenuState(data)), true);;

	if (buttonResume.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonResume.Select();
	else buttonResume.Deselect();

	if (buttonSettings.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonSettings.Select();
	else buttonSettings.Deselect();

	if (buttonMainMenu.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonMainMenu.Select();
	else buttonMainMenu.Deselect();
}

void PauseMenuState::Draw(float dt)
{
	// Only draw UI if the resolution is correct
	if (renderRes == data->window.getSize()) {
		// clear window
		data->window.clear(backgroundColor);

		// draw title/logo
		data->window.draw(title);

		data->window.draw(buttonAreaRect);

		// draw buttons
		buttonResume.Draw();
		buttonSettings.Draw();
		buttonMainMenu.Draw();

		// display frame
		data->window.display();
	}
}

sf::Vector2i PauseMenuState::getButtonSize()
{
	int width = (int)((float)data->window.getSize().x * 0.44f);
	int height = (int)((float)data->window.getSize().y / 16.0f);
	return sf::Vector2i(width, height);
}

sf::Vector2i PauseMenuState::getButtonPosition(int buttonNum, int side)
{
	int x;
	int y;

	if (side == 1) x = (int)((float)data->window.getSize().x / 20.0f);
	else if (side == 2) x = (int)((float)data->window.getSize().x / 2.0f - (float)getButtonSize().x / 2.0f);
	else if (side == 3) x = data->window.getSize().x - (int)((float)data->window.getSize().x / 20.0f) - getButtonSize().x;
	else
	{
		x = 0;
		std::cout << "ERROR: Incorrect button position" << std::endl;
	}

	if (buttonNum == -1) y = (int)((float)data->window.getSize().y - (float)getButtonSize().y * 2.2f);
	else y = (int)(title.getPosition().y * 2.0f + title.getGlobalBounds().height + (float)getButtonSize().y * 1.2f * (float)buttonNum);

	return sf::Vector2i(x, y);
}
