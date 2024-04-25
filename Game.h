#pragma once

class Game
{
private:
	MenuManager menuManager;
	unique_ptr<WorldMap> pWorldMap;
	unique_ptr<Fighting> pFighting;
	unique_ptr<Player> pPlayer;

	HFONT customFont;

	DisplayState displayState;

public:
	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	void setPlayer(const Player&);
	void setWorldMap(const WorldMap&);
	void setFighting(const Fighting&);
	void setDisplayState(DisplayState);

	Player& getPlayer() const;
	WorldMap& getWorldMap() const;
	Fighting& getFighting() const;
	MenuManager& getMenuManager();

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
};
