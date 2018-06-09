#include "Map.h"
#include "PerlinNoise.h"
#include <math.h>
#include <random>
#include <assert.h>

Map::Map(sf::RenderWindow& window, Debug & debug, const Config& config)
	:
	debug(debug),
	window(window)
{
	// load textures
	tileSprite.loadFromFile("Resources\\Textures\\mapTiles60x60.bmp");

	// load user configs
	int mSize = config.Get(Config::Option::MapSize);
	int mWidth = config.Get(Config::Option::MapWidth);
	int mHeight = config.Get(Config::Option::MapHeight);
	int mWrap = config.Get(Config::Option::MapWrapping);
	int dGridCoords = config.Get(Config::Option::DebugGridCoordinates);
	if (mWidth != 0 && mHeight != 0)
	{
		numColumns = mWidth;
		numRows = mHeight;
	}
	else
	{
		switch (mSize)
		{
		case 0:
			numColumns = 48;
			numRows = 24;
			break;
		case 1:
			numColumns = 96;
			numRows = 48;
			break;
		case 2:
			numColumns = 144;
			numRows = 72;
			break;
		case 3:
			numColumns = 192;
			numRows = 96;
		}
	}
	if (mWrap == 1)
	{
		allowWrappingEastWest = true;
	}
	else
	{
		allowWrappingEastWest = false;
	}
	if (dGridCoords == 1)
	{
		drawGridCoords = true;
	}
	else
	{
		drawGridCoords = false;
	}
	mapWidth = numColumns * tileSize;
	mapHeight = numRows * tileSize;
	mapPos = sf::Vector2i(mapWidth, mapHeight) / -2 + (sf::Vector2i)window.getSize() / 2;
	// generate the map
	GenerateMap(Type::Continents);
}

void Map::Draw() const
{
	int tIndex = 0;
	// draw each tile on the screen
	for (Tile t : tiles)
	{
		// convert tile index to grid and screen coordinates
		sf::Vector2i gridPos = GridAtIndex(tIndex);
		sf::Vector2i scrnPos = GridToScreen(gridPos);
		// make sure tile is inside screen
		if (sf::IntRect(scrnPos, sf::Vector2i(tileSize, tileSize)).intersects(sf::IntRect(sf::Vector2i(0,0), (sf::Vector2i)window.getSize())))
		{
			// draw the tile
			t.Draw(scrnPos, window);
			if (drawGridCoords)
			{
				debug.DrawGridCoordinates(gridPos, scrnPos, window);
			}
		}
		tIndex++;
	}
}

void Map::MoveView(const sf::Vector2i& deltaPos)
{
	// move the map horizontally by the value provided
	if (allowWrappingEastWest)
	{
		// if east-west wrapping is used, make sure to reset
		// map position once it is moved too far in one disf::IntRecton
		int scrnCenter = window.getSize().x/2;
		mapPos.x -= deltaPos.x;
		if (mapPos.x + mapWidth / 2 < scrnCenter - mapWidth)
		{
			mapPos.x += mapWidth;
		}
		else if (mapPos.x + mapWidth / 2 > scrnCenter)
		{
			mapPos.x -= mapWidth;
		}
	}
	else
	{
		// if map width is less that screen width, lock map to the screen
		// otherwise don't allow the view to go off the map instead	
		if (mapWidth <= (int)window.getSize().x)
		{
			mapPos.x = std::min(std::max(mapPos.x - deltaPos.x, 0), (int)window.getSize().x - mapWidth);
		}
		else
		{
			mapPos.x = std::max(std::min(mapPos.x - deltaPos.x, 0), (int)window.getSize().x - mapWidth);
		}
	}

	// move the map vertically by the value provided
	// if map height is less that screen height, lock map to the screen
	// otherwise don't allow the view to go off the map instead
	if (mapHeight <= (int)window.getSize().y)
	{
		mapPos.y = std::min(std::max(mapPos.y - deltaPos.y, 0), (int)window.getSize().y - mapHeight);
	}
	else
	{
		mapPos.y = std::max(std::min(mapPos.y - deltaPos.y, 0), (int)window.getSize().y - mapHeight);
	}
}

bool Map::WrappingEastWest() const
{
	return allowWrappingEastWest;
}

sf::Vector2i Map::Size() const
{
	return sf::Vector2i(numColumns, numRows);
}

void Map::SetDrawGridCoordsTo(bool option)
{
	drawGridCoords = option;
}

Map::Tile & Map::TileAt(sf::Vector2i gridPos)
{
	// returns the tile object at specified grid position
	if (gridPos.x < 0)
	{
		gridPos.x += numColumns;
	}
	else if (gridPos.x >= numColumns)
	{
		gridPos.x -= numColumns;
	}
	return tiles[gridPos.x + gridPos.y * numColumns];
}

const Map::Tile & Map::TileAt(const sf::Vector2i & gridPos) const
{
	// returns the tile object at specified grid position
	return tiles[gridPos.x + gridPos.y * numColumns];
}

