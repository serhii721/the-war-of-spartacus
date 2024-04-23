#pragma once
class Game
{
private:
	MenuManager menuManager;
	unique_ptr<WorldMap> pWorldMap;
	unique_ptr<Fighting> pFighting;
	unique_ptr<Player> pPlayer;

	enum DisplayState { MENU, WORLD_MAP, FIGHTING };
	DisplayState displayState;

public:
	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	void setPlayer(const Player&);
	void setWorldMap(const WorldMap&);

	Player& getPlayer() const;
	WorldMap& getWorldMap() const;
	MenuManager& getMenuManager();

	void drawWindow(HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
};
