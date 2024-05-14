#pragma once

class WorldMap
{
private:
	vector<HWND> hItems;

	vector<City> cities;
	int currentCity;
	int selectedCity;

	int currentScreen;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

public:
	enum Item
	{
		BUT_ROME_MAP,
		BUT_NAPLES_MAP,
		BUT_METAPONTO_MAP,
		BUT_BOJANO_MAP,
		BUT_ANCONA_MAP,
		BUT_PERUGIA_MAP,
		BUT_FLORENCE_MAP,
		BUT_BOLOGNA_MAP,
		BUT_GENOA_MAP,
		BUT_VENICE_MAP,
		BUT_MILAN_MAP,
		BUT_ROME_LIST,
		BUT_NAPLES_LIST,
		BUT_METAPONTO_LIST,
		BUT_BOJANO_LIST,
		BUT_ANCONA_LIST,
		BUT_PERUGIA_LIST,
		BUT_FLORENCE_LIST,
		BUT_BOLOGNA_LIST,
		BUT_GENOA_LIST,
		BUT_VENICE_LIST,
		BUT_MILAN_LIST,
		STAT_MAP,
		BUT_TRAVEL_LIST,
		BUT_ENTER_LIST,
		ITEM_NUMBER
	};
	enum Screen
	{
		SCREEN_ITEM_NUMBER
	};
	WorldMap();
	WorldMap(HWND, const vector<City>& cities_, int currentCity_ = 0);
	WorldMap(const WorldMap&);
	WorldMap& operator=(const WorldMap&);
	~WorldMap();

	City& getCurrentCity();

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
	void stylizeWindow(HWND, UINT, WPARAM, LPARAM);
};