sf::Vector2i Map::GridToScreen(const sf::Vector2i & gridPos) const
{
	// convets grid coordinates to pixel coordinates (top left corner of a tile)
	sf::Vector2i scrnPos = { gridPos.x * tileSize + mapPos.x, gridPos.y * tileSize + mapPos.y };

	if (allowWrappingEastWest) {
		int scrnCenter = window.getSize().x/2;

		if (scrnPos.x + tileSize / 2 < scrnCenter - mapWidth / 2)
		{
			scrnPos.x += mapWidth;
		}
		else if (scrnPos.x + tileSize / 2 > scrnCenter + mapWidth / 2)
		{
			scrnPos.x -= mapWidth;
		}
	}

	return scrnPos;
}

sf::Vector2i Map::ScreenToGrid(const sf::Vector2i & scrnPos) const
{
	// converts the screen pixel coordinates to the grid coordinates
	sf::Vector2i gridPos = { (scrnPos.x - mapPos.x) / tileSize, (scrnPos.y - mapPos.y) / tileSize };

	if (allowWrappingEastWest) {
		int scrnCenter = window.getSize().x/2;

		if (scrnPos.x + tileSize / 2 < scrnCenter - mapWidth)
		{
			gridPos.x += numColumns;
		}
		else if (scrnPos.x + tileSize / 2 > scrnCenter + mapWidth)
		{
			gridPos.x -= numColumns;
		}
	}

	gridPos.x = gridPos.x % numColumns;

	return gridPos;
}

sf::Vector2i Map::GridAtIndex(int index) const
{
	// convert tile index to grid coordinates
	return sf::Vector2i(index % numColumns, index / numColumns);
}

std::vector<sf::Vector2i> Map::GetTilesAround(const sf::Vector2i & centerPos, float distance) const
{
	// convert distance to int and round it roperly
	int dist = (int)(distance + 0.5f);
	// make sure we don't check tiles that are outside of the map
	int minY = std::max(centerPos.y - dist, 0);
	int maxY = std::min(centerPos.y + dist, numRows - 1);
	int minX = centerPos.x - dist;
	int maxX = centerPos.x + dist;
	// for the x cordinates allow checking out of bounds tiles as long as world wrapping is enable
	// otherwise do the same check as with y coordinates
	if (!allowWrappingEastWest)
	{
		minX = std::max(minX, 0);
		maxX = std::min(maxX, numColumns - 1);
	}

	std::vector<sf::Vector2i> area;
	for (int y = minY; y <= maxY; y++)
	{
		for (int x = minX; x <= maxX; x++)
		{
			if (TileAt(centerPos).DistanceToTile({ x,y }) <= distance + 0.5f)
			{
				area.emplace_back(sf::Vector2i(x, y));
			}
		}
	}
	return area;
}

float Map::Tile::DistanceToTile(const sf::Vector2i & gridPos) const
{
	int x = position.x - gridPos.x;
	int y = position.y - gridPos.y;
	return (float)std::sqrt(x * x + y * y);
}

