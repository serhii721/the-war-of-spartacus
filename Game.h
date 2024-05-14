#pragma once

class Game
{
private:
	MenuManager menuManager;
	unique_ptr<WorldMap> pWorldMap;
	unique_ptr<Fighting> pFighting;
	unique_ptr<Player> pPlayer;

	HFONT smallFont, mediumFont, largeFont;
	
	DisplayState displayState;

public:
	enum Background
	{
		MAIN_MENU,
		MAIN_MENU_LOAD,
		MAIN_MENU_NEW_GAME,
		MAIN_MENU_SETTINGS,
		MAIN_MENU_SPECIALS,
		CITY_MENU,
		CITY_MENU_ARENA,
		CITY_MENU_QUEST,
		CITY_MENU_MARKET,
		CITY_MENU_CHARACTER,
		CITY_MENU_ARENA_FIGHT,
		CITY_MENU_ARENA_BET,
		GAME_MENU,
		GAME_MENU_SAVE,
		GAME_MENU_LOAD,
		GAME_MENU_SETTINGS,
		WORLD_MAP,
		FIGHTING_ARENA
	};
	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	void setPlayer(const Player&);
	void setWorldMap(const WorldMap&);
	void setFighting(const Fighting&);
	void setDisplayState(DisplayState);

	Player& getPlayer();
	WorldMap& getWorldMap() const;
	Fighting& getFighting() const;
	MenuManager& getMenuManager();

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);

private:
	Background currentBackground;
	bool backgroundChanged;

public:
	void setBackground(Background);
	void backgroundChangeCompleted();
	void updateBackground();
	Background getBackground() const;
	bool isBackgroundChanged() const;
};
