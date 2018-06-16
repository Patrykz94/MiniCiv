#pragma once

#include "Game.h"

class MenuOption {
public:
	enum class Type {
		Button,
		YesNoButton,
		RadioButton,
		TextField,
		DropDownList,
	};
public:
	MenuOption() = default;
	MenuOption(Type typeIn, std::string Text, GameDataRef& dataIn, bool available = true);
	MenuOption(const MenuOption& in);

	sf::Text GetOptionText() const;

	bool IsSelected();

	void SetPosition(float x, float y);
	void SetPosition(sf::Vector2f pos)
	{
		SetPosition(pos.x, pos.y);
	}
	void Draw();
private:
	// get _data structure
	GameDataRef & data;

	Type type;

	sf::Font menuFont;
	static constexpr unsigned int fontSize = 80;
	sf::Text optionText;
	std::vector<std::string> options;

	bool available;
	bool selected = false;

	sf::Color baseColor = sf::Color::White;
	sf::Color selectedColor = sf::Color::Red;
	sf::Color unavailableColor = { 128, 128, 128 };
};