#pragma once
#include <fstream>
#include <string>

class Config
{
public:
	enum class Option
	{
		MapSize,
		MapWidth,
		MapHeight,
		MapWrapping
	};
public:
	Config(const std::string& filename);
	int Get(Option option) const;
private:
	int mapSize = 0;
	int mapWidth = 0;
	int mapHeight = 0;
	int mapWrapping = 1;
};