void Map::GenerateMap(Type type)
{
	// set up rng for map generation
	std::random_device rd;
	std::mt19937 rng(rd());
	int mapSize = numColumns * numRows;
	// make sure that map is empty before starting
	if (tiles.size() > 0)
	{
		tiles.clear();
	}
	// generate empty ocean map
	for (int i = 0; i < mapSize; i++)
	{
		tiles.emplace_back(Tile(*this, Tile::ElevationType::Ocean, GridAtIndex(i), tileSprite));
	}

	if (type == Type::Continents)
	{
		// TODO: Redo this the map generation
		// The idea is to first just generate basic the continent shapes (with like blobs below),
		// then try to adjust the ocean depth so that ocean is depest the further away it is
		// from continents, also try to smooth it out.

		// generate continents map
		// generate frist continet position
		std::uniform_int_distribution<int> contPos(0, numColumns - 1);
		int cPos = contPos(rng);
		// determine number of continents in the world
		int numberOfContinents = std::max(1, numColumns / 24);
		int continentsGenerated = 0;
		float blobsMultiplier = ((float)numRows * 2.0f) / 40.0f;
		// generate continents (will always generate at least 1)
		do
		{
			// generate number of "blobs" of landmass in the continent
			std::uniform_int_distribution<int> nBlobs((int)(5.0f * blobsMultiplier), (int)(15.0f * blobsMultiplier));
			int numBlobs = nBlobs(rng);
			for (int i = 0; i <= numBlobs; i++)
			{
				// generate size of a blob
				std::uniform_int_distribution<int> blobSize(2, 5);
				int bSize = blobSize(rng);
				// generate position of a blob
				std::uniform_int_distribution<int> blobPosY(bSize + 1, numRows - bSize - 2);
				std::uniform_int_distribution<int> blobPosX(cPos - 4, cPos + 4);

				// elevate area of the blob
				ElevateArea(GetTilesAround({ blobPosX(rng), blobPosY(rng) }, (float)bSize));
			}
			// calculate position of next continent
			cPos = (cPos + numColumns / numberOfContinents) % numColumns;
			continentsGenerated++;
		} while (continentsGenerated < numberOfContinents);
		// add Perlin noise to the values
		std::uniform_int_distribution<unsigned int> noiseSeed(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
		PerlinNoise pn = { noiseSeed(rng) };
		// determines size of Perlin noise, small values give large patches, large values give small patches
		double noiseFactor = 10.0f;
		// how big of a value can Perlin noise generate ( scale of 1 gives -0.5 t 0.5)
		float noiseScale = 2.5f;
		// just a number to divide by, will be number of columns or rows, whichever is larger
		// double divisionFactor = std::max((double)numColumns, (double)numRows);
		double divisionFactor = 72.0f;
		// loop through all tiles to apply Perlin noise to
		for (Tile& t : tiles)
		{
			// get position of a tile
			sf::Vector2i pos = t.GetPosition();
			// calculate the x,y,z components to pass to the noise function
			double xFactor = (double)pos.x / divisionFactor * noiseFactor;
			double yFactor = (double)pos.y / divisionFactor * noiseFactor;
			// z is just an average of x and y
			double zFactor = (xFactor + yFactor) / 2;
			// generate the noise value. The average value generated is 0.1 instead of 0.
			// this is to slightly reduce number water tiles generated
			float pNoise = (float)(pn.noise(xFactor, yFactor, zFactor) - 0.4f) * noiseScale;
			// apply noise to the current elevation
			t.SetElevation(t.GetElevation() + pNoise);
		}

		// smooth out the map a bit to make sure theres always a shore tile between land and ocean tiles
		// in order to do that, for each ocean tile on the map, we scan all tiles adjacent to it. If any of them
		// contains a land tile, change this tile elevation to be a shore

		// first create the rng
		std::uniform_real_distribution<float> shoreElevation(-0.5f, -0.01f);

		for (Tile& t : tiles)
		{
			if (t.GetElevationType() == Tile::ElevationType::Ocean)
			{
				for (sf::Vector2i adjacentTile : GetTilesAround(t.GetPosition(), 2.0f))
				{
					if ((int)TileAt(adjacentTile).GetElevationType() >= (int)Tile::ElevationType::Flat)
					{
						t.SetElevation(shoreElevation(rng));
					}
				}
			}
		}
	}
}

void Map::ElevateArea(const std::vector<sf::Vector2i>& area)
{
	for (sf::Vector2i v : area)
	{
		TileAt(v).SetElevation(0.2f);
	}
}

sf::IntRect Map::Tile::MapTileRect() const
{
	// return the correct tile sprite from sprite map
	return sf::IntRect({ 0, tileSize * (int)elevationType }, { tileSize, tileSize });
}

Map::Tile::Tile(Map& map, char c, const sf::Vector2i& gridPos, const sf::Texture& tileSprite)
	:
	map(map),
	tileSprite(tileSprite),
	position(gridPos)
{
	// convert from characters to ElevationType and TerrainType
	const char elevIndex = c - 'A';
	assert(elevIndex >= (int)ElevationType::Ocean && elevIndex < (int)ElevationType::Count);
	// set elevation and terrain to the values supplied via the string map
	elevationType = (ElevationType)elevIndex;
}

Map::Tile::Tile(Map& map, ElevationType elevationType, const sf::Vector2i& gridPos, const sf::Texture& tileSprite)
	:
	map(map),
	tileSprite(tileSprite),
	position(gridPos)
{
	assert(elevationType >= ElevationType::Ocean && elevationType < ElevationType::Count);
	// set elevation and terrain to the values supplied via the string map
	this->elevationType = elevationType;
}

void Map::Tile::Draw(const sf::Vector2i & scrnPos, sf::RenderWindow& window) const
{
	// draw selected tile on the screen
	sf::Sprite sprite = sf::Sprite(tileSprite, MapTileRect());
	sprite.setPosition((sf::Vector2f)scrnPos);
	window.draw(sprite);
}

void Map::Tile::SetElevation(float newElevation)
{
	elevation = std::max(-10.0f, std::min(10.0f, newElevation));
	if (elevation > 1.0f)
	{
		elevationType = ElevationType::Mountain;
	}
	else if (elevation > 0.5f)
	{
		elevationType = ElevationType::Hill;
	}
	else if (elevation >= 0.0f)
	{
		elevationType = ElevationType::Flat;
	}
	else if (elevation >= -0.5f)
	{
		elevationType = ElevationType::Shore;
	}
	else
	{
		elevationType = ElevationType::Ocean;
	}
}

float Map::Tile::GetElevation() const
{
	return elevation;
}

Map::Tile::ElevationType Map::Tile::GetElevationType() const
{
	return elevationType;
}

sf::Vector2i Map::Tile::GetPosition() const
{
	return position;
}
