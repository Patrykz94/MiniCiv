#pragma once

#include "Game.h"

class MenuOption {
public:
	enum class Type {
		Button,
		Selection
	};
public:
	MenuOption(GameDataRef& data);
	MenuOption(Type typeIn, std::string text, GameDataRef& dataIn, bool available = true);
	MenuOption(Type typeIn, std::string text, std::vector<std::string> options, GameDataRef& dataIn, bool available = true);
	
	bool IsSelected();

	void AddOptions(std::vector<std::string> options);

	void SetPosition(float x, float y, float x2 = 0.0f, float y2 = 0.0f);
	void SetPosition(sf::Vector2f pos, sf::Vector2f pos2 = {0.0f, 0.0f})
	{
		SetPosition(pos.x, pos.y, pos2.x, pos2.y);
	}
	void SetFontSize(int size);
	
	void Draw();
private:
	class Option {
	public:
		Option() = default;
		Option(std::string optionText, GameDataRef& data, sf::Font & font, sf::Color& baseColor, sf::Color& unavailableColor, bool available);
		
		void Select();
		void Deselect();

		void SetPosition(float x, float y);
		void SetFontSize(int size);

		void Draw() const;
	private:
		GameDataRef & data;

		sf::Font& font;
		sf::Text optionText;
		bool selected = false;
	};
private:
	// get data structure
	GameDataRef & data;

	float width = (float)data->window.getSize().x * 0.43f;

	Type type;

	sf::Font menuFont;
	sf::IntRect optionRect;
	sf::Text optionText;
	std::vector<Option> options;

	int nOptions = 0;

	bool available;
	bool selected = false;

	sf::Color baseColor = sf::Color::White;
	sf::Color selectedColor = sf::Color::Red;
	sf::Color unavailableColor = { 128, 128, 128 };
};