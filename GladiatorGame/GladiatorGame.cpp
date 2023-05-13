// GladiatorGame.cpp : This file contains the 'main' function. Program execution begins and ends there.
/*
 * It is the game about gladiators in the Rome.
 * ...
 */

#include "pch.h"

int main(void)
{
	// Setting the code page for Cyrillic
	SetConsoleCP(1251);
	SetConsoleOutputCP(1251);

	// Setting the random seed
	srand((unsigned)time(0));

	// Loading the user prefered language
	ifstream fin("Data/Settings.conf");
	if (fin) // If user saved prefered language before it will be loaded
	{
		int langOption;
		fin >> langOption;
		switch (langOption)
		{
		default: case 0: setLanguage(Language::ENGLISH);
			break;
		case 1: setLanguage(Language::UKRAINIAN);
			break;
		case 2: setLanguage(Language::RUSSIAN);
			break;
		case 3: setLanguage(Language::LATIN);
		}
	}
	else // If used didn't change language before it will be set to English
		setLanguage();

	Gladiator* player = new Gladiator();

	// OPPONENTS = 3
	Gladiator* bots = new Gladiator[OPPONENTS_NUMBER];

	// 1. Start menu
	if (!outputStartMenu(*player, bots))
	{
		if (player)
			delete player;
		if (bots)
			delete[] bots;

		return 0;
	}

	// 2. Menu in game
	outputGameMenu(*player, bots);

	if (player)
		delete player;
	if (bots)
		delete[] bots;

	return 0;
}
