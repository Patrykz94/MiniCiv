#include "MenuOption.h"

MenuOption::MenuOption(GameDataRef& dataIn)
	:
	data(dataIn)
{}

MenuOption::MenuOption(Type typeIn, std::string text, GameDataRef& dataIn, bool available)
	:
	data(dataIn),
	type(typeIn),
	available(available)
{
	menuFont = data->assets.GetFont("menuFont");
	optionText = sf::Text(text, menuFont, 80);
	if (available)
		optionText.setFillColor(baseColor);
	else
		optionText.setFillColor(unavailableColor);
}

MenuOption::MenuOption(Type typeIn, std::string text, std::vector<std::string> optionsIn, GameDataRef & dataIn, bool available)
	:
	data(dataIn),
	type(typeIn),
	available(available)
{
	menuFont = data->assets.GetFont("menuFont");
	optionText = { text, menuFont, 80 };
	for (std::string& option : optionsIn)
	{
		options.emplace_back(option, data, menuFont, baseColor, unavailableColor, available);
	}
	options[0].Select();

	nOptions = optionsIn.size();
	if (available)
		optionText.setFillColor(baseColor);
	else
		optionText.setFillColor(unavailableColor);
}

bool MenuOption::IsSelected()
{
	if (available)
	{
		if (type == Type::Button)
		{
			return selected;
		}
	}
	return false;
}

void MenuOption::AddOptions(std::vector<std::string> optionsIn)
{
	for (std::string& option : optionsIn)
	{
		options.emplace_back(option, data, menuFont, baseColor, unavailableColor, available);
	}
	options[0].Select();

	nOptions = optionsIn.size();
}

void MenuOption::SetPosition(float x, float y, float x2, float y2)
{
	optionText.setPosition(x, y);
	if (type == Type::Selection && options.size() > 0)
	{
		for (Option& o : options)
		{
			o.SetPosition(x2, y2);
		}
	}
}

void MenuOption::SetFontSize(int size)
{
	optionText.setCharacterSize(size);
	if (type == Type::Selection && options.size() > 0)
	{
		for (Option& o : options)
		{
			o.SetFontSize(size);
		}
	}
}

void MenuOption::Draw()
{
	// set color and check if highlighetd
	if (available)
	{
		if (type == Type::Button)
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
	}

	// draw option
	data->window.draw(optionText);
	if (type == Type::Selection && options.size() > 0)
	{
		for (Option& o : options)
		{
			o.Draw();
		}
	}
}

MenuOption::Option::Option(std::string optionTextIn, GameDataRef& dataIn, sf::Font & fontIn, sf::Color & baseColor, sf::Color & unavailableColor, bool available)
	:
	data(dataIn),
	font(fontIn)
{
	optionText = sf::Text(optionTextIn, font, 80);
	if (available)
		optionText.setFillColor(baseColor);
	else
		optionText.setFillColor(unavailableColor);
}

void MenuOption::Option::Select()
{
	selected = true;
}

void MenuOption::Option::Deselect()
{
	selected = false;
}

void MenuOption::Option::SetPosition(float x, float y)
{
	optionText.setPosition(x - optionText.getGlobalBounds().width, y);
}

void MenuOption::Option::SetFontSize(int size)
{
	optionText.setCharacterSize(size);
}

void MenuOption::Option::Draw() const
{
	if (selected) {
		data->window.draw(optionText);
	}
}
