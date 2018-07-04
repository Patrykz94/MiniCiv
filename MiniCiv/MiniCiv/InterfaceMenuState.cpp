#include "InterfaceMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"

InterfaceMenuState::InterfaceMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{
	// Set selection options and defaults
	fpsOptionNames = { "Yes", "No" };
	fpsOptions = { true, false };
	if (data->settings.GetShowFPS()) defaultFpsOption = 0;
	else defaultFpsOption = 1;

	gridCoordsOptionNames = { "Yes", "No" };
	gridCoordsOptions = { true, false };
	if (data->settings.GetShowGridCoords()) defaultGridCoordOption = 0;
	else defaultGridCoordOption = 1;
}

void InterfaceMenuState::Init()
{
	title.setTexture(data->assets.GetTexture("splashStateBackground"));

	// Set correct dimensions and positions of title image
	float titleScaleFactor = (data->window.getSize().x / 2) / title.getGlobalBounds().width;
	title.setScale(titleScaleFactor, titleScaleFactor);
	title.setPosition((float)data->window.getSize().x / 2.0f - title.getGlobalBounds().width / 2.0f, title.getGlobalBounds().height * 0.1f);

	// Set 2 button pannels, one for labels and one for options
	buttonAreaRect = sf::RectangleShape(sf::Vector2f((float)getButtonSize().x + 20.0f, (float)data->window.getSize().y - title.getPosition().y * 2.0f - title.getGlobalBounds().height - (float)getButtonSize().y * 1.2f + 20.0f));
	buttonAreaRect.setPosition(sf::Vector2f((float)getButtonPosition(0, buttonsPositionLeft).x - 10.0f, (float)getButtonPosition(0, buttonsPositionLeft).y - 10.0f));
	buttonAreaRect.setFillColor(sf::Color(20, 20, 20));

	buttonAreaRect2 = sf::RectangleShape(sf::Vector2f((float)getButtonSize().x + 20.0f, (float)data->window.getSize().y - title.getPosition().y * 2.0f - title.getGlobalBounds().height - (float)getButtonSize().y * 1.2f + 20.0f));
	buttonAreaRect2.setPosition(sf::Vector2f((float)getButtonPosition(0, buttonsPositionRight).x - 10.0f, (float)getButtonPosition(0, buttonsPositionRight).y - 10.0f));
	buttonAreaRect2.setFillColor(sf::Color(20, 20, 20));

	// Initialize our buttons and options
	selectionShowFPS = MenuSelection("Show FPS", getButtonPosition(0, buttonsPositionLeft), fpsOptionNames, getButtonPosition(0, buttonsPositionRight), getButtonSize(), data, defaultFpsOption);
	selectionShowGridCoords = MenuSelection("Show Grid Coordinates", getButtonPosition(1, buttonsPositionLeft), gridCoordsOptionNames, getButtonPosition(1, buttonsPositionRight), getButtonSize(), data, defaultGridCoordOption);
	buttonApply = MenuButton("Apply", getButtonPosition(-1, buttonsPositionRight), getButtonSize(), data);
	buttonBack = MenuButton("Back", getButtonPosition(-1, buttonsPositionLeft), getButtonSize(), data);

	renderRes = data->window.getSize();
}

void InterfaceMenuState::HandleInput()
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
				////////////////////////////////////////////////////////
				if (selectionShowFPS.IsSelectedLeft())
				{
					std::cout << "Move Left" << std::endl;
					selectionShowFPS.MoveLeft();
					break;
				}
				else if (selectionShowFPS.IsSelectedRight())
				{
					std::cout << "Move Right" << std::endl;
					selectionShowFPS.MoveRight();
					break;
				}
				////////////////////////////////////////////////////////
				else if (selectionShowGridCoords.IsSelectedLeft())
				{
					std::cout << "Move Left" << std::endl;
					selectionShowGridCoords.MoveLeft();
					break;
				}
				else if (selectionShowGridCoords.IsSelectedRight())
				{
					std::cout << "Move Right" << std::endl;
					selectionShowGridCoords.MoveRight();
					break;
				}
				////////////////////////////////////////////////////////
				else if (buttonApply.IsSelected())
				{
					std::cout << "Apply Settings" << std::endl;

					if (fpsOptions[selectionShowFPS.GetSelectedOption()] != data->settings.GetShowFPS()) data->settings.SetShowFPS(fpsOptions[selectionShowFPS.GetSelectedOption()]);
					if (gridCoordsOptions[selectionShowGridCoords.GetSelectedOption()] != data->settings.GetShowGridCoords()) data->settings.SetShowGridCoords(gridCoordsOptions[selectionShowGridCoords.GetSelectedOption()]);
					
					break;
				}
				else if (buttonBack.IsSelected())
				{
					std::cout << "Go back to menu" << std::endl;
					data->machine.RemoveState();
					break;
				}
				////////////////////////////////////////////////////////
			}
			break;
		}
	}
}

void InterfaceMenuState::Update(float dt)
{
	// If resolution has changed, re-initialize the menu
	if (renderRes != data->window.getSize()) data->machine.AddState(StateRef(new InterfaceMenuState(data)), true);;

	// If no settings changes to apply, disable apply button
	if ((fpsOptions[selectionShowFPS.GetSelectedOption()] != data->settings.GetShowFPS()) ||
		(gridCoordsOptions[selectionShowGridCoords.GetSelectedOption()] != data->settings.GetShowGridCoords()))
	{
		buttonApply.Enable();
	}
	else
	{
		buttonApply.Disable();
	}

	// Selections highlight code
	if (selectionShowFPS.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionShowFPS.SelectLeft();
	else selectionShowFPS.DeselectLeft();
	if (selectionShowFPS.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionShowFPS.SelectRight();
	else selectionShowFPS.DeselectRight();

	if (selectionShowGridCoords.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionShowGridCoords.SelectLeft();
	else selectionShowGridCoords.DeselectLeft();
	if (selectionShowGridCoords.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionShowGridCoords.SelectRight();
	else selectionShowGridCoords.DeselectRight();

	// Buttons highlight code
	if (buttonApply.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonApply.Select();
	else buttonApply.Deselect();

	if (buttonBack.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonBack.Select();
	else buttonBack.Deselect();
}

void InterfaceMenuState::Draw(float dt)
{
	// Only draw UI if the resolution is correct
	if (renderRes == data->window.getSize()) {
		// clear window
		data->window.clear(backgroundColor);

		// draw title/logo
		data->window.draw(title);

		data->window.draw(buttonAreaRect);
		data->window.draw(buttonAreaRect2);

		// draw buttons
		selectionShowFPS.Draw();
		selectionShowGridCoords.Draw();
		buttonApply.Draw();
		buttonBack.Draw();

		// display frame
		data->window.display();
	}
}

sf::Vector2i InterfaceMenuState::getButtonSize()
{
	int width = (int)((float)data->window.getSize().x * 0.44f);
	int height = (int)((float)data->window.getSize().y / 16.0f);
	return sf::Vector2i(width, height);
}

sf::Vector2i InterfaceMenuState::getButtonPosition(int buttonNum, int side)
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
