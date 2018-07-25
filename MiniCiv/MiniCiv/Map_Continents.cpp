#include "Map_Continents.h"
#include <random>

Map_Continents::Map_Continents(GameDataRef & data, sf::View & mapView)
	: Map(data, mapView)
{
	GenerateMap();
}

void Map_Continents::GenerateMap()
{
	//////////////////////////////////////////////////////////////////////////////
	/// First call the parent version of GenerateMap to make all the hexes we need
	//////////////////////////////////////////////////////////////////////////////

	Map::GenerateMap();

	// set up rng for map generation
	std::random_device rd;
	std::mt19937 rng(rd());
	//std::mt19937 rng(0);

	////////////////////////////////////////////////////////
	/// Make random hexagonal blobs to form basic continents
	////////////////////////////////////////////////////////

	int numContinents = 3;
	int continentDistance = numColumns / numContinents;

	for (int c = 0; c < numContinents; c++)
	{
		// Make some kind of a raised area
		int numSplats = std::uniform_int_distribution<int>(10, 18)(rng);
		for (int i = 0; i < numSplats; i++)
		{
			int range = std::uniform_int_distribution<int>(3, 6)(rng);
			int y = std::uniform_int_distribution<int>(range, numRows - 1 - range)(rng);
			int x = std::uniform_int_distribution<int>(0, 7)(rng) - y / 2 + (c * continentDistance);

			ElevateArea(x, y, range);
		}
	}

	///////////////////////////////////
	/// Add lumpines using Perlin Noise
	///////////////////////////////////

	// add Perlin noise to the values
	std::uniform_int_distribution<unsigned int> landmassNoiseSeed(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
	std::uniform_int_distribution<unsigned int> extremesNoiseSeed(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
	PerlinNoise pnLandmass = { landmassNoiseSeed(rng) };
	PerlinNoise pnExtremes = { extremesNoiseSeed(rng) };
	// determines size of Perlin noise, small values give large patches, large values give small patches
	double landmassNoiseFactor = 15.0f;
	double extremesNoiseFactor = 40.0f;
	// how big of a value can Perlin noise generate ( scale of 1 gives -0.5 t 0.5)
	// larger value crates more islands (and lakes I guess)
	float landmassNoiseScale = 1.6f;
	float extremesNoiseScale = 1.4f;
	// just a number to divide by, will be number of columns or rows, whichever is larger
	double divisionFactor = (double)std::max(numColumns, numRows);
	// loop through all tiles to apply Perlin noise to
	for (int column = 0; column < numColumns; column++)
	{
		for (int row = 0; row < numRows; row++)
		{
			Tile& t = TileAt(column, row);

			double xFactorLandmass = (double)column / divisionFactor * landmassNoiseFactor;
			double yFactorLandmass = (double)row / divisionFactor * landmassNoiseFactor;

			double xFactorExtremes = (double)column / divisionFactor * extremesNoiseFactor;
			double yFactorExtremes = (double)row / divisionFactor * extremesNoiseFactor;

			float pNoiseLandmass = float(pnLandmass.noise(xFactorLandmass, yFactorLandmass, 0.0) -0.5) * landmassNoiseScale;
			float pNoiseExtremes = float(pnExtremes.noise(xFactorExtremes, yFactorExtremes, 0.0) -0.5) * extremesNoiseScale;

			float pNoise = pNoiseLandmass + pNoiseExtremes;
			t.SetElevation(t.GetElevation() + pNoise);
		}
	}

	////////////////////////////////////////////////////////////////////////////////////
	/// Simulate temperature (just a formula calculating rough temperature per latitude)
	////////////////////////////////////////////////////////////////////////////////////

	std::uniform_real_distribution<float> tempRange(-0.05f, 0.05f);

	for (int column = 0; column < numColumns; column++)
	{
		for (int row = 0; row < numRows; row++)
		{
			Tile& t = TileAt(column, row);

			// Not really a latitude but a distance to poles ranging from 0 to 100;
			float equator = numRows / 2.0f;
			float latitude = std::abs(equator - (float)row) / equator;

			float sine = std::sin(0.1f);
			float temp = 1.0f - sine * sine * latitude * 100.0f;

			temp += tempRange(rng);

			t.SetTemperature(temp);
		}
	}

	////////////////////////////////////////////////////////////////
	/// Simulate rainfall/moisture (probably just Perlin it for now)
	////////////////////////////////////////////////////////////////

	std::uniform_int_distribution<unsigned int> noiseSeed(std::numeric_limits<unsigned int>::min(), std::numeric_limits<unsigned int>::max());
	PerlinNoise pn = { noiseSeed(rng) };
	// determines size of Perlin noise, small values give large patches, large values give small patches
	double noiseFactor = 15.0f;
	// how big of a value can Perlin noise generate ( scale of 1 gives -0.5 t 0.5)
	// larger value crates more extreme moisture
	float noiseScale = 2.5f;
	// loop through all tiles to apply Perlin noise to
	for (int column = 0; column < numColumns; column++)
	{
		for (int row = 0; row < numRows; row++)
		{
			Tile& t = TileAt(column, row);

			double xFactor = (double)column / divisionFactor * noiseFactor;
			double yFactor = (double)row / divisionFactor * noiseFactor;

			float pNoise = float(pn.noise(xFactor, yFactor, 0.0) - 0.5) * noiseScale;
			t.SetMoisture(t.GetMoisture() + pNoise);
		}
	}

	///////////////////////////////////////////
	/// Add shores between ocean and land tiles
	///////////////////////////////////////////

	for (Tile& t : tiles)
	{
		if (t.GetElevationType() == Tile::Elevation::Water)
		{
			for (sf::Vector2i adjacentTile : GetTilesWithinRangeOf(t, 2))
			{
				if (adjacentTile.y >= 0 && adjacentTile.y < numRows) {
					if (TileAt(adjacentTile).GetElevation() > heightFlat || t.GetElevation() > -0.2f)
					{
						t.SetTileType(Tile::Type::Shore);
					}
				}
			}
		}
	}

	// Now make sure all the hex visuals are updated to match the data
	UpdateTileVisuals();
}
