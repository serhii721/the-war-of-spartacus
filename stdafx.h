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
#include <exception>
using namespace std;

#include <windows.h>
#include <stdio.h>

#include "LocalizationEnums.h"
#include "Localization.h"
#include "Enums.h"
#include "Statistics.h"
#include "Weapon.h"
#include "Armour.h"
#include "Fighter.h"
#include "Leveling.h"
#include "Player.h"
#include "NamedNPC.h"
#include "HarmlessNPC.h"
#include "NPC.h"
#include "Converter.h"
#include "Constants.h"

const HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
extern Localization localization;

string toStringPrecision(double, int = 2);
//void output(const string&, int = 15);
//void outputError(const string&, int = 4);

NPC* generateNPC();
//void displayPlayer(const Player&);
//void displayNPC(const NPC&);
//void displayNPCBatch(const NPC*, int);
Player* createPlayer();

FightStatus checkFightStatus(const Player&, const NPC&);
void outputFightResult(FightStatus, int, int);
void outputOpponentAttackResult(AttackResult, int);
FightStatus fight(Player&, NPC&);

int getWeaponScaleLimit(Weapon::Type, Attribute, Limit);
int getArmourScaleLimit(Armour::Type, Armour::Stat, Limit);
Weapon* generateWeapon(Weapon::Type = Weapon::NUMBER);
Armour* generateArmour(Armour::Type = Armour::NUMBER);
void displayWeapon(const Weapon&);
void displayArmour(const Armour&);

bool saveGame(Player&, NPC*);
bool loadPlayer(Player&);
bool loadNPCs(NPC*);
bool loadGame(Player&, NPC*);

void skipDay(Player&, NPC*, int);

bool outputStartMenu(Player&, NPC*);
void outputGameMenu(Player&, NPC*);

LRESULT CALLBACK WFunc(HWND, UINT, WPARAM, LPARAM);

// Αστεπ.
TCHAR str[256] = "";

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__A9DB83DB_A9FD_11D0_BFD1_444553540000__INCLUDED_)
