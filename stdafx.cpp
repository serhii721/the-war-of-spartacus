// stdafx.cpp : source file that includes just the standard includes
// GladiatorGame.pch will be the pre-compiled header
// stdafx.obj will contain the pre-compiled type information
/*
 * It is the game about gladiators in the Rome.
 * ...
 */

#include "stdafx.h"

// Buffers
TCHAR str[256] = "";
string buf = "";
string logStr = "";

extern HINSTANCE hInst;

Localization l;
Game game;

// __________ Other __________

void updateWindow(HWND hWnd)
{
	//if (!game.isBackgroundChanged())
	//	return;
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top));
	InvalidateRect(hWnd, 0, 1);
	UpdateWindow(hWnd);
	//RedrawWindow(hWnd, 0, 0, RDW_ERASE | RDW_INVALIDATE | RDW_ALLCHILDREN);
}

// TODO: think about move to Converter
string toStringPrecision(double n, int precision)
{
	string s = to_string(n);
	if (precision > 0)
		return s.substr(0, s.find(".") + 1 + precision);
	else if (precision == 0)
		return s.substr(0, s.find("."));
	return "";
}

//void output(const string& s, int color)
//{
//	SetConsoleTextAttribute(hConsole, color);
//	cout << s;
//}

//void outputError(const string& s, int color)
//{
//	SetConsoleTextAttribute(hConsole, color);
//	cerr << s;
//}

// __________ NPC __________

unique_ptr<NPC> generateNPC(int aproximateLevel)
{
	// 1. Generating name
	const string PATH_BASE = "Data/Language/Names/", FORMAT = ".lang";
	ifstream fin;
	// Open file containing roman first names
	switch (l.getLanguage())
	{
	default: case Language::ENGLISH: fin.open(PATH_BASE + "En_FirstNames" + FORMAT); break;
	case Language::UKRAINIAN: fin.open(PATH_BASE + "Uk_FirstNames" + FORMAT); break;
	case Language::RUSSIAN: fin.open(PATH_BASE + "Ru_FirstNames" + FORMAT); break;
	}
	//if (!fin)
	//	output(localization[Localized::CREATION_NAME_LOAD_ERROR], 4);

	int count = 0;
	string line;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomFirstName = rand() % count; // Choose random line
	fin.close();

	// Open file containing roman last names
	switch (l.getLanguage())
	{
	default: case Language::ENGLISH: fin.open(PATH_BASE + "En_LastNames" + FORMAT); break;
	case Language::UKRAINIAN: fin.open(PATH_BASE + "Uk_LastNames" + FORMAT); break;
	case Language::RUSSIAN: fin.open(PATH_BASE + "Ru_LastNames" + FORMAT); break;
	}
	//if (!fin)
		//output(localization[Localized::CREATION_NAME_LOAD_ERROR], 4);

	count = 0;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomLastName = rand() % count; // Choose random line
	fin.close();

	// 2. Generating weapons
	unique_ptr<Weapon> rightHand = generateWeapon();
	unique_ptr<Weapon> leftHand;
	if (rightHand->getWeaponType() != Weapon::WeaponType::AXE && rightHand->getWeaponType() != Weapon::WeaponType::SPEAR)
	{
		leftHand = generateWeapon();
		if (!rightHand->isCompatibleWith(leftHand->getWeaponType()))
		{
			if (rand() % 100 < 75)
			{
				if (leftHand)
					leftHand.release();
				leftHand = generateWeapon(Weapon::SHIELD);
			}
			else if (rand() % 100 < 75)
				do
				{
					if (leftHand)
						leftHand.release();
					leftHand = generateWeapon();
				} while (!rightHand->isCompatibleWith(leftHand->getWeaponType()));
			else if (leftHand)
			{
				leftHand.release();
				leftHand = nullptr;
			}
		}
	}
	else
		leftHand = nullptr;

	// 3. Generating stats
	// Generating level in range (aproximate level - 5) to (aproximate level + 5)
	int level = rand() % 11 + aproximateLevel - 5;

	if (level < MIN_LEVEL)
		level = MIN_LEVEL;
	else if (level > MAX_LEVEL)
		level = MAX_LEVEL;

	// Calculating experience for current level
	Leveling leveling(level);
	leveling.setExperience(leveling.calculateExperienceForLevel(leveling.getLevel()));

	// Calculating unnassigned attributes for current level
	int unnassignedAttributes = level * ATTRIBUTES_PER_LEVEL;
	if (level > 9)
		unnassignedAttributes += 10;
	else
		unnassignedAttributes += level;

	// Randomly distributing attribute points
	vector<int> attributes(6, BASIC_ATTRIBUTES);
	int points;
	while (unnassignedAttributes > 0)
	{
		for (int i = 0; i < 6; i++)
		{
			if (unnassignedAttributes > MAX_ATTRIBUTE - BASIC_ATTRIBUTES)
				points = rand() % (MAX_ATTRIBUTE - BASIC_ATTRIBUTES + 1);
			else
				points = rand() % (unnassignedAttributes + 1);
			points /= 2;
			points++;
			if (attributes[i] + points < MAX_ATTRIBUTE)
			{
				attributes[i] += points;
				unnassignedAttributes -= points;
			}
			if (unnassignedAttributes == 0)
				break;
		}
	}

	// Updating weapon damage depending on stat scale
	rightHand->update(attributes[0], attributes[2]);
	if (leftHand)
		leftHand->update(attributes[0], attributes[2]);

	// Armour
	unique_ptr<Armour> armour = generateArmour();

	// Updating armour defense depending on stat scale
	armour->update(attributes[0], attributes[2]);

	NPC npc(
		Fighter(
			Statistics(
				attributes[0], // Strength
				attributes[1], // Constitution
				attributes[2], // Dexterity
				attributes[3], // Intelligence
				attributes[4], // Wisdom
				attributes[5], // Charisma
				rand() % (MAX_AGE - MIN_AGE) + MIN_AGE,
				BASIC_FAME * level + rand() % 200
			),
			BASIC_HP,
			BASIC_HP,
			move(rightHand),
			move(leftHand),
			move(armour)
		),
		NamedNPC(
			randomFirstName,
			randomLastName
		),
		leveling
	);
	npc.updateMaxHP();

	return make_unique<NPC>(npc);
}

