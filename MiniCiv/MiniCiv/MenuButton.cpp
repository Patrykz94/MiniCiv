#include "MenuButton.h"

#include <sstream>
#include <iostream>

MenuButton::MenuButton(GameDataRef & data)
	:
	data(data)
{}

MenuButton::MenuButton(std::string buttonTextIn, sf::Vector2i buttonPos, sf::Vector2i buttonSize, GameDataRef& data, bool available)
	:
	data(data),
	available(available)
{
	font = data->assets.GetFont(MENU_FONT_NAME);
	fontSize = (int)((float)buttonSize.y * 0.8f);

	buttonShape = sf::RectangleShape((sf::Vector2f)buttonSize);
	buttonShape.setPosition((sf::Vector2f)buttonPos);
	buttonShape.setFillColor(rectColorBase);

	buttonRect = sf::IntRect(buttonPos, buttonSize);
	buttonText = sf::Text(buttonTextIn, font, fontSize);
	sf::FloatRect textRect = buttonText.getGlobalBounds();
	buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	buttonText.setPosition(buttonShape.getPosition().x + buttonShape.getGlobalBounds().width / 2.0f,
		buttonShape.getPosition().y + buttonShape.getGlobalBounds().height / 2.0f);

	if (!available)
	{
		buttonText.setFillColor(textColorDisable);
		buttonShape.setFillColor(rectColorDisable);
	}
}

MenuButton & MenuButton::operator=(const MenuButton& other)
{
	available = other.available;

	font = data->assets.GetFont(MENU_FONT_NAME);
	fontSize = other.fontSize;

	buttonShape = other.buttonShape;
	buttonShape.setPosition(other.buttonShape.getPosition());
	buttonShape.setFillColor(rectColorBase);

	buttonRect = other.buttonRect;
	buttonText = sf::Text(other.buttonText.getString(), font, fontSize);
	sf::FloatRect textRect = buttonText.getGlobalBounds();
	buttonText.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	buttonText.setPosition(buttonShape.getPosition().x + buttonShape.getGlobalBounds().width / 2.0f,
		buttonShape.getPosition().y + buttonShape.getGlobalBounds().height / 2.0f);

	if (!available)
	{
		buttonText.setFillColor(textColorDisable);
		buttonShape.setFillColor(rectColorDisable);
	}

	return *this;
}

sf::IntRect MenuButton::GetButtonRect() const
{
	return buttonRect;
}

void MenuButton::Select()
{
	if (available)
	{
		selected = true;
		buttonText.setFillColor(textColorSelect);
		buttonShape.setOutlineColor(textColorSelect);
		buttonShape.setFillColor(rectColorSelect);
	}
}

void MenuButton::Deselect()
{
	if (available)
	{
		selected = false;
		buttonText.setFillColor(textColorBase);
		buttonShape.setOutlineColor(textColorBase);
		buttonShape.setFillColor(rectColorBase);
	}
}

bool MenuButton::IsSelected() const
{
	return selected;
}

void MenuButton::Enable()
{
	available = true;
	buttonText.setFillColor(textColorBase);
	buttonShape.setFillColor(rectColorBase);
}

void MenuButton::Disable()
{
	Deselect();
	available = false;
	buttonText.setFillColor(textColorDisable);
	buttonShape.setFillColor(rectColorDisable);
}

void MenuButton::Draw()
{
	data->window.draw(buttonShape);
	data->window.draw(buttonText);
}
