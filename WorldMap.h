#pragma once

class WorldMap
{
private:
	vector<City> cities;
	int currentCity;

	int currentScreen;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

public:
	enum Screen
	{
		ITEM_NUMBER
	};
	WorldMap();
	WorldMap(const vector<City>& cities_, int currentCity_ = 0);
	WorldMap(const WorldMap&);
	WorldMap& operator=(const WorldMap&);
	~WorldMap();

	City& getCurrentCity();

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
};
