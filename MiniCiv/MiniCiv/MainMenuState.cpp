#include "MainMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"
#include "SettingsMenuState.h"

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

	buttonGenerateMap = MenuButton("Generate Map", getButtonPosition(0, buttonsPosition), getButtonSize(), data);
	buttonSettings = MenuButton("Settings", getButtonPosition(1, buttonsPosition), getButtonSize(), data);
	buttonExit = MenuButton("Exit to Desktop", getButtonPosition(-1, buttonsPosition), getButtonSize(), data);

	buttonAreaRect = sf::RectangleShape(sf::Vector2f((float)getButtonSize().x + 20.0f, (float)data->window.getSize().y - title.getPosition().y * 2.0f - title.getGlobalBounds().height - (float)getButtonSize().y * 1.2f + 20.0f));
	buttonAreaRect.setPosition(sf::Vector2f((float)getButtonPosition(0, buttonsPosition).x - 10.0f, (float)getButtonPosition(0, buttonsPosition).y - 10.0f));
	buttonAreaRect.setFillColor(sf::Color(20,20,20));

	renderRes = data->window.getSize();
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
				else if (buttonSettings.IsSelected())
				{
					std::cout << "Show settings menu" << std::endl;
					data->machine.AddState(StateRef(new SettingsMenuState(data)), false);
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
{
	// If resolution has changed, re-initialize the menu
	if (renderRes != data->window.getSize()) data->machine.AddState(StateRef(new MainMenuState(data)), true);;

	if (buttonGenerateMap.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonGenerateMap.Select();
	else buttonGenerateMap.Deselect();

	if (buttonSettings.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonSettings.Select();
	else buttonSettings.Deselect();

	if (buttonExit.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonExit.Select();
	else buttonExit.Deselect();
}

void MainMenuState::Draw(float dt)
{
	// Only draw UI if the resolution is correct
	if (renderRes == data->window.getSize()) {
		// clear window
		data->window.clear(backgroundColor);

		// draw title/logo
		data->window.draw(title);

		data->window.draw(buttonAreaRect);

		// draw buttons
		buttonGenerateMap.Draw();
		buttonSettings.Draw();
		buttonExit.Draw();

		// display frame
		data->window.display();
	}
}

sf::Vector2i MainMenuState::getButtonSize()
{
	int width = (int)((float)data->window.getSize().x * 0.44f);
	int height = (int)((float)data->window.getSize().y / 16.0f);
	return sf::Vector2i(width, height);
}

sf::Vector2i MainMenuState::getButtonPosition(int buttonNum, int side)
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
