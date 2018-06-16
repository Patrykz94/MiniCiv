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
		// Switch to main menu
		data->machine.AddState(StateRef(new MainMenuState(data)), true);
	}
}

void SplashState::Draw(float dt)
{
	data->window.clear(sf::Color::Color(38, 38, 38));

	data->window.draw(background);

	data->window.display();
}
