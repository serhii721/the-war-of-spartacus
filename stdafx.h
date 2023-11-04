// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#if !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
#define AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers

#include <array> // `array` for localization
#include <cstdlib> // `rand()`, `srand()`
#include <ctime> // `time()`
#include <fstream> // `ifstream`, `ofstream`
#include <iostream>
#include <string>
#include <vector>
using namespace std;

#include <direct.h> // Library for creating folders (save games, etc.)
#include <windows.h>
#include <stdio.h>

#include "Enums.h"
#include "LocalizationEnums.h"
#include "Localization.h"
#include "Weapon.h"
#include "Armour.h"
#include "Gladiator.h"
#include "Converter.h"
#include "Constants.h"

const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
extern Localization localization;

string toStringPrecision(double, int = 2);
void output(const string&, int = 15);
void outputError(const string&, int = 4);

Gladiator* createRandomGladiator();
void displayGladiator(const Gladiator&);
void displayMob(const Gladiator&);
void displayMobBatch(const Gladiator*, int);
void createGladiator(Gladiator&);

FightStatus checkFightStatus(const Gladiator&, const Gladiator&);
void outputFightResult(FightStatus, int, int);
void outputOpponentAttackResult(AttackResult, int);
FightStatus fightGladiator(Gladiator&, Gladiator&);

int getWeaponScaleLimit(Weapon::Type, Attribute, Limit);
int getArmourScaleLimit(Armour::Type, Armour::Stat, Limit);
Weapon* createRandomGladiatorWeapon(Weapon::Type = Weapon::NUMBER);
Armour* createRandomGladiatorArmour(Armour::Type = Armour::NUMBER);
void displayWeapon(const Weapon&);
void displayArmour(const Armour&);

bool saveGame(Gladiator&, Gladiator*);
bool loadGame(Gladiator&, Gladiator*);
bool loadPlayer(Gladiator&);
bool loadBots(Gladiator*);

void skipDay(Gladiator&, Gladiator*, int);

bool outputStartMenu(Gladiator&, Gladiator*);
void outputGameMenu(Gladiator&, Gladiator*);

LRESULT CALLBACK WFunc(HWND, UINT, WPARAM, LPARAM);

// Αστεπ.
TCHAR str[256] = "";

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
