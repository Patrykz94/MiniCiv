#include "MenuSelection.h"

#include <sstream>
#include <iostream>

MenuSelection::MenuSelection(GameDataRef & data)
	:
	data(data)
{}

MenuSelection::MenuSelection(std::string labelTextIn, sf::Vector2i labelPos, std::vector<std::string> optionsTextIn, sf::Vector2i optionsPos, sf::Vector2i size, GameDataRef& dataIn, int defaultID, bool loopingAllowed, bool available)
	:
	data(dataIn),
	looping(loopingAllowed),
	available(available)
{
	font = data->assets.GetFont(MENU_FONT_NAME);
	fontSize = (int)((float)size.y * 0.8f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Set up the label shape for this option
	labelShape = sf::RectangleShape((sf::Vector2f)size);
	labelShape.setPosition((sf::Vector2f)labelPos);
	labelShape.setFillColor(rectColorBase);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up rect and text as well as positions
	labelRect = sf::IntRect(labelPos, size);
	labelText = sf::Text(labelTextIn, font, fontSize);
	sf::FloatRect labelTextRect = labelText.getGlobalBounds();
	labelText.setOrigin(labelTextRect.left + labelTextRect.width / 2.0f, labelTextRect.top + labelTextRect.height / 2.0f);
	labelText.setPosition(labelShape.getPosition().x + labelShape.getGlobalBounds().width / 2.0f,
		labelShape.getPosition().y + labelShape.getGlobalBounds().height / 2.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Set up the left arrow for this option
	leftShape = sf::RectangleShape(sf::Vector2f((float)size.x / 10.0f, (float)size.y));
	leftShape.setPosition((sf::Vector2f)optionsPos);
	leftShape.setFillColor(rectColorBase);
	// Set up the right arrow for this option
	rightShape = sf::RectangleShape(sf::Vector2f((float)size.x / 10.0f, (float)size.y));
	rightShape.setPosition(sf::Vector2f((float)optionsPos.x + (float)size.x - rightShape.getGlobalBounds().width, (float)optionsPos.y));
	rightShape.setFillColor(rectColorBase);
	// Set up the option
	optionShape = sf::RectangleShape(sf::Vector2f((float)size.x - 20.0f - leftShape.getGlobalBounds().width - rightShape.getGlobalBounds().width, (float)size.y));
	optionShape.setPosition(sf::Vector2f((float)optionsPos.x + leftShape.getGlobalBounds().width + 10.0f, (float)optionsPos.y));
	optionShape.setFillColor(rectColorBase);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up left arrow rect, text and position
	leftRect = sf::IntRect(optionsPos, sf::Vector2i(size.x / 10, size.y));
	leftArrow = sf::Text("<", font, fontSize);
	sf::FloatRect leftArrowRect = leftArrow.getGlobalBounds();
	leftArrow.setOrigin(leftArrowRect.left + leftArrowRect.width / 2.0f, leftArrowRect.top + leftArrowRect.height / 2.0f);
	leftArrow.setPosition(leftShape.getPosition().x + leftShape.getGlobalBounds().width / 2.0f,
		leftShape.getPosition().y + leftShape.getGlobalBounds().height / 2.0f);
	// Set up left arrow rect, text and position
	rightRect = sf::IntRect(sf::Vector2i(optionsPos.x + size.x - (int)rightShape.getGlobalBounds().width, optionsPos.y), sf::Vector2i(size.x / 10, size.y));
	rightArrow = sf::Text(">", font, fontSize);
	sf::FloatRect rightArrowRect = rightArrow.getGlobalBounds();
	rightArrow.setOrigin(rightArrowRect.left + rightArrowRect.width / 2.0f, rightArrowRect.top + rightArrowRect.height / 2.0f);
	rightArrow.setPosition(rightShape.getPosition().x + rightShape.getGlobalBounds().width / 2.0f,
		rightShape.getPosition().y + rightShape.getGlobalBounds().height / 2.0f);
	// Set up option rect, text and position
	optionRect = sf::IntRect(sf::Vector2i(optionsPos.x + (int)leftShape.getGlobalBounds().width + 10, optionsPos.y), sf::Vector2i(size.x - 20 - (int)leftShape.getGlobalBounds().width - (int)rightShape.getGlobalBounds().width, size.y));
	for (std::string& option : optionsTextIn)
	{
		sf::Text optionTextTemp = sf::Text(option, font, fontSize);

		sf::FloatRect optionTextRect = optionTextTemp.getGlobalBounds();
		optionTextTemp.setOrigin(optionTextRect.left + optionTextRect.width / 2.0f, optionTextRect.top + optionTextRect.height / 2.0f);
		optionTextTemp.setPosition(optionShape.getPosition().x + optionShape.getGlobalBounds().width / 2.0f,
			optionShape.getPosition().y + optionShape.getGlobalBounds().height / 2.0f);

		optionText.emplace_back(optionTextTemp);
	}
	numOptions = optionText.size();

	currentSelectionID = defaultID;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (!available)
	{
		leftArrow.setFillColor(textColorDisable);
		rightArrow.setFillColor(textColorDisable);
		leftShape.setFillColor(rectColorDisable);
		rightShape.setFillColor(rectColorDisable);
	}
}

MenuSelection & MenuSelection::operator=(const MenuSelection& other)
{
	looping = other.looping;
	available = other.available;

	font = data->assets.GetFont(MENU_FONT_NAME);
	fontSize = other.fontSize;

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Set up the label shape for this option
	labelShape = other.labelShape;
	labelShape.setPosition(other.labelShape.getPosition());
	labelShape.setFillColor(rectColorBase);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up rect and text as well as positions
	labelRect = other.labelRect;
	labelText = sf::Text(other.labelText.getString(), font, fontSize);
	sf::FloatRect labelTextRect = labelText.getGlobalBounds();
	labelText.setOrigin(labelTextRect.left + labelTextRect.width / 2.0f, labelTextRect.top + labelTextRect.height / 2.0f);
	labelText.setPosition(labelShape.getPosition().x + labelShape.getGlobalBounds().width / 2.0f,
		labelShape.getPosition().y + labelShape.getGlobalBounds().height / 2.0f);

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Set up the left arrow for this option
	leftShape = other.leftShape;
	leftShape.setPosition(other.leftShape.getPosition());
	leftShape.setFillColor(rectColorBase);
	// Set up the right arrow for this option
	rightShape = other.rightShape;
	rightShape.setPosition(other.rightShape.getPosition());
	rightShape.setFillColor(rectColorBase);
	// Set up the option
	optionShape = other.optionShape;
	optionShape.setPosition(other.optionShape.getPosition());
	optionShape.setFillColor(rectColorBase);
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Set up left arrow rect, text and position
	leftRect = other.leftRect;
	leftArrow = sf::Text(other.leftArrow.getString(), font, fontSize);
	sf::FloatRect leftArrowRect = leftArrow.getGlobalBounds();
	leftArrow.setOrigin(leftArrowRect.left + leftArrowRect.width / 2.0f, leftArrowRect.top + leftArrowRect.height / 2.0f);
	leftArrow.setPosition(leftShape.getPosition().x + leftShape.getGlobalBounds().width / 2.0f,
		leftShape.getPosition().y + leftShape.getGlobalBounds().height / 2.0f);
	// Set up left arrow rect, text and position
	rightRect = other.rightRect;
	rightArrow = sf::Text(other.rightArrow.getString(), font, fontSize);
	sf::FloatRect rightArrowRect = rightArrow.getGlobalBounds();
	rightArrow.setOrigin(rightArrowRect.left + rightArrowRect.width / 2.0f, rightArrowRect.top + rightArrowRect.height / 2.0f);
	rightArrow.setPosition(rightShape.getPosition().x + rightShape.getGlobalBounds().width / 2.0f,
		rightShape.getPosition().y + rightShape.getGlobalBounds().height / 2.0f);
	// Set up option rect, text and position
	optionRect = other.optionRect;

	for (sf::Text option : other.optionText)
	{
		sf::Text optionTextTemp = sf::Text(option.getString(), font, fontSize);

		sf::FloatRect optionTextRect = optionTextTemp.getGlobalBounds();
		optionTextTemp.setOrigin(optionTextRect.left + optionTextRect.width / 2.0f, optionTextRect.top + optionTextRect.height / 2.0f);
		optionTextTemp.setPosition(optionShape.getPosition().x + optionShape.getGlobalBounds().width / 2.0f,
			optionShape.getPosition().y + optionShape.getGlobalBounds().height / 2.0f);

		optionText.emplace_back(optionTextTemp);
	}
	numOptions = optionText.size();

	currentSelectionID = other.currentSelectionID;

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	if (!available)
	{
		leftArrow.setFillColor(textColorDisable);
		rightArrow.setFillColor(textColorDisable);
		leftShape.setFillColor(rectColorDisable);
		rightShape.setFillColor(rectColorDisable);
	}

	return *this;
}

sf::IntRect MenuSelection::GetLeftRect() const
{
	return leftRect;
}

sf::IntRect MenuSelection::GetRightRect() const
{
	return rightRect;
}

void MenuSelection::SelectLeft()
{
	if (available)
	{
		if (looping || currentSelectionID > 0)
		{
			selectedLeft = true;
			leftArrow.setFillColor(textColorSelect);
			leftShape.setFillColor(rectColorSelect);
		}
		else
		{
			selectedLeft = false;
			leftArrow.setFillColor(textColorDisable);
			leftShape.setFillColor(rectColorDisable);
		}
	}
}

void MenuSelection::SelectRight()
{
	if (available)
	{
		if (looping || currentSelectionID < numOptions - 1)
		{
			selectedRight = true;
			rightArrow.setFillColor(textColorSelect);
			rightShape.setFillColor(rectColorSelect);
		}
		else
		{
			selectedRight = false;
			rightArrow.setFillColor(textColorDisable);
			rightShape.setFillColor(rectColorDisable);
		}
	}
}

void MenuSelection::DeselectLeft()
{
	if (available)
	{
		if (looping || currentSelectionID > 0)
		{
			selectedLeft = false;
			leftArrow.setFillColor(textColorBase);
			leftShape.setFillColor(rectColorBase);
		}
		else
		{
			selectedLeft = false;
			leftArrow.setFillColor(textColorDisable);
			leftShape.setFillColor(rectColorDisable);
		}
	}
}

void MenuSelection::DeselectRight()
{
	if (available)
	{
		if (looping || currentSelectionID < numOptions - 1)
		{
			selectedRight = false;
			rightArrow.setFillColor(textColorBase);
			rightShape.setFillColor(rectColorBase);
		}
		else
		{
			selectedRight = false;
			rightArrow.setFillColor(textColorDisable);
			rightShape.setFillColor(rectColorDisable);
		}
	}
}

bool MenuSelection::IsSelectedLeft() const
{
	return selectedLeft;
}

bool MenuSelection::IsSelectedRight() const
{
	return selectedRight;
}

void MenuSelection::MoveLeft()
{
	if (looping || currentSelectionID > 0)
	{
		currentSelectionID = (currentSelectionID + numOptions - 1) % numOptions;
	}
}

void MenuSelection::MoveRight()
{
	if (looping || currentSelectionID < numOptions - 1)
	{
		currentSelectionID = (currentSelectionID + 1) % numOptions;
	}
}

void MenuSelection::Draw()
{
	// Draw the labels frist
	data->window.draw(labelShape);
	data->window.draw(labelText);

	// Then draw arrow buttons
	data->window.draw(leftShape);
	data->window.draw(leftArrow);
	data->window.draw(rightShape);
	data->window.draw(rightArrow);

	// Now draw the currently selected option
	data->window.draw(optionShape);
	data->window.draw(optionText[currentSelectionID]);
}