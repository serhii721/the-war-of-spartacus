#include "stdafx.h"
#include "Game.h"

Game::Game() : pPlayer(make_unique<Player>()), menuManager() { }

Game::Game(const Game& GAME) : pPlayer(make_unique<Player>(*GAME.pPlayer)), menuManager(GAME.menuManager) { }

Game& Game::operator=(const Game& GAME)
{
	if (&GAME == this) return *this;

	if (!pPlayer)
		pPlayer = make_unique<Player>(*GAME.pPlayer);
	else
		*pPlayer = *GAME.pPlayer;

	menuManager = GAME.menuManager;

	return *this;
}

Game::~Game() { }

void Game::setPlayer(const Player& rPlayer)
{
	*pPlayer = rPlayer;
}

Player& Game::getPlayer() const
{
	return *pPlayer;
}

MenuManager& Game::getMenuManager()
{
	return menuManager;
}
