#include "stdafx.h"
#include "Game.h"

Game::Game() : pPlayer(make_unique<Player>()), pWorldMap(make_unique<WorldMap>()), menuManager() { }

Game::Game(const Game& GAME) :
	pPlayer(make_unique<Player>(*GAME.pPlayer)),
	pWorldMap(make_unique<WorldMap>(*GAME.pWorldMap)),
	menuManager(GAME.menuManager)
{ }

Game& Game::operator=(const Game& GAME)
{
	if (&GAME == this) return *this;

	if (GAME.pPlayer)
	{
		if (!pPlayer)
			pPlayer = make_unique<Player>(*GAME.pPlayer);
		else
			*pPlayer = *GAME.pPlayer;
	}
	else pPlayer = nullptr;

	if (GAME.pWorldMap)
	{
		if (!pWorldMap)
			pWorldMap = make_unique<WorldMap>(*GAME.pWorldMap);
		else
			*pWorldMap = *GAME.pWorldMap;
	}
	else
		pWorldMap = nullptr;

	menuManager = GAME.menuManager;

	return *this;
}

Game::~Game() { }

void Game::setPlayer(const Player& rPlayer)
{
	*pPlayer = rPlayer;
}

void Game::setWorldMap(const WorldMap& rWorldMap)
{
	*pWorldMap = rWorldMap;
}

Player& Game::getPlayer() const
{
	return *pPlayer;
}

WorldMap& Game::getWorldMap() const
{
	return *pWorldMap;
}

MenuManager& Game::getMenuManager()
{
	return menuManager;
}
