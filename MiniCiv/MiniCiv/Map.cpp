#include "Map.h"

Map::Map(GameDataRef & data, sf::View & mapView)
	:
	data(data),
	mapView(mapView),
	mapZoomLevel(5),
	numColumns(64),
	numRows(32)
{
	tileSprite.loadFromFile("Resources\\Textures\\mapTiles440x440.png");
	mapView.setCenter(sf::Vector2f((float)mapWidth/2.0f, (float)mapHeight/2.0f));
	mapView.setSize((sf::Vector2f)data->window.getSize() * zoomLevels[mapZoomLevel]);
}

void Map::GenerateMap()
{
	// Generate an empty ocean map
	for (int row = 0; row < numRows; row++)
	{
		for (int column = 0; column < numColumns; column++)
		{
			// Create a tile
			tiles.emplace_back(*this, column, row);
		}
	}

	UpdateTileVisuals();
}

void Map::UpdateTileVisuals()
{
	for (Tile& t : tiles)
	{
		// Set terrain elevation
		if (t.GetElevation() > heightMountain)
		{
			t.SetElevationType(Tile::Elevation::Mountain);
			t.SetTileType(Tile::Type::Mountain);
		}
		else if (t.GetElevation() > heightHill)
		{
			t.SetElevationType(Tile::Elevation::Hill);
		}
		else if (t.GetElevation() > heightFlat)
		{
			t.SetElevationType(Tile::Elevation::Flat);
		}
		else
		{
			t.SetElevationType(Tile::Elevation::Water);
			if (t.GetTileType() != Tile::Type::Shore) t.SetTileType(Tile::Type::Ocean);
		}

		// Set terrain temperature/moisture
		if (t.GetElevationType() > Tile::Elevation::Water && t.GetElevationType() < Tile::Elevation::Mountain)
		{
			if (t.GetTemperature() < temperatureTundra)
			{
				t.SetTileType(Tile::Type::Snow);
				t.SetTileVegetation(Tile::Vegetation::None);
			}
			else if (t.GetTemperature() < temperaturePlains)
			{
				t.SetTileType(Tile::Type::Tundra);

				if (t.GetMoisture() >= moistureForest)
				{
					t.SetTileVegetation(Tile::Vegetation::Forest);
				}
				else
				{
					t.SetTileVegetation(Tile::Vegetation::None);
				}
			}
			else if (t.GetTemperature() < temperatureForest)
			{
				if (t.GetMoisture() >= moistureGrassland)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Grass);
				}
				else
				{
					t.SetTileType(Tile::Type::Plains);
					t.SetTileVegetation(Tile::Vegetation::None);
				}
			}
			else if (t.GetTemperature() < temperatureJungle)
			{
				if (t.GetMoisture() >= moistureForest)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Forest);
				}
				else if (t.GetMoisture() >= moistureGrassland)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Grass);
				}
				else
				{
					t.SetTileType(Tile::Type::Plains);
					t.SetTileVegetation(Tile::Vegetation::None);
				}
			}
			else if (t.GetTemperature() < temperatureDesert)
			{
				if (t.GetMoisture() >= moistureJungle)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Jungle);
				}
				else if (t.GetMoisture() >= moistureForest)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Forest);
				}
				else if (t.GetMoisture() >= moistureGrassland)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Grass);
				}
				else
				{
					t.SetTileType(Tile::Type::Plains);
					t.SetTileVegetation(Tile::Vegetation::None);
				}
			}
			else
			{
				if (t.GetMoisture() >= moistureJungle)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Jungle);
				}
				else if (t.GetMoisture() >= moistureForest)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Forest);
				}
				else if (t.GetMoisture() >= moistureGrassland)
				{
					t.SetTileType(Tile::Type::Grassland);
					t.SetTileVegetation(Tile::Vegetation::Grass);
				}
				else if (t.GetMoisture() >= moisturePlains)
				{
					t.SetTileType(Tile::Type::Plains);
					t.SetTileVegetation(Tile::Vegetation::None);
				}
				else
				{
					t.SetTileType(Tile::Type::Desert);
					t.SetTileVegetation(Tile::Vegetation::None);
				}
			}
		}
		else
		{
			t.SetTileVegetation(Tile::Vegetation::None);
		}
	}
}

