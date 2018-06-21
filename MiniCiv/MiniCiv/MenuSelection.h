#pragma once

#include "Game.h"

class MenuSelection {
public:
	MenuSelection(GameDataRef& data);
	// Please note that defaultID starts from 0. Not 1.
	MenuSelection(std::string labelText, sf::Vector2i labelPos, std::vector<std::string> optionsText, sf::Vector2i optionsPos, sf::Vector2i size, GameDataRef& data, int defaultID, bool loopingAllowed = true, bool available = true);
	MenuSelection& operator=(const MenuSelection& other);

	sf::IntRect GetLeftRect() const;
	sf::IntRect GetRightRect() const;

	void SelectLeft();
	void SelectRight();
	void DeselectLeft();
	void DeselectRight();

	bool IsSelectedLeft() const;
	bool IsSelectedRight() const;

	void MoveLeft();
	void MoveRight();

	void Draw();
private:
	GameDataRef & data;

	sf::Font font;
	int fontSize;

	// Label
	sf::IntRect labelRect;
	sf::RectangleShape labelShape;
	sf::Text labelText;

	// Options
	sf::IntRect leftRect;
	sf::IntRect rightRect;
	sf::RectangleShape leftShape;
	sf::RectangleShape rightShape;
	// Arrows are acutally rendered as '<' & '>' text characters
	sf::Text leftArrow;
	sf::Text rightArrow;

	sf::IntRect optionRect;
	sf::RectangleShape optionShape; 
	std::vector<sf::Text> optionText;

	// which option is currently selected
	int currentSelectionID;

	int numOptions;

	bool available;
	bool looping;
	bool selectedLeft = false;
	bool selectedRight = false;

	// constant button colours
	const sf::Color textColorSelect = sf::Color(12, 12, 12);
	const sf::Color rectColorSelect = sf::Color(232, 232, 232);
	const sf::Color textColorBase = sf::Color(232, 232, 232);
	const sf::Color rectColorBase = sf::Color(12, 12, 12);
	const sf::Color textColorDisable = sf::Color(128, 128, 128);
	const sf::Color rectColorDisable = sf::Color(30, 30, 30);
};