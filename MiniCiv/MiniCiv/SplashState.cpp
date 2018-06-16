#include "SplashState.h"

#include <sstream>
#include <iostream>

#include "MainMenuState.h"

SplashState::SplashState(GameDataRef dataIn)
	:
	data(dataIn)
{}

void SplashState::Init()
{
	data->assets.LoadTexture("splashStateBackground", SPLASH_SCENE_BACKGROUD_FILEPATH);
	// Load all images and textures and fonts
	data->assets.LoadTexture("Main Menu Button", MAIN_MENU_BUTTON_PATH);
	data->assets.LoadFont("menuFont", FONT_HELVETICA_PATH);

	background.setTexture(data->assets.GetTexture("splashStateBackground"));
	background.setPosition(((sf::Vector2f)data->window.getSize() - sf::Vector2f(background.getGlobalBounds().width, background.getGlobalBounds().height)) / 2.0f);
}

void SplashState::HandleInput()
{
	sf::Event event;

	while (data->window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type)
		{
			data->window.close();
		}
	}
}

void SplashState::Update(float dt)
{
	if (clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME)
	{
		// Create game window with given resolution
		std::cout << "Create game window" << std::endl;
		sf::VideoMode mode = sf::VideoMode::getDesktopMode();
		// default values
		int scrnWidth = (int)((float)mode.width * 0.75f);
		int scrnHeight = (int)((float)mode.height * 0.75f);
		// create window
		data->window.create(sf::VideoMode(scrnWidth, scrnHeight, mode.bitsPerPixel), GAME_TITLE, sf::Style::Close);

		// Switch to main menu
		std::cout << "Load main menu" << std::endl;
		data->machine.AddState(StateRef(new MainMenuState(data)), true);
	}
}

void SplashState::Draw(float dt)
{
	data->window.clear(sf::Color::Color(38, 38, 38));

	data->window.draw(background);

	data->window.display();
}