std::vector<sf::Vector2i> Map::GetTilesWithinRangeOf(Tile& centerTile, int range)
{
	std::vector<sf::Vector2i> results;
	for (int dx = -range + 1; dx < range; dx++)
	{
		for (int dy = std::max(-range + 1, -dx - range + 1); dy < std::min(range, -dx + range); dy++)
		{
			results.emplace_back(centerTile.GetQ() + dx, centerTile.GetR() + dy);
		}
	}
	return results;
}

void Map::ElevateArea(int q, int r, int range, float centerHeight)
{
	Tile& centerHex = TileAt(q, r);

	std::vector<sf::Vector2i> tilesToElevate = GetTilesWithinRangeOf(centerHex, range);

	for (sf::Vector2i v : tilesToElevate)
	{
		Tile& t = TileAt(v);
		
		float distFraction = (float)t.DistanceTo(centerHex) / (float)range;

		t.SetElevation(centerHeight * math::lerp(0.8f, 0.3f, distFraction * distFraction));
	}
}

int Map::DistanceBetween(Tile & a, Tile & b) const
{
	return a.DistanceTo(b);
}

Map::Tile & Map::TileAt(int x, int y)
{
	assert(tiles.size() > 0);
	assert(y >= 0 && y < numRows);

	if (allowWrappingEastWest)
	{
		x = x % numColumns;
		if (x < 0) x += numColumns;
	}
	else
	{
		assert(x >= 0 && x < numColumns);
	}

	return tiles[x + y * numColumns];
}

const Map::Tile & Map::TileAt(int x, int y) const
{
	assert(tiles.size() > 0);
	assert(x >= 0 && x < numColumns && y >= 0 && y < numRows);

	return tiles[x + y * numColumns];
}

void Map::Draw() const
{
	for (const Tile& t : tiles)
	{
		sf::Vector2i pos = t.PositionFromView(mapView, mapWidth, allowWrappingEastWest);
		if (sf::IntRect(sf::Vector2i(mapView.getCenter() - mapView.getSize() / 2.0f), (sf::Vector2i)mapView.getSize()).intersects(sf::IntRect(pos, sf::Vector2i(Tile::GetWidth(), Tile::GetHeight()))))
		{
			t.Draw(pos, data->window);
			if (data->settings.GetShowGridCoords()) data->debug.DrawGridCoordinates(sf::Vector2i(t.GetQ(), t.GetR()), pos, data->window);
		}
	}
}

void Map::MoveView(const sf::Vector2i & deltaPos)
{
	// move the map view by the delta provided
	mapView.move((sf::Vector2f)deltaPos * zoomLevels[mapZoomLevel]);
}

void Map::ZoomIn()
{
	if (mapZoomLevel > 0)
	{
		if (data->settings.GetZoomToMouse())
		{
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(data->window);
			mousePos -= sf::Vector2f(data->window.getSize()) / 2.0f;
			sf::Vector2f mousePosMap = mousePos * zoomLevels[mapZoomLevel];
			mousePosMap += mapView.getCenter();

			mapView.setSize((sf::Vector2f)data->window.getSize() * zoomLevels[--mapZoomLevel]);

			sf::Vector2f newMousePos = (sf::Vector2f)sf::Mouse::getPosition(data->window);
			newMousePos -= sf::Vector2f(data->window.getSize()) / 2.0f;
			sf::Vector2f newMousePosMap = newMousePos * zoomLevels[mapZoomLevel];
			newMousePosMap += mapView.getCenter();

			mapView.move((mousePosMap - newMousePosMap));
		}
		else
		{
			mapView.setSize((sf::Vector2f)data->window.getSize() * zoomLevels[--mapZoomLevel]);
		}
	}
}

