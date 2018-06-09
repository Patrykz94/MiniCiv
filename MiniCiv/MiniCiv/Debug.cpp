#include "Debug.h"

Debug::Debug()
{
	font.loadFromFile(FONT_ARIAL_PATH);
}

void Debug::Update(float dt)
{
	playTime += dt;
	fpsTimeCounter += dt;
	fpsFramesCounter++;

	if (fpsTimeCounter > fpsRefreshTime)
	{
		float avgFps = fpsTimeCounter / fpsFramesCounter;
		framesPerSecond = (int)(1 / avgFps + 0.5f);
		fpsTimeCounter = 0.0f;
		fpsFramesCounter = 0;
	}
}

void Debug::DrawFPS(sf::RenderWindow& window) const
{
	sf::Text text = sf::Text(std::to_string(framesPerSecond), font, 30);
	text.setStyle(sf::Text::Bold);
	text.setFillColor(sf::Color::Color(0, 150, 0));
	text.setPosition((sf::Vector2f)window.getSize() - sf::Vector2f(75.0f, 40.0f));
	window.draw(text);
}

void Debug::DrawGridCoordinates(const sf::Vector2i& gridPos, const sf::Vector2i& scrnPos, sf::RenderWindow& window) const
{
	sf::Text text = sf::Text(std::to_string(gridPos.x) + "\n" + std::to_string(gridPos.y), font, 10);
	text.setFillColor(sf::Color::Red);
	text.setPosition((sf::Vector2f)scrnPos);
	window.draw(text);
}
