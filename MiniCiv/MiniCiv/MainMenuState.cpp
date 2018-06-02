#include "MainMenuState.h"

#include <sstream>
#include <iostream>

MainMenuState::MainMenuState(GameDataRef data)
	:
	_data(data)
{}

void MainMenuState::Init()
{
	// Load all images and textures and fonts
	this->_data->assets.LoadTexture("Main Menu Button", MAIN_MENU_BUTTON_PATH);
	this->_data->assets.LoadFont("Helvetica Font", FONT_HELVETICA_PATH);

	this->_title.setTexture(this->_data->assets.GetTexture("Splash State Background"));
	this->_generateMapButton.setTexture(this->_data->assets.GetTexture("Main Menu Button"));
	this->_optionsButton.setTexture(this->_data->assets.GetTexture("Main Menu Button"));
	this->_exitButton.setTexture(this->_data->assets.GetTexture("Main Menu Button"));
	this->_menuFont = this->_data->assets.GetFont("Helvetica Font");

	// Set correct dimensions and positions of images
	float titleScaleFactor = (this->_data->window.getSize().x / 2) / this->_title.getGlobalBounds().width;
	this->_title.setScale(titleScaleFactor, titleScaleFactor);
	this->_title.setPosition((float)this->_data->window.getSize().x / 2.0f - this->_title.getGlobalBounds().width / 2.0f, this->_title.getGlobalBounds().height * 0.1f);
	
	// Set positions for buttons and add text

	sf::Vector2f buttonSize = sf::Vector2f(this->_generateMapButton.getGlobalBounds().width, this->_generateMapButton.getGlobalBounds().height);
	sf::Vector2f buttonPos = sf::Vector2f((float)this->_data->window.getSize().x / 2.0f - buttonSize.x / 2.0f, (float)this->_data->window.getSize().y / 2.0f - buttonSize.y);

	// Generate Map Button
	this->_generateMapButton.setPosition(buttonPos);
	this->_generateMapButtonText = sf::Text("Generate Map", _menuFont, 78);
	this->_generateMapButtonText.setFillColor(sf::Color::Color(220, 220, 220));
	this->_generateMapButtonText.setPosition(_generateMapButton.getPosition() + sf::Vector2f(20, 0));

	// Options Button
	this->_optionsButton.setPosition(buttonPos.x, buttonPos.y + buttonSize.y * 1.2f);
	this->_optionsButtonText = sf::Text("Options", _menuFont, 78);
	this->_optionsButtonText.setFillColor(sf::Color::Color(220, 220, 220));
	this->_optionsButtonText.setPosition(_optionsButton.getPosition() + sf::Vector2f(20, 0));

	// Exit Button
	this->_exitButton.setPosition(buttonPos.x, buttonPos.y + buttonSize.y * 2.4f);
	this->_exitButtonText = sf::Text("Exit", _menuFont, 78);
	this->_exitButtonText.setFillColor(sf::Color::Color(220, 220, 220));
	this->_exitButtonText.setPosition(_exitButton.getPosition() + sf::Vector2f(20, 0));
}

void MainMenuState::HandleInput()
{
	sf::Event event;

	while (this->_data->window.pollEvent(event))
	{
		if (sf::Event::Closed == event.type)
		{
			this->_data->window.close();
		}

		if (this->_data->input.IsSpriteClicked(this->_generateMapButton, sf::Mouse::Left, this->_data->window))
		{
			std::cout << "Go to Game Screen" << std::endl;
		}

		if (this->_data->input.IsSpriteClicked(this->_optionsButton, sf::Mouse::Left, this->_data->window))
		{
			std::cout << "Go to Options Menu" << std::endl;
		}

		if (this->_data->input.IsSpriteClicked(this->_exitButton, sf::Mouse::Left, this->_data->window))
		{
			this->_data->window.close();
		}
	}
}

void MainMenuState::Update(float dt)
{
}

void MainMenuState::Draw(float dt)
{
	this->_data->window.clear(_backgroundColor);

	this->_data->window.draw(this->_title);

	this->_data->window.draw(this->_generateMapButton);
	this->_data->window.draw(this->_generateMapButtonText);

	this->_data->window.draw(this->_optionsButton);
	this->_data->window.draw(this->_optionsButtonText);

	this->_data->window.draw(this->_exitButton);
	this->_data->window.draw(this->_exitButtonText);
	
	this->_data->window.display();
}