void Map::ZoomOut()
{
	if (mapZoomLevel < 14)
	{

		if (data->settings.GetZoomToMouse())
		{
			sf::Vector2f mousePos = (sf::Vector2f)sf::Mouse::getPosition(data->window);
			mousePos -= sf::Vector2f(data->window.getSize()) / 2.0f;
			sf::Vector2f mousePosMap = mousePos * zoomLevels[mapZoomLevel];
			mousePosMap += mapView.getCenter();

			mapView.setSize((sf::Vector2f)data->window.getSize() * zoomLevels[++mapZoomLevel]);

			sf::Vector2f newMousePos = (sf::Vector2f)sf::Mouse::getPosition(data->window);
			newMousePos -= sf::Vector2f(data->window.getSize()) / 2.0f;
			sf::Vector2f newMousePosMap = newMousePos * zoomLevels[mapZoomLevel];
			newMousePosMap += mapView.getCenter();

			mapView.move((mousePosMap - newMousePosMap));
		}
		else
		{
			mapView.setSize((sf::Vector2f)data->window.getSize() * zoomLevels[++mapZoomLevel]);
		}
	}
}

void Map::UpdateView()
{
	// Make sure the zoom level is within bounds
	if (mapView.getSize().x >(float)mapWidth || mapView.getSize().y > (float)mapHeight)
	{
		ZoomIn();
	}

	// Make sure the "camera" position is within bounds
	// if view height is more than map height, lock map to the view
	// otherwise don't allow the view to go off the map instead
	if (mapView.getSize().y > (float)mapHeight)
	{
		mapView.setCenter(mapView.getCenter().x, std::min(std::max(mapView.getCenter().y, mapHeight - mapView.getSize().y / 2.0f), mapView.getSize().y / 2.0f));
	}
	else
	{
		mapView.setCenter(mapView.getCenter().x, std::max(std::min(mapView.getCenter().y, mapHeight - mapView.getSize().y / 2.0f), mapView.getSize().y / 2.0f));
	}
}

const sf::Texture & Map::GetTileSprite() const
{
	return tileSprite;
}

const sf::Vector2i & Map::GetTileSize() const
{
	return tileSize;
}

bool Map::IsWrappingAllowed() const
{
	return allowWrappingEastWest;
}

sf::Vector2i Map::GetGridSize() const
{
	return sf::Vector2i(numColumns, numRows);
}

//sf::Vector2i Map::GridToScreen(const int column, const int row) const
//{
//	// converts grid coordinates to pixel coordinates (top left corner of a tile)
//	int scrnX = column * tileSize.x + row * (tileSize.x/2) + mapPos.x;
//	int scrnY = row * rowDist + mapPos.y;
//
//	if (allowWrappingEastWest) {
//		int scrnCenter = data->window.getSize().x / 2;
//
//		if (scrnX + tileSize.x / 2 < scrnCenter - mapWidth / 2)
//		{
//			scrnX += (mapWidth - tileSize.x / 2);
//		}
//		else if (scrnX + tileSize.x / 2 > scrnCenter + mapWidth / 2)
//		{
//			scrnX -= (mapWidth - tileSize.x / 2);
//		}
//	}
//
//	return sf::Vector2i(scrnX, scrnY);
//}

Map::Tile::Tile(const Map& map, int column, int row)
	:
	map(map),
	Q(column),
	R(row),
	S(-(Q+R))
{}

sf::Vector2i Map::Tile::Position() const
{
	float height = radius * 2.0f;
	float width = WIDTH_MULTIPLIER * height;
	
	float vert = height * 0.75f;
	float horiz = width;

	return sf::Vector2i((int)(horiz * ((float)Q + (float)R / 2.0f)), (int)(vert * (float)R));
}