//void displayPlayer(const Player& p)
//{
//	output(localization[Localized::STATUS_NAME] + ' ');
//	output(g.name + '\n', 14);
//	output(localization[Localized::STATUS_AGE] + ' ');
//	output(to_string(g.age) + '\n', 14);
//	output(localization[Localized::STATUS_HP] + ' ');
//	output(to_string(g.getHP()) + '\n', 12);
//	output(localization[Localized::STATUS_FAME] + ' ');
//	output(to_string(g.fame) + '\n', 6);
//	output(localization[Localized::STATUS_STRENGTH] + ' ');
//	output(to_string(g.strength) + '\n', 4);
//	output(localization[Localized::STATUS_CONSTITUTION] + ' ');
//	output(to_string(g.constitution) + '\n', 12);
//	output(localization[Localized::STATUS_DEXTERITY] + ' ');
//	output(to_string(g.dexterity) + '\n', 2);
//	output(localization[Localized::STATUS_INTELLIGENCE] + ' ');
//	output(to_string(g.intelligence) + '\n', 1);
//	output(localization[Localized::STATUS_WISDOM] + ' ');
//	output(to_string(g.wisdom) + '\n', 13);
//	output(localization[Localized::STATUS_CHARISMA] + ' ');
//	output(to_string(g.charisma), 6);
//}

//void displayNPC(const NPC& rNPC)
//{
//	displayPlayer(rMob); // TODO: fix
//	output("\n\n");
//
//	// Weapons
//	if (rMob.isRightHandOccupied())
//	{
//		output(localization[Localized::STATUS_WEAPON] + "\n\n", 14);
//		displayWeapon(*rMob.rightHand);
//		output("\n");
//
//		if (rMob.isLeftHandOccupied())
//		{
//			displayWeapon(*rMob.leftHand);
//			output("\n\n");
//		}
//	}
//	else if (rMob.isLeftHandOccupied())
//	{
//		output(localization[Localized::STATUS_WEAPON] + "\n\n", 14);
//		displayWeapon(*rMob.leftHand);
//		output("\n\n");
//	}
//
//	if (rMob.isArmourEquipped())
//	{
//		output("Armour:" + string("\n\n"), 14);
//		displayArmour(*rMob.armour);
//		output("\n\n");
//	}
//}

//void displayNPCBatch(const NPC* npcs, int number)
//{
//	for (int i = 0; i < number; i++)
//	{
//		output(to_string(i + 1) + ' ' + localization[Localized::STATUS_NPC] + "\n\n", 4);
//		displayNPC(npcs[i]);
//	}
//}

