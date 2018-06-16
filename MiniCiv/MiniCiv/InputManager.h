#pragma once

#include <SFML/Graphics.hpp>

class InputManager
{
public:
	InputManager() {}
	~InputManager() {}

	bool IsObjectClicked(sf::Sprite object, sf::Mouse::Button button, sf::RenderWindow& window);
	bool IsObjectClicked(sf::Text object, sf::Mouse::Button button, sf::RenderWindow& window);

	sf::Vector2i GetMousePosition(sf::RenderWindow& window);
};