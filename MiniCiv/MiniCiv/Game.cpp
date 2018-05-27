#include "Game.h"

Game::Game(sf::RenderWindow & window, sf::Clock& clock, bool& VSyncState)
	:
	window(window),
	clock(clock),
	VSync(VSyncState)
{
	font.loadFromFile("Data\\Fonts\\arial.ttf");
	if (config.Get(Config::Option::DebugFPS) == 1)
	{
		drawFps = true;
	}
	else
	{
		drawFps = false;
	}
	// Check if VSync should be enabled
	if (config.Get(Config::Option::Vsync) == 1)
	{
		VSync = true;
	}
	else
	{
		VSync = false;
	}
	// update VSync setting
	window.setVerticalSyncEnabled(VSync);
}

void Game::Go()
{
	// Clear screen
	window.clear();

	// Time measuring
	sf::Time elapsed = clock.restart();
	float dt = elapsed.asSeconds();

	// Process game logics
	UpdateModel(dt);

	// Create Graphics
	ComposeFrame();

	// Update the window
	window.display();
}

void Game::UpdateModel(float dt)
{
	// update the debug object
	debug.Update(dt);

	// process map scroll
	if (sf::Mouse::isButtonPressed(sf::Mouse::Left) && window.hasFocus())
	{
		sf::Vector2i mousePos = sf::Mouse::getPosition();
		if (LPressTime > 0.15f || (LPressTime > 0.0f && lastMousePressAt != mousePos))
		{
			world.MoveView(lastMousePressAt - mousePos);
		}
		LPressTime += dt;
		lastMousePressAt = mousePos;
	}
	else
	{
		// if mouse released, reset time back to 0
		LPressTime = 0;
	}
}

void Game::ComposeFrame()
{
	// Displaying graphics goes here
	world.Draw();
	if (drawFps)
	{
		debug.DrawFPS(window);
	}
}