//Player* createPlayer()
//{
//	int strength = BASIC_PLAYER_ATTRIBUTES,
//		constitution = BASIC_PLAYER_ATTRIBUTES,
//		dexterity = BASIC_PLAYER_ATTRIBUTES,
//		intelligence = BASIC_PLAYER_ATTRIBUTES,
//		wisdom = BASIC_PLAYER_ATTRIBUTES,
//		charisma = BASIC_PLAYER_ATTRIBUTES;
//
//	bool characterCreation = true;
//	string name;
//	int age;
//	int option;
//	while (characterCreation)
//	{
//		int attributes = 30;
//
//		//output(localization[Localized::CREATION_CREATE_CHARACTER] + "\n\n");
//
//		// Name
//		// TODO: think about regex
//		//output(localization[Localized::CREATION_INPUT_NAME] + ' ', 15);
//		//SetConsoleTextAttribute(hConsole, 14);
//		//getline(cin, player.name);
//
//		// Age
//		// TODO: if min max
//		//output(localization[Localized::CREATION_INPUT_AGE] + ' ');
//		//SetConsoleTextAttribute(hConsole, 14);
//		//cin >> player.age;
//
//		// Input of player's attributes
//		//bool setAttributes = true,
//		//	setStrength = true,
//		//	setConstitution = true,
//		//	setDexterity = true,
//		//	setIntelligence = true,
//		//	setWisdom = true,
//		//	setCharisma = true;
//		//while (
//		//	setAttributes &&
//		//		(setStrength || setConstitution || setDexterity || setIntelligence || setWisdom || setCharisma)
//		//	) {
//		//	// Attribute points' instruction
//		//	output("\n");
//		//	output(localization[Localized::CREATION_INPUT_POINTS_INSTRUCTION] + "\n");
//		//	output(localization[Localized::STRENGTH] + ' ', 4);
//		//	output(localization[Localized::CONSTITUTION] + ' ', 12);
//		//	output(localization[Localized::DEXTERITY] + ' ', 2);
//		//	output(localization[Localized::INTELLIGENCE] + ' ', 1);
//		//	output(localization[Localized::WISDOM] + ' ', 13);
//		//	output(localization[Localized::CHARISMA] + '\n', 6);
//		//	output(localization[Localized::CREATION_INPUT_POINTS_INSTRUCTION2] + "\n\n");
//
//		//	// Strength
//		//	while (setAttributes && setStrength)
//		//	{
//		//		output("\n");
//		//		output(localization[Localized::CREATION_INPUT_POINTS]);
//		//		output(" " + to_string(attributes) + "\n", 10);
//		//		output(localization[Localized::CREATION_INPUT_STRENGTH] + ' ');
//		//		SetConsoleTextAttribute(hConsole, 4);
//		//		cin >> player.strength;
//		//		if (player.strength + 1 < MIN_STRENGTH || player.strength > MAX_STRENGTH)
//		//			output(localization[Localized::WRONG_INPUT] + '\n', 4);
//		//		else
//		//		{
//		//			player.strength += BASIC_PLAYER_ATTRIBUTES;
//		//			if (attributes - (player.strength - 10) < 0)
//		//				output(localization[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
//		//			else
//		//			{
//		//				attributes -= player.strength - 10;
//		//				setStrength = false;
//		//			}
//		//		}
//		//	}
//		//	if (!attributes)
//		//		setAttributes = false;
//
//		//	// Constitution
//		//	while (setAttributes && setConstitution)
//		//	{
//		//		output("\n");
//		//		output(localization[Localized::CREATION_INPUT_POINTS]);
//		//		output(" " + to_string(attributes) + "\n", 10);
//		//		output(localization[Localized::CREATION_INPUT_CONSTITUTION] + ' ');
//		//		SetConsoleTextAttribute(hConsole, 12);
//		//		cin >> player.constitution;
//		//		if (player.constitution + 1 < MIN_CONSTITUTION || player.constitution > MAX_CONSTITUTION)
//		//			output(localization[Localized::WRONG_INPUT] + '\n', 4);
//		//		else
//		//		{
//		//			player.constitution += BASIC_PLAYER_ATTRIBUTES;
//		//			if (attributes - (player.constitution - 10) < 0)
//		//				output(localization[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
//		//			else
//		//			{
//		//				attributes -= player.constitution - 10;
//		//				setConstitution = false;
//		//			}
//		//		}
//		//	}
//		//	if (!attributes)
//		//		setAttributes = false;
//
//		//	// Dexterity
//		//	while (setAttributes && setDexterity)
//		//	{
//		//		output("\n");
//		//		output(localization[Localized::CREATION_INPUT_POINTS]);
//		//		output(" " + to_string(attributes) + "\n", 10);
//		//		output(localization[Localized::CREATION_INPUT_DEXTERITY] + ' ');
//		//		SetConsoleTextAttribute(hConsole, 2);
//		//		cin >> player.dexterity;
//		//		if (player.dexterity + 1 < MIN_DEXTERITY || player.dexterity > MAX_DEXTERITY)
//		//			output(localization[Localized::WRONG_INPUT] + '\n', 4);
//		//		else
//		//		{
//		//			player.dexterity += BASIC_PLAYER_ATTRIBUTES;
//		//			if (attributes - (player.dexterity - 10) < 0)
//		//				output(localization[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
//		//			else
//		//			{
//		//				attributes -= player.dexterity - 10;
//		//				setDexterity = false;
//		//			}
//		//		}
//		//	}
//		//	if (!attributes)
//		//		setAttributes = false;
//
//		//	// Intelligence
//		//	while (setAttributes && setIntelligence)
//		//	{
//		//		output("\n");
//		//		output(localization[Localized::CREATION_INPUT_POINTS]);
//		//		output(" " + to_string(attributes) + "\n", 10);
//		//		output(localization[Localized::CREATION_INPUT_INTELLIGENCE] + ' ');
//		//		SetConsoleTextAttribute(hConsole, 1);
//		//		cin >> player.intelligence;
//		//		if (player.intelligence + 1 < MIN_INTELLIGENCE || player.intelligence > MAX_INTELLIGENCE)
//		//			output(localization[Localized::WRONG_INPUT] + '\n', 4);
//		//		else
//		//		{
//		//			player.intelligence += BASIC_PLAYER_ATTRIBUTES;
//		//			if (attributes - (player.intelligence - 10) < 0)
//		//				output(localization[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
//		//			else
//		//			{
//		//				attributes -= player.intelligence - 10;
//		//				setIntelligence = false;
//		//			}
//		//		}
//		//	}
//		//	if (!attributes)
//		//		setAttributes = false;
//
//		//	// Wisdom
//		//	while (setAttributes && setWisdom)
//		//	{
//		//		output("\n");
//		//		output(localization[Localized::CREATION_INPUT_POINTS]);
//		//		output(" " + to_string(attributes) + "\n", 10);
//		//		output(localization[Localized::CREATION_INPUT_WISDOM] + ' ');
//		//		SetConsoleTextAttribute(hConsole, 13);
//		//		cin >> player.wisdom;
//		//		if (player.wisdom + 1 < MIN_WISDOM || player.wisdom > MAX_WISDOM)
//		//			output(localization[Localized::WRONG_INPUT] + '\n', 4);
//		//		else
//		//		{
//		//			player.wisdom += BASIC_PLAYER_ATTRIBUTES;
//		//			if (attributes - (player.wisdom - 10) < 0)
//		//				output(localization[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
//		//			else
//		//			{
//		//				attributes -= player.wisdom - 10;
//		//				setWisdom = false;
//		//			}
//		//		}
//		//	}
//		//	if (!attributes)
//		//		setAttributes = false;
//
//		//	// Charisma
//		//	while (setAttributes && setCharisma)
//		//	{
//		//		output("\n");
//		//		output(localization[Localized::CREATION_INPUT_POINTS]);
//		//		output(" " + to_string(attributes) + "\n", 10);
//		//		output(localization[Localized::CREATION_INPUT_CHARISMA] + ' ');
//		//		SetConsoleTextAttribute(hConsole, 13);
//		//		cin >> player.charisma;
//		//		if (player.charisma + 1 < MIN_CHARISMA || player.charisma > MAX_CHARISMA)
//		//			output(localization[Localized::WRONG_INPUT] + '\n', 4);
//		//		else
//		//		{
//		//			player.charisma += BASIC_PLAYER_ATTRIBUTES;
//		//			if (attributes - (player.charisma - 10) < 0)
//		//				output(localization[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
//		//			else
//		//			{
//		//				attributes -= player.charisma - 10;
//		//				setCharisma = false;
//		//			}
//		//		}
//		//	}
//		//	if (!attributes)
//		//		setAttributes = false;
//		//}
//
//		// Confirming player's parameters
//	//	bool notConfirmed = true;
//	//	while (notConfirmed)
//	//	{
//	//		// Show created character
//	//		output(OUTPUT_DIVIDER + '\n' + localization[Localized::STATUS_NPC] + "\n\n");
//	//		output(localization[Localized::Message::STATUS_NAME] + ' ');
//	//		output(player.name + '\n', 14);
//	//		output(localization[Localized::STATUS_AGE] + ' ');
//	//		output(to_string(player.age) + '\n', 14);
//	//		output(localization[Localized::STATUS_STRENGTH] + ' ');
//	//		output(to_string(player.strength) + '\n', 4);
//	//		output(localization[Localized::STATUS_CONSTITUTION] + ' ');
//	//		output(to_string(player.constitution) + '\n', 12);
//	//		output(localization[Localized::STATUS_DEXTERITY] + ' ');
//	//		output(to_string(player.dexterity) + '\n', 2);
//	//		output(localization[Localized::STATUS_INTELLIGENCE] + ' ');
//	//		output(to_string(player.intelligence) + '\n', 1);
//	//		output(localization[Localized::STATUS_WISDOM] + ' ');
//	//		output(to_string(player.wisdom) + '\n', 13);
//	//		output(localization[Localized::STATUS_CHARISMA] + ' ');
//	//		output(to_string(player.charisma) + "\n\n", 6);
//
//	//		if (attributes > 0)
//	//		{
//	//			output(localization[Localized::CREATION_INPUT_POINTS] + ' ');
//	//			output(to_string(attributes) + "\n\n", 10);
//	//		}
//	//		output(localization[Localized::CREATION_INPUT_CORRECT] + '\n');
//	//		output("1. " + localization[Localized::YES] + '\n');
//	//		output("2. " + localization[Localized::NO] + "\n\n");
//	//		output(localization[Localized::CHOOSE_OPTION] + ' ');
//	//		cin >> option;
//	//		cin.get();
//	//		switch (option)
//	//		{
//	//		case 1:
//	//			characterCreation = false;
//	//			notConfirmed = false;
//	//			break;
//	//		case 2:
//	//			characterCreation = true;
//	//			notConfirmed = false;
//	//			output(OUTPUT_DIVIDER);
//	//			break;
//	//		default:
//	//			output("\n");
//	//			output(localization[Localized::WRONG_INPUT] + "\n", 4);
//	//			notConfirmed = true;
//	//			break;
//	//		}
//	//	}
//	}
//
//	// Generating weapons
//	Weapon* rightHand = generateWeapon();
//	Weapon* leftHand = generateWeapon();
//	if (!rightHand->isCompatibleWith(leftHand->getType()))
//	{
//		if (rand() % 100 < 75)
//		{
//			if (leftHand)
//				delete leftHand;
//			leftHand = generateWeapon(Weapon::SHIELD);
//		}
//		else if (rand() % 100 < 75)
//			do
//			{
//				if (leftHand)
//					delete leftHand;
//				leftHand = generateWeapon();
//			} while (!rightHand->isCompatibleWith(leftHand->getType()));
//		else if (leftHand)
//		{
//			delete leftHand;
//			leftHand = nullptr;
//		}
//	}
//
//	//output("\n");
//	//output(localization[Localized::CREATION_INPUT_SUCCESS] + '\n', 10);
//
//	return new Player(
//		Fighter(
//			Statistics(
//				strength,
//				constitution,
//				dexterity,
//				intelligence,
//				wisdom,
//				charisma,
//				age,
//				BASIC_FAME
//			),
//			BASIC_HP, // HP
//			BASIC_HP, // Full HP
//			rightHand,
//			leftHand,
//			generateArmour()
//		),
//		MIN_LEVEL,
//		name
//	);
//}

