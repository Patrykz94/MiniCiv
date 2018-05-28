#include "SplashState.h"

#include <sstream>
#include <iostream>

SplashState::SplashState(GameDataRef data)
	:
	_data(data)
{}

void SplashState::Init()
{
	this->_data->assets.LoadTexture("Splash State Background", SPLASH_SCENE_BACKGROUD_FILEPATH);
	_background.setTexture(this->_data->assets.GetTexture("Splash State Background"));
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
		std::cout << "Go to Main Menu" << std::endl;
	}
}

void SplashState::Draw(float dt)
{
	sf::Vector2f backgroundSize = sf::Vector2f(this->_background.getGlobalBounds().width, this->_background.getGlobalBounds().height);
	this->_background.setPosition(((sf::Vector2f)this->_data->window.getSize() - backgroundSize) / 2.0f);

	this->_data->window.clear(sf::Color::Color(38, 38, 38));

	this->_data->window.draw(this->_background);

	this->_data->window.display();
}
