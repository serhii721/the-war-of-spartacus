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

#include "Enums.h"
#include "Constants.h"
#include "LocalizationEnums.h"
#include "Localization.h"
#include "Weapon.h"
#include "Armour.h"
#include "Gladiator.h"
#include "Converter.h"

const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
extern Localization localization;

string toStringPrecision(double, int = 2);
void output(const string&, int = 15);
void outputError(const string&, int = 4);

Gladiator* createRandomGladiator();
void displayGladiator(const Gladiator&);
void displayBot(const Gladiator&);
void displayGladiatorBatch(const Gladiator*, int);
void createGladiator(Gladiator&);

FightStatus checkFightStatus(const Gladiator&, const Gladiator&);
void outputFightResult(FightStatus, int, int);
void outputOpponentAttackResult(AttackResult, int);
void fightGladiator(Gladiator&, Gladiator&);

Weapon* createRandomWeapon(WeaponType = WeaponType::NUMBER);
void displayWeapon(const Weapon&);
void displayArmour(const Armour&);

bool saveGame(Gladiator&, Gladiator*);
bool loadGame(Gladiator&, Gladiator*);
bool loadPlayer(Gladiator&);
bool loadBots(Gladiator*);

void skipDay(Gladiator&, Gladiator*, int);

bool outputStartMenu(Gladiator&, Gladiator*);
bool outputGameMenu(Gladiator&, Gladiator*);

#endif //PCH_H