//bool gladiatorFight(Gladiator& player, Gladiator& bot)
//{
//	// Calculating damage (weapon damage * (strength scale + dexterity scale)). Max scale from 100 attributes is 70% increase
//	int playerDamage = player.weapon->damage * ((1 + player.strength / 140) + (1 + player.dexterity / 140) - 1);
//	int botDamage = bot.weapon->damage * ((1 + bot.strength / 140) + (1 + bot.dexterity / 140) - 1);
//
//	// Calculating evasion chance from dexterity. Min 5% chance to max 35% chance from 100 attribute
//	int playerEvasion = player.dexterity * 0.3030 + 5;
//	int botEvasion = bot.dexterity * 0.3030 + 5;
//
//	// Calculating bash chance from strength. Min 3% chance to max 20% chance from 100 attribute
//	int playerBash = player.strength * 0.17 + 3;
//	int botBash = bot.strength * 0.17 + 3;
//	bool playerStun = false, npcStun = false;
//
//	output(localization[Localized::FIGHT_START] + "\n\n", 4);
//
//	while (player.health > 0 && bot.health > 0) // Fight until both opponents have health
//	{
//		if (!playerStun) // If player is not bashed he attacks
//		{
//			output(localization[Localized::FIGHT_PLAYER_ATTACKS] + '\n');
//			Sleep(200);
//			if (1 + rand() / (RAND_MAX / (100 - 1)) > botEvasion) // Check bot evasion chance
//			{
//				output(localization[Localized::FIGHT_PLAYER_HITS] + '\n', 2);
//				bot.health -= playerDamage;
//				if (1 + rand() / (RAND_MAX / (100 - 1)) < playerBash) // Check player chance to bash
//				{
//					npcStun = true;
//					Sleep(100);
//					output(localization[Localized::FIGHT_ENEMY_STUNNED] + '\n', 2);
//				}
//			}
//			else
//				output(localization[Localized::FIGHT_ENEMY_EVADES] + '\n', 12);
//		}
//		else
//			playerStun = false;
//
//		Sleep(500);
//
//		if (bot.health < 1) // If bot is alive fight continues
//			break;
//
//		output("\n\n");
//
//		if (!npcStun) // If bot is not bashed he attacks
//		{
//			output(localization[Localized::FIGHT_ENEMY_ATTACKS] + '\n');
//			Sleep(200);
//			if (1 + rand() / (RAND_MAX / (100 - 1)) > playerEvasion) // Check player evasion chance
//			{
//				output(localization[Localized::FIGHT_ENEMY_HITS] + '\n', 12);
//				player.health -= botDamage;
//				if (1 + rand() / (RAND_MAX / (100 - 1)) < botBash) // Check bot chance to bash
//				{
//					playerStun = true;
//					Sleep(100);
//					output(localization[Localized::FIGHT_PLAYER_STUNNED] + '\n', 12);
//				}
//			}
//			else
//				output(localization[Localized::FIGHT_PLAYER_EVADES] + '\n', 2);
//		}
//		else
//			npcStun = false;
//
//		Sleep(500);
//
//		output("\n\n");
//
//		if (player.health < 1) // If player is alive fight continues
//			break;
//
//		// Display player's HP
//		output(localization[Localized::FIGHT_PLAYER_HP] + ' ');
//		output(to_string(player.health) + '\n', 10);
//		// Display enemy's HP
//		output(localization[Localized::FIGHT_ENEMY_HP] + ' ');
//		output(to_string(bot.health) + "\n\n", 4);
//
//		Sleep(500);
//	}
//
//	if (player.health < 1) // Player lost
//	{
//		player.health = 1;
//
//		output(localization[Localized::FIGHT_ENEMY_LAST_HIT] + "\n\n", 4);
//		output(localization[Localized::FIGHT_PLAYER_HP] + ' ');
//		output(to_string(player.health) + "\n", 10);
//		output(localization[Localized::FIGHT_ENEMY_HP] + ' ');
//		output(to_string(bot.health) + "\n\n", 4);
//		return false;
//	}
//	else // Player won
//	{
//		bot.health = 1;
//
//		output(localization[Localized::FIGHT_PLAYER_LAST_HIT] + "\n\n", 10);
//		output(localization[Localized::FIGHT_PLAYER_HP] + ' ');
//		output(to_string(player.health) + "\n\n", 10);
//		return true;
//	}
//
//}

