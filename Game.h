#pragma once
class Game
{
private:
	unique_ptr<Player> pPlayer;
	MenuManager menuManager;

public:
	Game();
	Game(const Game&);
	Game& operator=(const Game&);
	~Game();

	void setPlayer(const Player&);

	Player& getPlayer() const;
	MenuManager& getMenuManager();

	City city;
};
