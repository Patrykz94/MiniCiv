#include "SplashState.h"

#include <sstream>
#include <iostream>

#include "MainMenuState.h"

SplashState::SplashState(GameDataRef data)
	:
	_data(data)
{}

void SplashState::Init()
{
	this->_data->assets.LoadTexture("Splash State Background", SPLASH_SCENE_BACKGROUD_FILEPATH);

	this->_background.setTexture(this->_data->assets.GetTexture("Splash State Background"));
	this->_background.setPosition(((sf::Vector2f)this->_data->window.getSize() - sf::Vector2f(this->_background.getGlobalBounds().width, this->_background.getGlobalBounds().height)) / 2.0f);
}

void SplashState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type)
		{
			this->_data->window.close();
		}
	}
}

void SplashState::Update(float dt)
{
	if (this->_clock.getElapsedTime().asSeconds() > SPLASH_STATE_SHOW_TIME)
	{
		// Switch to main menu
		this->_data->machine.AddState(StateRef(new MainMenuState(_data)), true);
	}
}

void SplashState::Draw(float dt)
{
	this->_data->window.clear(sf::Color::Color(38, 38, 38));

	this->_data->window.draw(this->_background);

	this->_data->window.display();
}
