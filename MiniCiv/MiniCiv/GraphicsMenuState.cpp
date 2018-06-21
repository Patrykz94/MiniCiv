#include "GraphicsMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"

GraphicsMenuState::GraphicsMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{}

void GraphicsMenuState::Init()
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

	// Set selection options and defaults
	std::vector<std::string> displayOptions = { "Fullscreen", "Window", "Borderless Window" };
	int defaultDisplayOption = 0;
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	for (sf::VideoMode& mode : modes)
	{
		modeNames.emplace_back(std::to_string(mode.width) + "x" + std::to_string(mode.height));
	}
	int defaultModeOption = 0;
	std::vector<std::string> VsyncOptions = { "Yes", "No" };
	int defaultVSyncOption = 0;
	
	// Initialize our buttons and options
	selectionDisplayMode = MenuSelection("Display Mode", getButtonPosition(0, buttonsPositionLeft), displayOptions, getButtonPosition(0, buttonsPositionRight), getButtonSize(), data, defaultDisplayOption, false);
	selectionResolution = MenuSelection("Resolution", getButtonPosition(1, buttonsPositionLeft), modeNames, getButtonPosition(1, buttonsPositionRight), getButtonSize(), data, defaultModeOption, true);
	selectionVSync = MenuSelection("Vsync Enabled", getButtonPosition(2, buttonsPositionLeft), VsyncOptions, getButtonPosition(2, buttonsPositionRight), getButtonSize(), data, defaultVSyncOption, false);
	buttonApply = MenuButton("Apply", getButtonPosition(-1, buttonsPositionRight), getButtonSize(), data, false);
	buttonBack = MenuButton("Back", getButtonPosition(-1, buttonsPositionLeft), getButtonSize(), data);
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
				////////////////////////////////////////////////////////
				if (selectionDisplayMode.IsSelectedLeft())
				{
					std::cout << "Move Left" << std::endl;
					selectionDisplayMode.MoveLeft();
					break;
				}
				else if (selectionDisplayMode.IsSelectedRight())
				{
					std::cout << "Move Right" << std::endl;
					selectionDisplayMode.MoveRight();
					break;
				}
				////////////////////////////////////////////////////////
				else if (selectionResolution.IsSelectedLeft())
				{
					std::cout << "Move Left" << std::endl;
					selectionResolution.MoveLeft();
					break;
				}
				else if (selectionResolution.IsSelectedRight())
				{
					std::cout << "Move Right" << std::endl;
					selectionResolution.MoveRight();
					break;
				}
				////////////////////////////////////////////////////////
				else if (selectionVSync.IsSelectedLeft())
				{
					std::cout << "Move Left" << std::endl;
					selectionVSync.MoveLeft();
					break;
				}
				else if (selectionVSync.IsSelectedRight())
				{
					std::cout << "Move Right" << std::endl;
					selectionVSync.MoveRight();
					break;
				}
				////////////////////////////////////////////////////////
				else if (buttonApply.IsSelected())
				{
					std::cout << "Apply Settings" << std::endl;
					//data->machine.RemoveState();
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

void GraphicsMenuState::Update(float dt)
{
	// Selections highlight code
	if (selectionDisplayMode.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionDisplayMode.SelectLeft();
	else selectionDisplayMode.DeselectLeft();
	if (selectionDisplayMode.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionDisplayMode.SelectRight();
	else selectionDisplayMode.DeselectRight();

	if (selectionResolution.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionResolution.SelectLeft();
	else selectionResolution.DeselectLeft();
	if (selectionResolution.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionResolution.SelectRight();
	else selectionResolution.DeselectRight();

	if (selectionVSync.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionVSync.SelectLeft();
	else selectionVSync.DeselectLeft();
	if (selectionVSync.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionVSync.SelectRight();
	else selectionVSync.DeselectRight();

	// Buttons highlight code
	if (buttonApply.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonApply.Select();
	else buttonApply.Deselect();

	if (buttonBack.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonBack.Select();
	else buttonBack.Deselect();
}

void GraphicsMenuState::Draw(float dt)
{
	// clear window
	data->window.clear(backgroundColor);

	// draw title/logo
	data->window.draw(title);

	data->window.draw(buttonAreaRect);
	data->window.draw(buttonAreaRect2);

	// draw buttons
	selectionDisplayMode.Draw();
	selectionResolution.Draw();
	selectionVSync.Draw();
	buttonApply.Draw();
	buttonBack.Draw();

	// display frame
	data->window.display();
}

sf::Vector2i GraphicsMenuState::getButtonSize()
{
	int width = (int)((float)data->window.getSize().x * 0.44f);
	int height = (int)((float)data->window.getSize().y / 16.0f);
	return sf::Vector2i(width, height);
}

sf::Vector2i GraphicsMenuState::getButtonPosition(int buttonNum, int side)
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