// __________ Weapon and Armour __________

int getWeaponScaleLimit(Weapon::WeaponType ttype, Attribute aattribute, Limit llimit)
{
	switch (ttype)
	{
	case Weapon::AXE:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 32;
			case Limit::MAX: return 160;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 6;
			case Limit::MAX: return 30;
			}
			break;
		}
		break;

	case Weapon::DAGGER:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 3;
			case Limit::MAX: return 15;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 17;
			case Limit::MAX: return 85;
			}
			break;
		}
		break;

	case Weapon::MACE:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 15;
			case Limit::MAX: return 75;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 5;
			case Limit::MAX: return 25;
			}
			break;
		}
		break;

	case Weapon::SPEAR:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 10;
			case Limit::MAX: return 50;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 30;
			case Limit::MAX: return 150;
			}
			break;
		}
		break;

	case Weapon::SWORD:
		switch (aattribute)
		{
		case Attribute::STRENGTH:
			switch (llimit)
			{
			case Limit::MIN: return 10;
			case Limit::MAX: return 50;
			}
			break;
		case Attribute::DEXTERITY:
			switch (llimit)
			{
			case Limit::MIN: return 10;
			case Limit::MAX: return 50;
			}
			break;
		}
		break;

	default: break;
	}
	//outputError("Unknown weapon type when getting its scale limit!\n");
	return -1;
}

int getArmourScaleLimit(Armour::ArmourType ttype, Armour::Stat sstat, Limit llimit)
{
	switch (ttype)
	{
	case Armour::LIGHT:
		switch (sstat)
		{
		case Armour::STR_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 5;
			case Limit::MAX: return 25;
			}
			break;
		case Armour::DEX_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 15;
			case Limit::MAX: return 75;
			}
			break;
		case Armour::EVASION_PROB_ADDITION:
			switch (llimit)
			{
			case Limit::MIN: return 1;
			case Limit::MAX: return 10;
			}
			break;
		case Armour::STUN_PROB_SUBSTRACTION:
			switch (llimit) { case Limit::MIN: case Limit::MAX: return 0; }
			break;
		}
		break;

	case Armour::HEAVY:
		switch (sstat)
		{
		case Armour::STR_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 15;
			case Limit::MAX: return 75;
			}
			break;
		case Armour::DEX_ADDITION_PERC:
			switch (llimit)
			{
			case Limit::MIN: return 5;
			case Limit::MAX: return 25;
			}
			break;
		case Armour::EVASION_PROB_ADDITION:
			switch (llimit) { case Limit::MIN: case Limit::MAX: return 0; }
			break;
		case Armour::STUN_PROB_SUBSTRACTION:
			switch (llimit)
			{
			case Limit::MIN: return 1;
			case Limit::MAX: return 4;
			}
			break;
		}
		break;

	default: break;
	}

	//outputError("Unknown weapon type when getting its scale limit!\n");
	return -1;
}

unique_ptr<Weapon> generateWeapon(Weapon::WeaponType ttype)
{
	if (ttype != Weapon::SHIELD)
	{
		Weapon::WeaponType newWeaponType = ttype != Weapon::NUMBER ? ttype : Weapon::WeaponType(rand() % (Weapon::NUMBER - 1));

		int handsNeededForWeapon = 1;
		if (newWeaponType == Weapon::WeaponType::AXE || newWeaponType == Weapon::WeaponType::SPEAR)
			handsNeededForWeapon = 2;

		int maxStrAdditionPerc = getWeaponScaleLimit(newWeaponType, Attribute::STRENGTH, Limit::MAX),
			maxDexAdditionPerc = getWeaponScaleLimit(newWeaponType, Attribute::DEXTERITY, Limit::MAX);

		return make_unique<Weapon>(
			Item(
				Item::ItemType::WEAPON,
				BASIC_ITEM_VALUE // TODO: Calculate value
			),
			(MIN_WEAPON_DAMAGE + rand() % WEAPON_RAND_DAM_ADDITION) * handsNeededForWeapon, // Damage
			// `Weapon::NUMBER - 1` is a shield
			newWeaponType, // Type
			0, // Damage addition
			maxStrAdditionPerc * 3 / 5 + (rand() % (maxStrAdditionPerc / 10 + 1)), // Strength damage addition
			maxDexAdditionPerc * 3 / 5 + (rand() % (maxDexAdditionPerc / 10 + 1)), // Dexterity damage addition
			0, // Shield's probability addition
			0, // Shield's defense percent addition
			"" // Name
		);
	}
	else // A shield only created manually
		return make_unique<Weapon>(
			Item(
				Item::ItemType::WEAPON,
				BASIC_ITEM_VALUE // TODO: Calculate value
			),
			0, // Damage
			Weapon::SHIELD,
			0, // Damage addition
			0, // Strength percent addition
			0, // Dexterity percent addition
			MIN_SHIELD_PROB_ADDITION + rand() % SHIELD_RAND_PROB_ADDITION, // Shield's probability addition
			MIN_SHIELD_DEF_PERC_ADDITION + rand() % SHIELD_RAND_DEF_PERC_ADDITION, // Shield's defense percent addition
			"" // Name
		);
}

