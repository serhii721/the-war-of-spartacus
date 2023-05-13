// pch.h: This is a precompiled header file.
// Files listed below are compiled only once, improving build performance for future builds.
// This also affects IntelliSense performance, including code completion and many code browsing features.
// However, files listed here are ALL re-compiled if any one of them is updated between builds.
// Do not add files here that you will be updating frequently as this negates the performance advantage.

#ifndef PCH_H
#define PCH_H

#include <array> // `array` for localization
#include <cstdlib> // `rand()`, `srand()`
#include <ctime> // `time()`
#include <fstream> // `ifstream`, `ofstream`
#include <iostream>
#include <string>
using namespace std;

#include <direct.h> // Library for creating folders (save games, etc.)
#include <Windows.h>

#include "LocalizationEnums.h"
#include "Localization.h"
#include "Weapon.h"
#include "Gladiator.h"
#include "Converter.h"

const int OPPONENTS_NUMBER = 3;
const int BASIC_REGEN = 5;
const int BASIC_PLAYER_ATTRIBUTES = 10;
const string OUTPUT_DIVIDER = "\n______________________________________________\n\n\n";

// Gladiator's statistics
const unsigned MIN_AGE = 20, MAX_AGE = 30;
const int BASIC_HEALTH = 100;
const int BASIC_FAME = 0;
const int BASIC_FATIGUE = 0;
const int MIN_STRENGTH = 1, MAX_STRENGTH = 100;
const int MIN_CONSTITUTION = 1, MAX_CONSTITUTION = 100;
const int MIN_DEXTERITY = 1, MAX_DEXTERITY = 100;
const int MIN_INTELLIGENCE = 1, MAX_INTELLIGENCE = 100;
const int MIN_WISDOM = 1, MAX_WISDOM = 100;
const int MIN_CHARISMA = 1, MAX_CHARISMA = 100;

// Weapons' statistics
const int WEAPON_NUMBER = 7; // Number of weapons implemented in game
const int BASIC_DURABILITY = 1000;
const int BASIC_DAMAGE = 1;
const int BASIC_LENGTH = 50; // Centimetres
const int BASIC_WEIGHT = 800; // Grams
const int BASIC_SPEED = 100; // Attack speed

const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
extern Localization localization;

string toStringPrecision(double, int = 2);
void output(string, int = 15);

Gladiator* createRandomGladiator();
void displayGladiator(Gladiator&);
void displayBot(Gladiator&);
void displayGladiatorBatch(Gladiator*, int);
void createGladiator(Gladiator&);
bool gladiatorFight(Gladiator&, Gladiator&);

Weapon* createRandomGladius();	// Sword
Weapon* createRandomSpatha();	// Sword
Weapon* createRandomHasta();	// Spear
Weapon* createRandomFasces();	// Axe
Weapon* createRandomMace();		// Mace
Weapon* createRandomPugio();	// Dagger
Weapon* createRandomPilum();	// Javelin
Weapon* createRandomWeapon();
void displayWeapon(Weapon&);

bool saveGame(Gladiator&, Gladiator*);
// Function `loadGame()` calls other load functions
bool loadGame(Gladiator&, Gladiator*);
bool loadPlayer(Gladiator&);
bool loadBots(Gladiator*);

void skipDay(Gladiator&, Gladiator*, int);

bool outputStartMenu(Gladiator&, Gladiator*);
bool outputGameMenu(Gladiator&, Gladiator*);

#endif //PCH_H
