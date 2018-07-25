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
	// Load all images and textures and fonts
	data->assets.LoadTexture("splashStateBackground", SPLASH_SCENE_BACKGROUD_FILEPATH);
	data->assets.LoadFont(MENU_FONT_NAME, FONT_HELVETICA_PATH);

	background.setTexture(data->assets.GetTexture("splashStateBackground"));

	// Rescale splash screen background
	float scaleFactorX = data->window.getSize().x / background.getGlobalBounds().width;
	float scaleFactorY = data->window.getSize().y / background.getGlobalBounds().height;
	float scaleFactor = std::min(scaleFactorX, scaleFactorY);
	background.setScale(scaleFactor, scaleFactor);
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
		// create window
		sf::Vector2i res = data->settings.GetScreenResolution();
		data->window.create(sf::VideoMode(res.x, res.y, sf::VideoMode::getDesktopMode().bitsPerPixel), GAME_TITLE, data->settings.GetScreenMode());
		data->window.setVerticalSyncEnabled(data->settings.GetScreenVsync());

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
