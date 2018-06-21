#pragma once

#include "Game.h"

class MenuButton {
public:
	MenuButton(GameDataRef& data);
	MenuButton(std::string buttonText, sf::Vector2i buttonPos, sf::Vector2i buttonSize, GameDataRef& data, bool available = true);
	MenuButton& operator=(const MenuButton& other);

	sf::IntRect GetButtonRect() const;
	void Select();
	void Deselect();

	bool IsSelected() const;
	
	void Draw();
private:
	GameDataRef& data;

	sf::Font font;
	int fontSize;

	sf::IntRect buttonRect;
	sf::RectangleShape buttonShape;
	sf::Text buttonText;

	bool available;
	bool selected = false;

	// constant button colours
	const sf::Color textColorSelect = sf::Color(12, 12, 12);
	const sf::Color rectColorSelect = sf::Color(232, 232, 232);
	const sf::Color textColorBase = sf::Color(232, 232, 232);
	const sf::Color rectColorBase = sf::Color(12, 12, 12);
	const sf::Color textColorDisable = sf::Color(128, 128, 128);
	const sf::Color rectColorDisable = sf::Color(30, 30, 30);
};