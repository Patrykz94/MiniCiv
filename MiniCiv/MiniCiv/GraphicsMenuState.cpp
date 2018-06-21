#include "GraphicsMenuState.h"

#include <sstream>
#include <iostream>

#include "GameState.h"

GraphicsMenuState::GraphicsMenuState(GameDataRef dataIn)
	:
	data(dataIn)
{
	// Set selection options and defaults
	displayOptionNames = { "Fullscreen", "Window", "Borderless Window" };
	displayOptions = { "fullscreen", "window", "borderlesswindow" };
	if (data->settings.GetScreenModeName() == "fullscreen") defaultDisplayOption = 0;
	else if (data->settings.GetScreenModeName() == "window") defaultDisplayOption = 1;
	else if (data->settings.GetScreenModeName() == "borderlesswindow") defaultDisplayOption = 2;

	modes = sf::VideoMode::getFullscreenModes();
	int modeNum = 0;
	defaultModeOption = 0;
	for (sf::VideoMode& mode : modes)
	{
		if (sf::Vector2i(mode.width, mode.height) == data->settings.GetScreenResolution())
		{
			defaultModeOption = modeNum;
		}
		modeNames.emplace_back(std::to_string(mode.width) + "x" + std::to_string(mode.height));
		modeNum++;
	}

	VsyncOptionNames = { "Yes", "No" };
	VsyncOptions = { true, false };
	if (data->settings.GetScreenVsync()) defaultVsyncOption = 0;
	else defaultVsyncOption = 1;
}

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
	
	// Initialize our buttons and options
	selectionDisplayMode = MenuSelection("Display Mode", getButtonPosition(0, buttonsPositionLeft), displayOptionNames, getButtonPosition(0, buttonsPositionRight), getButtonSize(), data, defaultDisplayOption, false);
	selectionResolution = MenuSelection("Resolution", getButtonPosition(1, buttonsPositionLeft), modeNames, getButtonPosition(1, buttonsPositionRight), getButtonSize(), data, defaultModeOption, true);
	selectionVsync = MenuSelection("Vsync Enabled", getButtonPosition(2, buttonsPositionLeft), VsyncOptionNames, getButtonPosition(2, buttonsPositionRight), getButtonSize(), data, defaultVsyncOption, false);
	buttonApply = MenuButton("Apply", getButtonPosition(-1, buttonsPositionRight), getButtonSize(), data);
	buttonBack = MenuButton("Back", getButtonPosition(-1, buttonsPositionLeft), getButtonSize(), data);

	renderRes = data->window.getSize();
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
				else if (selectionVsync.IsSelectedLeft())
				{
					std::cout << "Move Left" << std::endl;
					selectionVsync.MoveLeft();
					break;
				}
				else if (selectionVsync.IsSelectedRight())
				{
					std::cout << "Move Right" << std::endl;
					selectionVsync.MoveRight();
					break;
				}
				////////////////////////////////////////////////////////
				else if (buttonApply.IsSelected())
				{
					std::cout << "Apply Settings" << std::endl;
					bool newDisplayModeRequired = false;
					if (displayOptions[selectionDisplayMode.GetSelectedOption()] != data->settings.GetScreenModeName()) newDisplayModeRequired = true;
					bool newResolutionRequired = false;
					if (modes[selectionResolution.GetSelectedOption()].width != data->settings.GetScreenResolution().x
						|| modes[selectionResolution.GetSelectedOption()].height != data->settings.GetScreenResolution().y) newResolutionRequired = true;
					bool newVsyncRequired = false;
					if (VsyncOptions[selectionVsync.GetSelectedOption()] != data->settings.GetScreenVsync()) newVsyncRequired = true;

					if (newDisplayModeRequired || newResolutionRequired)
					{
						int style = data->settings.GetScreenMode();
						if (newDisplayModeRequired) style = data->settings.GetScreenModeFromName(displayOptions[selectionDisplayMode.GetSelectedOption()]);

						sf::Vector2i res = data->settings.GetScreenResolution();
						if (newResolutionRequired) res = sf::Vector2i(modes[selectionResolution.GetSelectedOption()].width, modes[selectionResolution.GetSelectedOption()].height);
						
						data->window.create(sf::VideoMode(res.x, res.y), GAME_TITLE, style);
						data->settings.SetScreenMode(displayOptions[selectionDisplayMode.GetSelectedOption()]);
						data->settings.SetScreenResolution(res);
						newDisplayModeRequired = false;
						newResolutionRequired = false;

						data->window.setVerticalSyncEnabled(VsyncOptions[selectionVsync.GetSelectedOption()]);
						data->settings.SetScreenVsync(VsyncOptions[selectionVsync.GetSelectedOption()]);
						newVsyncRequired = false;
					}

					if (newVsyncRequired)
					{
						data->window.setVerticalSyncEnabled(VsyncOptions[selectionVsync.GetSelectedOption()]);
						data->settings.SetScreenVsync(VsyncOptions[selectionVsync.GetSelectedOption()]);
						newVsyncRequired = false;
					}

					// TODO: Make all UI states recalculate their dimensions after a resolution change

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
	// If resolution has changed, re-initialize the menu
	if (renderRes != data->window.getSize()) data->machine.AddState(StateRef(new GraphicsMenuState(data)), true);;

	// Selections highlight code
	if (selectionDisplayMode.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionDisplayMode.SelectLeft();
	else selectionDisplayMode.DeselectLeft();
	if (selectionDisplayMode.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionDisplayMode.SelectRight();
	else selectionDisplayMode.DeselectRight();

	if (selectionResolution.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionResolution.SelectLeft();
	else selectionResolution.DeselectLeft();
	if (selectionResolution.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionResolution.SelectRight();
	else selectionResolution.DeselectRight();

	if (selectionVsync.GetLeftRect().contains(sf::Mouse::getPosition(data->window))) selectionVsync.SelectLeft();
	else selectionVsync.DeselectLeft();
	if (selectionVsync.GetRightRect().contains(sf::Mouse::getPosition(data->window))) selectionVsync.SelectRight();
	else selectionVsync.DeselectRight();

	// Buttons highlight code
	if (buttonApply.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonApply.Select();
	else buttonApply.Deselect();

	if (buttonBack.GetButtonRect().contains(sf::Mouse::getPosition(data->window))) buttonBack.Select();
	else buttonBack.Deselect();
}

void GraphicsMenuState::Draw(float dt)
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
		selectionDisplayMode.Draw();
		selectionResolution.Draw();
		selectionVsync.Draw();
		buttonApply.Draw();
		buttonBack.Draw();

		// display frame
		data->window.display();
	}
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