unique_ptr<Armour> generateArmour(Armour::ArmourType ttype)
{
	// Determining the type
	Armour::ArmourType type = ttype != Armour::NUMBER ? ttype : Armour::ArmourType(rand() % Armour::NUMBER);

	// Calculating the armour statistics based on its type
	int maxStrDefAddition = getArmourScaleLimit(type, Armour::STR_ADDITION_PERC, Limit::MAX),
		maxDexDefAddition = getArmourScaleLimit(type, Armour::DEX_ADDITION_PERC, Limit::MAX);
	int evasionProbAddition, stunProbSubstraction;
	switch (type)
	{
	case Armour::LIGHT:
		evasionProbAddition = getArmourScaleLimit(type, Armour::EVASION_PROB_ADDITION, Limit::MAX) * 2 / 5 + rand() % 3;
		stunProbSubstraction = 0;
		break;
	case Armour::HEAVY:
		evasionProbAddition = 0;
		stunProbSubstraction = getArmourScaleLimit(type, Armour::STUN_PROB_SUBSTRACTION, Limit::MAX) / 2;
		break;
	}

	return make_unique<Armour>(
		Item(
			Item::ItemType::ARMOUR,
			BASIC_ITEM_VALUE // TODO: Calculate value
		),
		MIN_ARMOUR_DEFENSE + 5 + rand() % ARMOUR_RAND_DEF_ADDITION, // Defense
		type,
		0, // Defense addition
		maxStrDefAddition * 3 / 5 + (rand() % maxStrDefAddition / 10 + 1), // Strength defense addition
		maxDexDefAddition * 3 / 5 + (rand() % maxDexDefAddition / 10 + 1), // Dexterity defense addition
		evasionProbAddition,
		stunProbSubstraction
	);
}

//void displayWeapon(const Weapon& rWeapon)
//{
//	//if (rWeapon.getName() != "")
//	//	output("Name: " + rWeapon.getName() + '\n');
//
//	//output(localization[Localized::WEAPON_STATUS_TYPE] + ' ');
//	string typeName;
//	switch (rWeapon.getType())
//	{
//	case Weapon::SWORD: typeName = "Sword"; break;
//	case Weapon::SPEAR: typeName = "Spear"; break;
//	case Weapon::DAGGER: typeName = "Dagger"; break;
//	case Weapon::AXE: typeName = "Axe"; break;
//	case Weapon::MACE: typeName = "Mace"; break;
//	default: /*outputError("Unknown weapon type!\n");*/ break;
//	}
//	//output(typeName + '\n', 14);
//
//	//output(localization[Localized::WEAPON_STATUS_DAMAGE] + ' ');
//	//output(to_string(rWeapon.getDamage()) + '\n', 4);
//}
//
//void displayArmour(const Armour& rArmour)
//{
//	string typeName;
//	switch (rArmour.getType())
//	{
//	case Armour::LIGHT: typeName = "Light"; break;
//	case Armour::HEAVY: typeName = "Heavy"; break;
//	default: /*outputError("Unknown armour type!\n");*/ break;
//	}
//	//output("Type: " + typeName + '\n');
//	//output("Defense: " + to_string(rArmour.getTotalDefense()));
//}

// __________ Save and Load __________

//bool saveGame(Player& player, NPC* npcs)
//{
//	const string DIR_NAME = "Saves/", FILE_PLAYER = "Player", FILE_OPPONENTS = "Opponents", FORMAT = ".save";
//
//	// Creating the folder of saved games if it doesn't exist
//	BOOL success = CreateDirectory(DIR_NAME.c_str(), NULL);
//	if (!success && GetLastError() != ERROR_ALREADY_EXISTS)
//		return false;
//
//	// Open file for saving player's character and empty it
//	ofstream fout(DIR_NAME + FILE_PLAYER + FORMAT, ios::binary);
//	// Check if file is open
//	if (fout)
//	{
//		// TODO: serialization
//		fout.close();
//	}
//	else // If save file couldn't be opened - save unsucessful
//		return false;
//
//	// Open file for saving enemies and empty it
//	fout.open(DIR_NAME + FILE_OPPONENTS + FORMAT, ios::binary);
//	// Check if file is open
//	if (fout)
//	{
//		// Save NPCs
//		for (int i = 0; i < OPPONENTS_NUMBER; i++)
//		{
//			// TODO: serialization
//		}
//		// Close save files
//		fout.close();
//		return true;
//	}
//	// If save file couldn't be opened - save unsucessful
//	return false;
//}

//bool loadPlayer(Player& player)
//{
//	// Open file with player's character
//	const string DIR_NAME = "Saves/", FILE_NAME = "Player", FORMAT = ".save";
//
//	// Check if save file is open and not empty
//	ifstream fin(DIR_NAME + FILE_NAME + FORMAT, ios::binary);
//	if (fin)
//	{
//		// TODO: structurization
//		fin.close();
//
//		// Load sucessful
//		return true;
//	}
//
//	fin.close();
//	// Load unsucessful
//	return false;
//}

//bool loadNPCs(NPC* npcs)
//{
//	// Open file for loading enemies
//	const string DIR_NAME = "Saves/", FILE_NAME = "Opponents", FORMAT = ".save";
//	ifstream fin(DIR_NAME + FILE_NAME + FORMAT, ios::binary);
//
//	// Check if save file is open and not empty
//	if (fin)
//	{
//		for (int i = 0; i < OPPONENTS_NUMBER; i++)
//		{
//			// TODO: structurization
//		}
//		fin.close();
//
//		// Load sucessful
//		return true;
//	}
//
//	// Load unsucessful
//	return false;
//}

