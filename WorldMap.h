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

	void drawWindow(HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
};
