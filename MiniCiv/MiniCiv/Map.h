#pragma once

#include <SFML/Graphics.hpp>

#include <iostream>

#include "Game.h"
#include "PerlinNoise.h"

class Map
{
protected:
	class Tile
	{
	public:
		enum class Type
		{
			Ocean,
			Shore,
			Desert,
			Plains,
			Grassland,
			Tundra,
			Snow,
			Mountain,
			Count
		};
		enum class Elevation
		{
			Water,
			Flat,
			Hill,
			Mountain,
			Count
		};
		enum class Vegetation
		{
			None,
			Grass,
			Forest,
			Jungle,
			Count
		};
	public:
		Tile(const Map& map, int column, int row);

		sf::Vector2i Position() const;
		sf::Vector2i PositionFromView(sf::View& mapView, int mapWidth, bool allowWrappingEastWest) const;
		void Draw(sf::Vector2i scrnPos, sf::RenderWindow& window) const;
		int GetQ() const;
		int GetR() const;
		int GetS() const;

		float GetElevation() const;
		void SetElevation(float elevation);

		Elevation GetElevationType() const;
		void SetElevationType(Elevation elevation);

		Type GetTileType() const;
		void SetTileType(Type type);

		Vegetation GetTileVegetation() const;
		void SetTileVegetation(Vegetation vegetation);

		float GetMoisture() const;
		void SetMoisture(float moisture);

		float GetTemperature() const;
		void SetTemperature(float temperature);

		int DistanceTo(Tile& t) const;
		static int GetWidth();
		static int GetHeight();
	private:
		sf::IntRect MapTileRect() const;
		sf::IntRect MapVegetationRect() const;
	private:
		const int Q;	//	Column
		const int R;	//	Row
		const int S;

		float elevation = -0.5f;
		float moisture = 0.4f;
		// Temperature ranges from 0 to 1
		float temperature = 0.6f;

		const Map& map;

		Type tileType = Type::Ocean;
		Elevation tileElevation = Elevation::Water;
		Vegetation tileVegetation = Vegetation::None;

		static constexpr float radius = 220.0f;
		static constexpr float WIDTH_MULTIPLIER = 1.0f;
	};
public:
	Map(GameDataRef& data, sf::View& mapView);

	void Draw() const;
	void MoveView(const sf::Vector2i& deltaPos);
	void ZoomIn();
	void ZoomOut();
	void UpdateView();
	const sf::Texture& GetTileSprite() const;
	const sf::Vector2i& GetTileSize() const;
	bool IsWrappingAllowed() const;
	sf::Vector2i GetGridSize() const;
protected:
	virtual void GenerateMap();
	
	void UpdateTileVisuals();
	std::vector<sf::Vector2i> GetTilesWithinRangeOf(Tile& centerTile, int radius);
	void ElevateArea(int q, int r, int radius, float centerHeight = 0.8f);
	int DistanceBetween(Tile& a, Tile& b) const;
	Tile & TileAt(int x, int y);
	Tile & TileAt(sf::Vector2i gridPos)
	{
		return TileAt(gridPos.x, gridPos.y);
	}
	const Tile & TileAt(int x, int y) const;
	const Tile & TileAt(sf::Vector2i gridPos) const
	{
		return TileAt(gridPos.x, gridPos.y);
	}
	//sf::Vector2i GridToScreen(const int column, const int row) const;
protected:
	// maps size in columns and rows
	const int numColumns;
	const int numRows;

	std::vector<Tile> tiles;

	// Tile with height above whatever, is whatever
	float heightMountain = 1.0f;
	float heightHill = 0.6f;
	float heightFlat = 0.0f;

	// Tile with moisture above whatever is whatever
	float moistureJungle = 0.8f;
	float moistureForest = 0.5f;
	float moistureGrassland = 0.3f;
	float moisturePlains = 0.2f;

	// Tile with temperature above whatever is whatever
	// NOTE: 0.3 = 0 celcius, 1.0 = 30 celcius
	float temperatureDesert = 0.8f;
	float temperatureJungle = 0.65f;
	float temperatureForest = 0.45f;
	float temperatureGrassland = 0.35f;
	float temperaturePlains = 0.35f;
	float temperatureTundra = 0.2f;
private:
	GameDataRef& data;
	sf::View& mapView;

	static constexpr float zoomLevels[15] = { 1.0f, 1.25f, 1.5f, 2.0f, 2.5f, 3.0f, 4.0f, 5.0f, 6.0f, 7.5f, 9.0f, 12.0f, 15.0f, 18.0f, 22.0f };
	int mapZoomLevel;

	const sf::Vector2i tileSize = { 440, 440 };

	sf::Texture tileSprite;

	// is cylindrical wrapping required?
	bool allowWrappingEastWest = true;
	int rowDist = int((float)tileSize.y * 0.75f);

	// dimensions of the map in pixels
	const int mapWidth = numColumns * tileSize.x + (tileSize.x/2);
	const int mapHeight = (int)((float)numRows * rowDist + (1.0f - 0.75f) * (float)tileSize.y);
};