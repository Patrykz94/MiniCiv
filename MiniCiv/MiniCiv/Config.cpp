#include "Config.h"
#include <algorithm>

Config::Config(const std::string& filename)
{
	std::ifstream config(filename);

	if (config)
	{
		for (std::string line; std::getline(config, line);)
		{
			if (line == "[Map Size]" || line == "[map size]" || line == "[MAP SIZE]")
			{
				std::string tmp;
				config >> tmp;
				if (tmp == "small"|| tmp == "Small" || tmp == "SMALL")
				{
					mapSize = 0;
				}
				else if (tmp == "normal" || tmp == "Normal" || tmp == "NORMAL"
						|| tmp == "medium" || tmp == "Medium" || tmp == "MEDIUM")
				{
					mapSize = 1;
				}
				else if (tmp == "large" || tmp == "Large" || tmp == "LARGE")
				{
					mapSize = 2;
				}
				else if (tmp == "giant" || tmp == "Giant" || tmp == "GIANT")
				{
					mapSize = 3;
				}
			}
			else if (line == "[Map Width]" || line == "[map width]" || line == "[MAP WIDTH]")
			{
				int tmp;
				config >> tmp;
				if (tmp != 0)
				{
					mapWidth = std::max(std::min(tmp, 480), 24);
				}
			}
			else if (line == "[Map Height]" || line == "[map height]" || line == "[MAP HEIGHT]")
			{
				int tmp;
				config >> tmp;
				if (tmp != 0)
				{
					mapHeight = std::max(std::min(tmp, 240), 12);
				}
			}
			else if (line == "[Map Wrapping]" || line == "[map wrapping]" || line == "[MAP WRAPPING]")
			{
				std::string tmp;
				config >> tmp;
				if (tmp == "TRUE" || tmp == "True" || tmp == "true" || tmp == "1")
				{
					mapWrapping = 1;
				}
				else if (tmp == "FALSE" || tmp == "False" || tmp == "false" || tmp == "0")
				{
					mapWrapping = 0;
				}
			}
			else if (line == "[VSync]" || line == "[Vsync]" || line == "[vsync]" || line == "[VSYNC]")
			{
				std::string tmp;
				config >> tmp;
				if (tmp == "TRUE" || tmp == "True" || tmp == "true" || tmp == "1")
				{
					VSync = 1;
				}
				else if (tmp == "FALSE" || tmp == "False" || tmp == "false" || tmp == "0")
				{
					VSync = 0;
				}
			}
			else if (line == "[Show FPS]" || line == "[show FPS]" || line == "[show fps]" || line == "[SHOW FPS]")
			{
				std::string tmp;
				config >> tmp;
				if (tmp == "TRUE" || tmp == "True" || tmp == "true" || tmp == "1")
				{
					debugFPS = 1;
				}
				else if (tmp == "FALSE" || tmp == "False" || tmp == "false" || tmp == "0")
				{
					debugFPS = 0;
				}
			}
			else if (line == "[Show GridCoordinates]" || line == "[show gridcoordinates]" || line == "[SHOW GRIDCOORDINATES]")
			{
				std::string tmp;
				config >> tmp;
				if (tmp == "TRUE" || tmp == "True" || tmp == "true" || tmp == "1")
				{
					debugGridCoords = 1;
				}
				else if (tmp == "FALSE" || tmp == "False" || tmp == "false" || tmp == "0")
				{
					debugGridCoords = 0;
				}
			}
		}
	}
}

int Config::Get(Option option) const
{
	switch (option)
	{
	case Option::MapSize:
		return mapSize;
	case Option::MapWidth:
		return mapWidth;
	case Option::MapHeight:
		return mapHeight;
	case Option::MapWrapping:
		return mapWrapping;
	case Option::Vsync:
		return VSync;
	case Option::DebugFPS:
		return debugFPS;
	case Option::DebugGridCoordinates:
		return debugGridCoords;
	default:
		return 0;
	}
}
