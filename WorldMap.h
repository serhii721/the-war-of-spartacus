#pragma once

class WorldMap
{
private:
	vector<City> cities;
	int currentCity;

public:
	WorldMap();
	WorldMap(const vector<City>& cities_, int currentCity_ = 0);
	WorldMap(const WorldMap&);
	WorldMap& operator=(const WorldMap&);
	~WorldMap();

	City& getCurrentCity();
};