//bool loadGame(Player& player, NPC* npcs)
//{
//	if (loadPlayer(player) && loadNPCs(npcs))
//		return true;
//	return false;
//}

// __________ Game process __________

//void skipDay(Player& player, NPC* npcs, int n)
//{
//	//output(localization[Localized::TIME_WAITED] + "\n\n");
//
//	// NPCs regen HP
//	for (int i = 0; i < n; i++)
//	{
//		if (npcs[i].getHP() < npcs[i].getFullHP())
//			npcs[i].setHP(npcs[i].getHP() + BASIC_REGEN);
//		if (npcs[i].getHP() > npcs[i].getFullHP())
//			npcs[i].setHP(npcs[i].getFullHP());
//	}
//
//	// Player regen HP
//	if (player.getHP() < player.getFullHP())
//	{
//		player.setHP(player.getHP() + BASIC_REGEN);
//		if (player.getHP() > player.getFullHP())
//		{
//			player.setHP(player.getFullHP());
//			//output(localization[Localized::TIME_FULL_HP] + '\n', 10);
//		}
//		else
//		{
//			//output(localization[Localized::TIME_REGEN] + ' ');
//			//output(to_string(BASIC_REGEN) + '\n', 10);
//			//output(localization[Localized::TIME_CURRENT_HP] + ' ');
//			//output(to_string(player.getHP()) + '\n', 10);
//		}
//	}
//	//else
//		//output(localization[Localized::TIME_NOTHING] + '\n');
//
//	//output(OUTPUT_DIVIDER);
//}

//bool outputStartMenu(Player& player, NPC* npcs)
//{
//	int option;
//	Language langOption;
//	ofstream fout;
//	bool languageChanged = false;
//	//output("\n\nGladiator Game\n", 4);
//	//output(OUTPUT_DIVIDER);
//	while (true)
//	{
//		//output(
//		//	"1. " + localization[Localized::START_MENU_START_GAME] + '\n' +
//		//	"2. " + localization[Localized::START_MENU_LOAD_GAME] + '\n' +
//		//	"3. " + localization[Localized::START_MENU_CHANGE_LANGUAGE] + '\n' +
//		//	"4. " + localization[Localized::EXIT] + "\n\n" +
//		//	localization[Localized::CHOOSE_OPTION] + ' '
//		//);
//		//cin >> option;
//		//cin.get();
//		//output(OUTPUT_DIVIDER);
//
//		switch (option)
//		{
//		case 1: // New game
//			player = *createPlayer();
//			for (int i = 0; i < OPPONENTS_NUMBER; i++)
//				npcs[i] = *generateNPC();
//			//output(OUTPUT_DIVIDER);
//			return true;
//
//		case 2: // Load game
//			if (loadGame(player, npcs))
//			{
//				//output(localization[Localized::START_MENU_LOAD_GAME_SUCCESS] + "\n", 10);
//				//output(OUTPUT_DIVIDER);
//				return true;
//			}
//			else
//			{
//				//output(localization[Localized::START_MENU_LOAD_GAME_ERROR] + "\n", 12);
//				//output(OUTPUT_DIVIDER);
//				break;
//			}
//
//		case 3: // Change language
//			while (!languageChanged)
//			{
//				//output(
//				//	"1. English\n" +
//				//	string("2. Українська\n") +
//				//	"3. Русский\n" +
//				//	"4. Latin\n\n" +
//				//	localization[Localized::CHOOSE_OPTION] + ' '
//				//);
//				//cin >> option;
//				//output(OUTPUT_DIVIDER);
//
//				switch (option)
//				{
//				case 1:
//					langOption = Language::ENGLISH;
//					languageChanged = true;
//					break;
//				case 2:
//					langOption = Language::UKRAINIAN;
//					languageChanged = true;
//					break;
//				case 3:
//					langOption = Language::RUSSIAN;
//					languageChanged = true;
//					break;
//				case 4:
//					langOption = Language::LATIN;
//					languageChanged = true;
//					break;
//				default: // If choosen option is not in the menu
//					//output(localization[Localized::WRONG_INPUT] + "\n\n", 12);
//					break;
//				}
//			}
//			localization.setLanguage(langOption);
//			fout.open("Data/Settings.conf");
//			fout << langOption;
//			fout.close();
//			languageChanged = false;
//			break;
//
//		case 4: // Exit game
//			//output(localization[Localized::EXIT_MESSAGE] + '\n', 10);
//			//output(OUTPUT_DIVIDER);
//			return false;
//
//		default: // If chosen option is not in the menu
//			//output(localization[Localized::WRONG_INPUT] + "\n", 12);
//			//output(OUTPUT_DIVIDER);
//			break;
//		}
//	}
//}

