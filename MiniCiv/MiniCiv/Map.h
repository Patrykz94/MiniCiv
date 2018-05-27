#pragma once

#include <SFML/Graphics.hpp>
#include <algorithm>
#include <vector>
#include "Debug.h"
#include "Config.h"

class Map
{
private:
	class Tile
	{
	public:
		enum class ElevationType
		{
			Ocean,
			Shore,
			Flat,
			Hill,
			Mountain,
			Count
		};
	public:
		Tile(Map& map, char c, const sf::Vector2i& gridPos, const sf::Texture& tileSprite);
		Tile(Map& map, ElevationType elevationType, const sf::Vector2i& gridPos, const sf::Texture& tileSprite);
		void Draw(const sf::Vector2i& screenPos, sf::RenderWindow& window) const;
		void SetElevation(float elevation);
		float GetElevation() const;
		ElevationType GetElevationType() const;
		sf::Vector2i GetPosition() const;
		float DistanceToTile(const sf::Vector2i& gridPos) const;
	private:
		sf::IntRect MapTileRect() const;
	private:
		Map & map;
		sf::Vector2i position;
		const sf::Texture& tileSprite;
		ElevationType elevationType;
		float elevation = -1.0f;
	};
public:
	enum class Type
	{
		Empty,
		Continents,
		Count
	};
public:
	Map(sf::RenderWindow& window, Debug& debug, const Config& config);
	bool WrappingEastWest() const;
	sf::Vector2i Size() const;
	void Draw() const;
	void MoveView(const sf::Vector2i& deltaPos);
	void SetDrawGridCoordsTo(bool option);
private:
	// allows selecting the tile by supplying it's position on the grid
	Tile & TileAt(sf::Vector2i gridPos);
	const Tile& TileAt(const sf::Vector2i& gridPos) const;
	sf::Vector2i GridToScreen(const sf::Vector2i& gridPos) const;
	sf::Vector2i ScreenToGrid(const sf::Vector2i& scrnPos) const;
	sf::Vector2i GridAtIndex(int index) const;
	// returns a vector of grid coordinates around the tile provided
	std::vector<sf::Vector2i> GetTilesAround(const sf::Vector2i& gridPos, float distance) const;
	void GenerateMap(Type type = Type::Empty);
	// elevate an area passed into this function
	void ElevateArea(const std::vector<sf::Vector2i>& area);
public:
	// is drawing grid coordinates required?
	bool drawGridCoords = true;
private:
	sf::RenderWindow& window;
	// debug class
	Debug& debug;
	// sprite containing all terrain tile graphics
	sf::Texture tileSprite;
	// size of each tile
	static constexpr int tileSize = 60;
	// dimensions of the map in number of tiles
	int numColumns;
	int numRows;
	// dimensions of the map in pixels
	int mapWidth = numColumns * tileSize;
	int mapHeight = numRows * tileSize;
	// is cylindrical wrapping required?
	bool allowWrappingEastWest = true;
	// offset of the map
	sf::Vector2i mapPos = sf::Vector2i(mapWidth, mapHeight) / -2 + (sf::Vector2i)window.getSize()/2;
	// create the tilemap
	std::vector<Tile> tiles;
};