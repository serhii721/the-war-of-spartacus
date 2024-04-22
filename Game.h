#pragma once
class Game
{
private:
	unique_ptr<Player> pPlayer;
	unique_ptr<WorldMap> pWorldMap;
	MenuManager menuManager;

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
};
