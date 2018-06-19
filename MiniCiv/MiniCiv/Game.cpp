#include "Game.h"
#include "SplashState.h"

Game::Game(std::string title)
{
	// get desktop resolution
	sf::VideoMode desktop = sf::VideoMode::getDesktopMode();

	data->window.create(sf::VideoMode(desktop.width/3, desktop.height/3, desktop.bitsPerPixel), title, sf::Style::None);

	data->machine.AddState(StateRef(new SplashState(data)));

	Run();
}

void Game::Run()
{
	float newTime, frameTime;// , interpolation;

	float currentTime = clock.getElapsedTime().asSeconds();
	float accumulator = 0.0f;

	while (data->window.isOpen())
	{
		data->machine.ProcessStateChanges();
		
		newTime = clock.getElapsedTime().asSeconds();
		frameTime = newTime - currentTime;

		/* if (frameTime > 0.25f)
		{
			frameTime = 0.25f;
		} */

		currentTime = newTime;
		//accumulator += frameTime;

		//while (accumulator >= dt)
		//{
			data->machine.GetActiveState()->HandleInput();
			data->machine.GetActiveState()->Update(frameTime);

		//	accumulator -= dt;
		//}

		//interpolation = accumulator / dt;
		data->machine.GetActiveState()->Draw(frameTime);
	}
}