//void outputGameMenu(Player& rPlayer, NPC* npcs)
//{
//	while (true)
//	{
//		int option;
//		//output(
//		//	"1. " + localization[Localized::GAME_MENU_WAIT] + '\n' +
//		//	"2. " + localization[Localized::GAME_MENU_START_FIGHT] + '\n' +
//		//	"3. " + localization[Localized::GAME_MENU_VIEW_PLAYER] + '\n' +
//		//	"4. " + localization[Localized::GAME_MENU_VIEW_OPPONENTS] + '\n' +
//		//	"5. " + localization[Localized::GAME_MENU_SAVE_GAME] + '\n' +
//		//	"6. " + localization[Localized::EXIT] + "\n\n" +
//		//	localization[Localized::CHOOSE_OPTION] + ' '
//		//);
//		//cin >> option;
//		//output(OUTPUT_DIVIDER);
//
//		int i;
//		bool incorrectOption = true;
//		vector<int> availableNPCIndexes;
//		switch (option)
//		{
//		case 1: // Wait a day
//			skipDay(rPlayer, npcs, OPPONENTS_NUMBER);
//			break;
//
//		case 2: // Start fight
//			// The player can not fight if has less than 30% HP
//			if (rPlayer.getHP() > rPlayer.getFullHP() * 3 / 10)
//			{
//				//output(localization[Localized::FIGHT_PLAYER_WOUNDED] + "\n", 4);
//				break;
//			}
//
//			while (incorrectOption)
//			{
//				// Output available opponents
//				//output(localization[Localized::FIGHT_CHOOSE_OPPONENT] + "\n\n");
//
//				int npcNumber = 1;
//				for (i = 0; i < OPPONENTS_NUMBER; i++)
//				{
//					// The NPC can not fight if has less than 30% HP
//					if (npcs[i].getHP() >= npcs[i].getFullHP() * 3 / 10)
//					{
//						availableNPCIndexes.push_back(i);
//						//output(to_string(npcNumber) + ". ");
//						//output(npcs[i].getName() + '\n');
//						++npcNumber;
//					}
//				}
//				//output("\n");
//				// Output the exit option
//				//output(to_string(npcNumber + 1) + ". " + localization[Localized::EXIT] + "\n\n");
//
//				//output(localization[Localized::CHOOSE_OPTION] + ' ');
//				//cin >> option;
//
//				// Check of the chosen option
//				if (option > 0 && option < npcNumber + 2)
//				{
//					//output(OUTPUT_DIVIDER);
//					incorrectOption = false;
//
//					if (option != npcNumber + 1) // Fight
//					{
//						FightStatus result = fight(rPlayer, npcs[availableNPCIndexes[option]]);
//
//						// Concequence of the fight
//						// TODO:
//						// How much does player's and opponent's fame changes after the fight
//						// How much experience and gold does the player get
//						switch (result)
//						{
//						case FightStatus::OPPONENT_LOST:
//							rPlayer.setFame(rPlayer.getFame() + npcs[availableNPCIndexes[option]].getFame() / 10);
//							//output(localization[Localized::FIGHT_GAINED_FAME] + ' ');
//							//output(to_string(npcs[availableNPCIndexes[option]].fame / 10) + "\n\n", 6);
//							break;
//						case FightStatus::OPPONNENT_SURRENDERED:
//							rPlayer.setFame(rPlayer.getFame() + npcs[availableNPCIndexes[option]].getFame() / 10);
//							//output(localization[Localized::FIGHT_GAINED_FAME] + ' ');
//							//output(to_string(npcs[availableNPCIndexes[option]].fame / 10) + "\n\n", 6);
//							break;
//						case FightStatus::PLAYER_SURRENDERED:
//							rPlayer.setFame(rPlayer.getFame() - rPlayer.getFame() / 10);
//							npcs[availableNPCIndexes[option]].setFame(
//								npcs[availableNPCIndexes[option]].getFame() + rPlayer.getFame() / 10
//							);
//							break;
//						case FightStatus::PLAYER_LOST:
//							// TODO: goto game menu
//							//output("Game over! You lose!", 4);
//							break;
//						default: case FightStatus::CONTINUE:
//							//outputError("Unknown fight result!\n");
//							break;
//						}
//					}
//					else // Exit
//						break;
//				}
//				//else
//					//output(localization[Localized::WRONG_INPUT] + "\n\n", 12);
//			}
//			break;
//
//		case 3: // Show player
//			//output(localization[Localized::STATUS_PLAYER] + "\n\n");
//			//displayPlayer(rPlayer);
//			//output("\n\n");
//
//			// Weapons
//			if (rPlayer.isRightHandOccupied())
//			{
//				//output(localization[Localized::STATUS_PLAYER_WEAPON] + "\n\n", 14);
//				displayWeapon(rPlayer.getRightHand());
//				//output("\n");
//
//				if (rPlayer.isLeftHandOccupied())
//				{
//					displayWeapon(rPlayer.getLeftHand());
//					//output("\n\n");
//				}
//			}
//			else if (rPlayer.isLeftHandOccupied())
//			{
//				//output(localization[Localized::STATUS_PLAYER_WEAPON] + "\n\n", 14);
//				displayWeapon(rPlayer.getLeftHand());
//				//output("\n\n");
//			}
//
//			if (rPlayer.isArmourEquipped())
//			{
//				//output("Your armour:" + string("\n\n"), 14);
//				displayArmour(rPlayer.getArmour());
//				//output("\n\n");
//			}
//			break;
//
//		case 4: // Show opponents
//			//output(localization[Localized::STATUS_OPPONENTS] + "\n\n");
//			//displayNPCBatch(npcs, OPPONENTS_NUMBER);
//			break;
//
//		case 5: // Save
//			//if (saveGame(rPlayer, npcs))
//			//	output(localization[Localized::GAME_MENU_SAVE_GAME_SUCCESS] + '\n', 10);
//			//else
//			//	output(localization[Localized::GAME_MENU_SAVE_GAME_ERROR] + '\n', 12);
//			break;
//
//		case 6: // Exit game
//			//output(localization[Localized::EXIT_MESSAGE] + '\n', 10);
//			return;
//
//		default: // If choosen option is not in the menu
//			//output(localization[Localized::WRONG_INPUT] + "\n", 12);
//			break;
//		}
//		//output(OUTPUT_DIVIDER);
//	}
//}

LRESULT CALLBACK WFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	PAINTSTRUCT ps;
	static int sx, sy;

	switch (message)
	{
	case WM_CREATE:
		// Setting the random seed
		srand((unsigned)time(0));

		l.setLanguage();
		game.getMenuManager().setMenu(new MainMenu(hwnd));
		break;

	case WM_COMMAND:
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_SIZE:
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		game.resizeWindow(sx / 2, sy / 2);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		game.drawWindow(hwnd, hdc, sx / 2, sy / 2);
		EndPaint(hwnd, &ps);
		break;

	case WM_DRAWITEM:
	{
		game.stylizeWindow(hwnd, message, wParam, lParam);
		return true;
	}
	break;

	case WM_MOUSEMOVE:
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_MOUSEHOVER:
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_MOUSELEAVE:
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
	return 0L;
}
