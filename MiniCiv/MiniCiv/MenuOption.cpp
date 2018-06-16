#include "MenuOption.h"

MenuOption::MenuOption(MenuOption::Type typeIn, std::string text, GameDataRef& dataIn, bool available)
	:
	data(dataIn),
	type(typeIn),
	available(available)
{
	menuFont = data->assets.GetFont("menuFont");
	optionText = sf::Text(text, menuFont, fontSize);
}

MenuOption::MenuOption(const MenuOption & in)
	:
	data(in.data),
	type(in.type)
{
	menuFont = data->assets.GetFont("menuFont");
	optionText = in.optionText;
	available = in.available;
}

sf::Text MenuOption::GetOptionText() const
{
	return optionText;
}

bool MenuOption::IsSelected()
{
	if (available)
	{
		return selected;
	}
	return false;
}

void MenuOption::SetPosition(float x, float y)
{
	optionText.setPosition(x, y);
}

void MenuOption::Draw()
{
	// set color and check if highlighetd
	if (available)
	{
		// is current option selected?
		if (optionText.getGlobalBounds().contains((sf::Vector2f)sf::Mouse::getPosition(data->window)))
		{
			selected = true;
			optionText.setFillColor(selectedColor);
		}
		else
		{
			selected = false;
			optionText.setFillColor(baseColor);
		}
	}
	else
	{
		optionText.setFillColor(unavailableColor);
	}

	if (type == Type::Button) {
		// draw button
		data->window.draw(optionText);
	}
}