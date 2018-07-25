#pragma once

#include "Map.h"

class Map_Continents : public Map
{
public:
	Map_Continents(GameDataRef& data, sf::View& mapView);
	void GenerateMap() override;
};