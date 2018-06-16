#include "AssetManager.h"

void AssetManager::LoadTexture(std::string name, std::string fileName)
{
	sf::Texture tex;

	if (tex.loadFromFile(fileName))
	{
		textures[name] = tex;
	}
}

sf::Texture & AssetManager::GetTexture(std::string name)
{
	return textures.at(name);
}

void AssetManager::LoadFont(std::string name, std::string fileName)
{
	sf::Font font;

	if (font.loadFromFile(fileName))
	{
		fonts[name] = font;
	}
}

sf::Font & AssetManager::GetFont(std::string name)
{
	return fonts.at(name);
}