sf::Vector2i Map::Tile::PositionFromView(sf::View & mapView, int mapWidthIn, bool allowWrappingEastWest) const
{
	int mapWidth = mapWidthIn - GetWidth() / 2;
	sf::Vector2i position = Position();

	if (allowWrappingEastWest)
	{
		float howManyWidthsFromViewCenter = ((float)position.x - mapView.getCenter().x) / (float)mapWidth;

		if (abs(howManyWidthsFromViewCenter) <= 0.5f)
		{
			return position;
		}

		if (howManyWidthsFromViewCenter > 0) howManyWidthsFromViewCenter += 0.5f;
		else howManyWidthsFromViewCenter -= 0.5f;

		int howManyWidthToFix = (int)howManyWidthsFromViewCenter;

		position.x -= howManyWidthToFix * mapWidth;
	}

	return position;
}

void Map::Tile::Draw(sf::Vector2i scrnPos, sf::RenderWindow& window) const
{
	sf::Sprite tile = sf::Sprite(map.GetTileSprite(), MapTileRect());
	tile.setPosition((sf::Vector2f)scrnPos);
	window.draw(tile);

	if (tileVegetation != Vegetation::None)
	{
		sf::Sprite vegetation = sf::Sprite(map.GetTileSprite(), MapVegetationRect());
		vegetation.setPosition((sf::Vector2f)scrnPos);
		window.draw(vegetation);
	}
}

int Map::Tile::GetQ() const
{
	return Q;
}

int Map::Tile::GetR() const
{
	return R;
}

int Map::Tile::GetS() const
{
	return S;
}

float Map::Tile::GetElevation() const
{
	return elevation;
}

void Map::Tile::SetElevation(float elevationIn)
{
	elevation = elevationIn;

	// TODO: set tile elevation type by elevation height;
}

Map::Tile::Elevation Map::Tile::GetElevationType() const
{
	return tileElevation;
}

void Map::Tile::SetElevationType(Elevation elevationIn)
{
	assert((int)elevationIn >= 0 && elevationIn < Elevation::Count);
	tileElevation = elevationIn;
}

Map::Tile::Type Map::Tile::GetTileType() const
{
	return tileType;
}

void Map::Tile::SetTileType(Type typeIn)
{
	assert((int)typeIn >= 0 && typeIn < Type::Count);
	tileType = typeIn;
}

Map::Tile::Vegetation Map::Tile::GetTileVegetation() const
{
	return tileVegetation;
}

void Map::Tile::SetTileVegetation(Vegetation vegetationIn)
{
	assert((int)vegetationIn >= 0 && vegetationIn < Vegetation::Count);
	tileVegetation = vegetationIn;
}

void Map::Tile::SetMoisture(float moistureIn)
{
	moisture = moistureIn;
}

float Map::Tile::GetTemperature() const
{
	return temperature;
}

void Map::Tile::SetTemperature(float temperatureIn)
{
	temperature = temperatureIn;
}

float Map::Tile::GetMoisture() const
{
	return moisture;
}

int Map::Tile::DistanceTo(Tile & t) const
{
	int dQ = std::abs(Q - t.GetQ());
	int dS = std::abs(S - t.GetS());
	if (map.IsWrappingAllowed()) {
		if (dQ > map.GetGridSize().x / 2)
		{
			dQ -= map.GetGridSize().x;
			dS -= map.GetGridSize().x;
		}
	}

	return std::max(
		dQ,
		std::max(
			std::abs(R - t.GetR()),
			dS
		)
	);
}

int Map::Tile::GetWidth()
{
	return int(radius * 2.0f * WIDTH_MULTIPLIER);
}

int Map::Tile::GetHeight()
{
	return int(radius * 2.0f);
}

sf::IntRect Map::Tile::MapTileRect() const
{
	return sf::IntRect({map.GetTileSize().x * (int)tileType, map.GetTileSize().y * (int)tileElevation}, map.GetTileSize());
}

sf::IntRect Map::Tile::MapVegetationRect() const
{
	return sf::IntRect({ map.GetTileSize().x * (int)tileVegetation, map.GetTileSize().y * (int)Elevation::Count }, map.GetTileSize());
}
