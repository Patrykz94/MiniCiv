#include "Game.h"
#include <iostream>

int main()
{
	bool VsyncState = true;
	// Display the list of all the video modes available for fullscreen
	std::vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
	std::cout << "Available Fullscreen resolution modes are:" << std::endl;
	for (std::size_t i = 0; i < modes.size(); ++i)
	{
		sf::VideoMode mode = modes[i];
		std::cout << "Mode #" << i << ": "
			<< mode.width << "x" << mode.height << " - "
			<< mode.bitsPerPixel << " bpp" << std::endl;
	}

	// Create the main window
	sf::RenderWindow window(sf::VideoMode(modes[0].width, modes[0].height, modes[0].bitsPerPixel), "MiniCiv", sf::Style::Fullscreen);
	window.setVerticalSyncEnabled(VsyncState);
	
	bool isFullScreen = true;

	sf::Clock clock;

	// Start the game loop
	Game game(window, clock, VsyncState);

	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			switch (event.type)
			{
				// Toggle fullscreen
			case sf::Event::KeyReleased:
				switch (event.key.code)
				{
				case sf::Keyboard::Return:
					if (isFullScreen == true)
					{
						window.create(sf::VideoMode(800, 600), "MiniCiv", sf::Style::Close);
						isFullScreen = false;
						window.setVerticalSyncEnabled(VsyncState);
					}
					else
					{
						window.create(sf::VideoMode(modes[0].width, modes[0].height, modes[0].bitsPerPixel), "MiniCiv", sf::Style::Fullscreen);
						isFullScreen = true;
						window.setVerticalSyncEnabled(VsyncState);
					}
					break;
				}

				break;
				// Close window: exit
			case sf::Event::Closed:
				window.close();
				break;
			}
		}

		game.Go();
	}
	return EXIT_SUCCESS;
}