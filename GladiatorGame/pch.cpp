// pch.cpp: source file corresponding to the pre-compiled header

#include "pch.h"

// When you are using pre-compiled headers, this source file is necessary for compilation to succeed.

Localization localization;

// __________ Other __________

string toStringPrecision(double n, int precision)
{
	string str = to_string(n);
	if (precision > 0)
		return str.substr(0, str.find(".") + 1 + precision);
	else if (precision == 0)
		return str.substr(0, str.find("."));
	return "";
}

void output(string s, int color)
{
	SetConsoleTextAttribute(hConsole, color);
	cout << s;
}

// __________ Gladiator __________

Gladiator* createRandomGladiator()
{
	const string PATH_BASE = "Data/Language/Names/", FORMAT = ".lang";
	ifstream fin;
	// Open file containing roman first names
	if (localization.language == 0)
		fin.open(PATH_BASE + "En_FirstNames" + FORMAT);
	else if (localization.language == 1)
		fin.open(PATH_BASE + "Uk_FirstNames" + FORMAT);
	else
		fin.open(PATH_BASE + "Ru_FirstNames" + FORMAT);

	if (!fin)
		output(localization.messages[Localized::CREATION_NAME_LOAD_ERROR], 4);
	int count = 0;
	string line, name;
	while (getline(fin, line)) // Count lines in file
		count++;
	int randomName = rand() % count; // Choose random line
	fin.clear();
	fin.seekg(0);
	int i = 0;
	for (; i < randomName; i++)
		getline(fin, line);
	fin.close();
	name = line; // Assign random name

	// Open file containing roman last names
	if (localization.language == 0)
		fin.open(PATH_BASE + "En_LastNames" + FORMAT);
	else if (localization.language == 1)
		fin.open(PATH_BASE + "Uk_LastNames" + FORMAT);
	else
		fin.open(PATH_BASE + "Ru_LastNames" + FORMAT);

	if (!fin)
		output(localization.messages[Localized::CREATION_NAME_LOAD_ERROR], 4);
	count = 0;
	while (getline(fin, line)) // Count lines in file
		count++;
	randomName = rand() % count; // Choose random line
	fin.clear();
	fin.seekg(0);
	for (i = 0; i < randomName; i++)
		getline(fin, line);
	fin.close();

	name += ' ' + line; // Add last name

	return new Gladiator(
		name,
		MIN_AGE + rand() % (MAX_AGE - 1),
		BASIC_HEALTH,
		1 + rand() % 300, // Fame
		BASIC_FATIGUE,
		MIN_STRENGTH + rand() / (RAND_MAX / (MAX_STRENGTH - 10)),
		MIN_CONSTITUTION + rand() / (RAND_MAX / (MAX_CONSTITUTION - 10)),
		MIN_DEXTERITY + rand() / (RAND_MAX / (MAX_DEXTERITY - 10)),
		MIN_INTELLIGENCE + rand() / (RAND_MAX / (MAX_INTELLIGENCE - 10)),
		MIN_WISDOM + rand() / (RAND_MAX / (MAX_WISDOM - 10)),
		MIN_CHARISMA + rand() / (RAND_MAX / (MAX_CHARISMA - 10)),
		createRandomWeapon()
	);
}

void displayGladiator(Gladiator& g)
{
	output(localization.messages[Localized::STATUS_NAME] + ' ');
	output(g.name + '\n', 14);
	output(localization.messages[Localized::STATUS_AGE] + ' ');
	output(to_string(g.age) + '\n', 14);
	output(localization.messages[Localized::STATUS_HEALTH] + ' ');
	output(to_string(g.health) + '\n', 12);
	output(localization.messages[Localized::STATUS_FAME] + ' ');
	output(to_string(g.fame) + '\n', 6);
	output(localization.messages[Localized::STATUS_FATIGUE] + ' ');
	output(to_string(g.fatigue) + '\n', 12);
	output(localization.messages[Localized::STATUS_STRENGTH] + ' ');
	output(to_string(g.strength) + '\n', 4);
	output(localization.messages[Localized::STATUS_CONSTITUTION] + ' ');
	output(to_string(g.constitution) + '\n', 12);
	output(localization.messages[Localized::STATUS_DEXTERITY] + ' ');
	output(to_string(g.dexterity) + '\n', 2);
	output(localization.messages[Localized::STATUS_INTELLIGENCE] + ' ');
	output(to_string(g.intelligence) + '\n', 1);
	output(localization.messages[Localized::STATUS_WISDOM] + ' ');
	output(to_string(g.wisdom) + '\n', 13);
	output(localization.messages[Localized::STATUS_CHARISMA] + ' ');
	output(to_string(g.charisma) + "\n\n", 6);
}

void displayBot(Gladiator& g)
{
	output("\n");
	displayGladiator(g);
	output(localization.messages[Localized::STATUS_WEAPON] + "\n\n", 14);
	displayWeapon(*g.weapon);
}

void displayGladiatorBatch(Gladiator* arr, int number)
{
	for (int i = 0; i < number; i++)
	{
		output(to_string(i + 1) + ' ' + localization.messages[Localized::STATUS_GLADIATOR] + '\n', 4);
		displayBot(arr[i]);
	}
}

void createGladiator(Gladiator& player)
{
	player.strength = BASIC_PLAYER_ATTRIBUTES;
	player.constitution = BASIC_PLAYER_ATTRIBUTES;
	player.dexterity = BASIC_PLAYER_ATTRIBUTES;
	player.intelligence = BASIC_PLAYER_ATTRIBUTES;
	player.wisdom = BASIC_PLAYER_ATTRIBUTES;
	player.charisma = BASIC_PLAYER_ATTRIBUTES;

	bool characterCreation = true;
	int option;
	while (characterCreation)
	{
		int attributes = 30;

		output(localization.messages[Localized::CREATION_CREATE_CHARACTER] + "\n\n");

		// Name
		output(localization.messages[Localized::CREATION_INPUT_NAME] + ' ', 15);
		SetConsoleTextAttribute(hConsole, 14);
		getline(cin, player.name);

		// Age
		output(localization.messages[Localized::CREATION_INPUT_AGE] + ' ');
		SetConsoleTextAttribute(hConsole, 14);
		cin >> player.age;

		// Input of player's attributes
		bool setAttributes = true,
			setStrength = true,
			setConstitution = true,
			setDexterity = true,
			setIntelligence = true,
			setWisdom = true,
			setCharisma = true;
		while (
			setAttributes &&
			(setStrength || setConstitution || setDexterity || setIntelligence || setWisdom || setCharisma)
			) {
			// Attribute points' instruction
			output("\n");
			output(localization.messages[Localized::CREATION_INPUT_POINTS_INSTRUCTION] + "\n");
			output(localization.messages[Localized::STRENGTH] + ' ', 4);
			output(localization.messages[Localized::CONSTITUTION] + ' ', 12);
			output(localization.messages[Localized::DEXTERITY] + ' ', 2);
			output(localization.messages[Localized::INTELLIGENCE] + ' ', 1);
			output(localization.messages[Localized::WISDOM] + ' ', 13);
			output(localization.messages[Localized::CHARISMA] + '\n', 6);
			output(localization.messages[Localized::CREATION_INPUT_POINTS_INSTRUCTION2] + "\n\n");

			// Strength
			while (setAttributes && setStrength)
			{
				output("\n");
				output(localization.messages[Localized::CREATION_INPUT_POINTS]);
				output(" " + to_string(attributes) + "\n", 10);
				output(localization.messages[Localized::CREATION_INPUT_STRENGTH] + ' ');
				SetConsoleTextAttribute(hConsole, 4);
				cin >> player.strength;
				if (player.strength + 1 < MIN_STRENGTH || player.strength > MAX_STRENGTH)
					output(localization.messages[Localized::WRONG_INPUT] + '\n', 4);
				else
				{
					player.strength += BASIC_PLAYER_ATTRIBUTES;
					if (attributes - (player.strength - 10) < 0)
						output(localization.messages[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
					else
					{
						attributes -= player.strength - 10;
						setStrength = false;
					}
				}
			}
			if (!attributes)
				setAttributes = false;

			// Constitution
			while (setAttributes && setConstitution)
			{
				output("\n");
				output(localization.messages[Localized::CREATION_INPUT_POINTS]);
				output(" " + to_string(attributes) + "\n", 10);
				output(localization.messages[Localized::CREATION_INPUT_CONSTITUTION] + ' ');
				SetConsoleTextAttribute(hConsole, 12);
				cin >> player.constitution;
				if (player.constitution + 1 < MIN_CONSTITUTION || player.constitution > MAX_CONSTITUTION)
					output(localization.messages[Localized::WRONG_INPUT] + '\n', 4);
				else
				{
					player.constitution += BASIC_PLAYER_ATTRIBUTES;
					if (attributes - (player.constitution - 10) < 0)
						output(localization.messages[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
					else
					{
						attributes -= player.constitution - 10;
						setConstitution = false;
					}
				}
			}
			if (!attributes)
				setAttributes = false;

			// Dexterity
			while (setAttributes && setDexterity)
			{
				output("\n");
				output(localization.messages[Localized::CREATION_INPUT_POINTS]);
				output(" " + to_string(attributes) + "\n", 10);
				output(localization.messages[Localized::CREATION_INPUT_DEXTERITY] + ' ');
				SetConsoleTextAttribute(hConsole, 2);
				cin >> player.dexterity;
				if (player.dexterity + 1 < MIN_DEXTERITY || player.dexterity > MAX_DEXTERITY)
					output(localization.messages[Localized::WRONG_INPUT] + '\n', 4);
				else
				{
					player.dexterity += BASIC_PLAYER_ATTRIBUTES;
					if (attributes - (player.dexterity - 10) < 0)
						output(localization.messages[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
					else
					{
						attributes -= player.dexterity - 10;
						setDexterity = false;
					}
				}
			}
			if (!attributes)
				setAttributes = false;

			// Intelligence
			while (setAttributes && setIntelligence)
			{
				output("\n");
				output(localization.messages[Localized::CREATION_INPUT_POINTS]);
				output(" " + to_string(attributes) + "\n", 10);
				output(localization.messages[Localized::CREATION_INPUT_INTELLIGENCE] + ' ');
				SetConsoleTextAttribute(hConsole, 1);
				cin >> player.intelligence;
				if (player.intelligence + 1 < MIN_INTELLIGENCE || player.intelligence > MAX_INTELLIGENCE)
					output(localization.messages[Localized::WRONG_INPUT] + '\n', 4);
				else
				{
					player.intelligence += BASIC_PLAYER_ATTRIBUTES;
					if (attributes - (player.intelligence - 10) < 0)
						output(localization.messages[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
					else
					{
						attributes -= player.intelligence - 10;
						setIntelligence = false;
					}
				}
			}
			if (!attributes)
				setAttributes = false;

			// Wisdom
			while (setAttributes && setWisdom)
			{
				output("\n");
				output(localization.messages[Localized::CREATION_INPUT_POINTS]);
				output(" " + to_string(attributes) + "\n", 10);
				output(localization.messages[Localized::CREATION_INPUT_WISDOM] + ' ');
				SetConsoleTextAttribute(hConsole, 13);
				cin >> player.wisdom;
				if (player.wisdom + 1 < MIN_WISDOM || player.wisdom > MAX_WISDOM)
					output(localization.messages[Localized::WRONG_INPUT] + '\n', 4);
				else
				{
					player.wisdom += BASIC_PLAYER_ATTRIBUTES;
					if (attributes - (player.wisdom - 10) < 0)
						output(localization.messages[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
					else
					{
						attributes -= player.wisdom - 10;
						setWisdom = false;
					}
				}
			}
			if (!attributes)
				setAttributes = false;

			// Charisma
			while (setAttributes && setCharisma)
			{
				output("\n");
				output(localization.messages[Localized::CREATION_INPUT_POINTS]);
				output(" " + to_string(attributes) + "\n", 10);
				output(localization.messages[Localized::CREATION_INPUT_CHARISMA] + ' ');
				SetConsoleTextAttribute(hConsole, 13);
				cin >> player.charisma;
				if (player.charisma + 1 < MIN_CHARISMA || player.charisma > MAX_CHARISMA)
					output(localization.messages[Localized::WRONG_INPUT] + '\n', 4);
				else
				{
					player.charisma += BASIC_PLAYER_ATTRIBUTES;
					if (attributes - (player.charisma - 10) < 0)
						output(localization.messages[Localized::CREATION_INPUT_NO_POINTS] + '\n', 4);
					else
					{
						attributes -= player.charisma - 10;
						setCharisma = false;
					}
				}
			}
			if (!attributes)
				setAttributes = false;
		}

		// Confirming player's parameters
		bool notConfirmed = true;
		while (notConfirmed)
		{
			// Show created character
			output(OUTPUT_DIVIDER + '\n' + localization.messages[Localized::STATUS_GLADIATOR] + "\n\n");
			output(localization.messages[Localized::STATUS_NAME] + ' ');
			output(player.name + '\n', 14);
			output(localization.messages[Localized::STATUS_AGE] + ' ');
			output(to_string(player.age) + '\n', 14);
			output(localization.messages[Localized::STATUS_STRENGTH] + ' ');
			output(to_string(player.strength) + '\n', 4);
			output(localization.messages[Localized::STATUS_CONSTITUTION] + ' ');
			output(to_string(player.constitution) + '\n', 12);
			output(localization.messages[Localized::STATUS_DEXTERITY] + ' ');
			output(to_string(player.dexterity) + '\n', 2);
			output(localization.messages[Localized::STATUS_INTELLIGENCE] + ' ');
			output(to_string(player.intelligence) + '\n', 1);
			output(localization.messages[Localized::STATUS_WISDOM] + ' ');
			output(to_string(player.wisdom) + '\n', 13);
			output(localization.messages[Localized::STATUS_CHARISMA] + ' ');
			output(to_string(player.charisma) + "\n\n", 6);

			if (attributes > 0)
			{
				output(localization.messages[Localized::CREATION_INPUT_POINTS] + ' ');
				output(to_string(attributes) + "\n\n", 10);
			}
			output(localization.messages[Localized::CREATION_INPUT_CORRECT] + '\n');
			output("1. " + localization.messages[Localized::YES] + '\n');
			output("2. " + localization.messages[Localized::NO] + "\n\n");
			output(localization.messages[Localized::CHOOSE_OPTION] + ' ');
			cin >> option;
			cin.get();
			switch (option)
			{
			case 1:
				characterCreation = false;
				notConfirmed = false;
				break;
			case 2:
				characterCreation = true;
				notConfirmed = false;
				output(OUTPUT_DIVIDER);
				break;
			default:
				output("\n");
				output(localization.messages[Localized::WRONG_INPUT] + "\n", 4);
				notConfirmed = true;
				break;
			}
		}
	}
	player.health = BASIC_HEALTH;
	player.fatigue = BASIC_FATIGUE;
	player.fame = BASIC_FAME;
	player.weapon = createRandomWeapon();

	output("\n");
	output(localization.messages[Localized::CREATION_INPUT_SUCCESS] + '\n', 10);
}

bool gladiatorFight(Gladiator& player, Gladiator& bot)
{
	// Calculating damage (weapon damage * (strength scale + dexterity scale)). Max scale from 100 attributes is 70% increase
	int playerDamage = player.weapon->damage * ((1 + player.strength / 140) + (1 + player.dexterity / 140) - 1);
	int botDamage = bot.weapon->damage * ((1 + bot.strength / 140) + (1 + bot.dexterity / 140) - 1);

	// Calculating evasion chance from dexterity. Min 5% chance to max 35% chance from 100 attribute
	int playerEvasion = player.dexterity * 0.3030 + 5;
	int botEvasion = bot.dexterity * 0.3030 + 5;

	// Calculating bash chance from strength. Min 3% chance to max 20% chance from 100 attribute
	int playerBash = player.strength * 0.17 + 3;
	int botBash = bot.strength * 0.17 + 3;
	bool playerStun = false, botStun = false;

	output(localization.messages[Localized::FIGHT_START] + "\n\n", 4);

	while (player.health > 0 && bot.health > 0) // Fight until both opponents have health
	{
		if (!playerStun) // If player is not bashed he attacks
		{
			output(localization.messages[Localized::FIGHT_PLAYER_ATTACKS] + '\n');
			Sleep(200);
			if (1 + rand() / (RAND_MAX / (100 - 1)) > botEvasion) // Check bot evasion chance
			{
				output(localization.messages[Localized::FIGHT_PLAYER_HITS] + '\n', 2);
				bot.health -= playerDamage;
				player.weapon->durability -= 1;
				if (1 + rand() / (RAND_MAX / (100 - 1)) < playerBash) // Check player chance to bash
				{
					botStun = true;
					Sleep(100);
					output(localization.messages[Localized::FIGHT_ENEMY_STUNNED] + '\n', 2);
				}
			}
			else
				output(localization.messages[Localized::FIGHT_ENEMY_EVADES] + '\n', 12);
		}
		else
			playerStun = false;

		Sleep(500);

		if (bot.health < 1) // If bot is alive fight continues
			break;

		output("\n\n");

		if (!botStun) // If bot is not bashed he attacks
		{
			output(localization.messages[Localized::FIGHT_ENEMY_ATTACKS] + '\n');
			Sleep(200);
			if (1 + rand() / (RAND_MAX / (100 - 1)) > playerEvasion) // Check player evasion chance
			{
				output(localization.messages[Localized::FIGHT_ENEMY_HITS] + '\n', 12);
				player.health -= botDamage;
				bot.weapon->durability -= 1;
				if (1 + rand() / (RAND_MAX / (100 - 1)) < botBash) // Check bot chance to bash
				{
					playerStun = true;
					Sleep(100);
					output(localization.messages[Localized::FIGHT_PLAYER_STUNNED] + '\n', 12);
				}
			}
			else
				output(localization.messages[Localized::FIGHT_PLAYER_EVADES] + '\n', 2);
		}
		else
			botStun = false;

		Sleep(500);

		output("\n\n");

		if (player.health < 1) // If player is alive fight continues
			break;

		// Display player's health
		output(localization.messages[Localized::FIGHT_PLAYER_HEALTH] + ' ');
		output(to_string(player.health) + '\n', 10);
		// Display enemy's health
		output(localization.messages[Localized::FIGHT_ENEMY_HEALTH] + ' ');
		output(to_string(bot.health) + "\n\n", 4);

		Sleep(500);
	}

	if (player.health < 1) // Player lost
	{
		player.health = 1;

		output(localization.messages[Localized::FIGHT_ENEMY_LAST_HIT] + "\n\n", 4);
		output(localization.messages[Localized::FIGHT_PLAYER_HEALTH] + ' ');
		output(to_string(player.health) + "\n", 10);
		output(localization.messages[Localized::FIGHT_ENEMY_HEALTH] + ' ');
		output(to_string(bot.health) + "\n\n", 4);
		return false;
	}
	else // Player won
	{
		bot.health = 1;

		output(localization.messages[Localized::FIGHT_PLAYER_LAST_HIT] + "\n\n", 10);
		output(localization.messages[Localized::FIGHT_PLAYER_HEALTH] + ' ');
		output(to_string(player.health) + "\n\n", 10);
		return true;
	}

}

// __________ Weapon __________

Weapon* createRandomGladius()
{
	int length = 45 + rand() / (RAND_MAX / (68 - 45));
	int weigth = length * 16;
	int speed = (int)(weigth * 0.125);
	return new Weapon(
		"Gladius",
		BASIC_DURABILITY,
		10 + rand() / (RAND_MAX / (35 - 10)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomSpatha()
{
	int length = 70 + rand() / (RAND_MAX / (90 - 70));
	int weigth = length * 18;
	int speed = (int)(weigth * 0.125);
	return new Weapon(
		"Spatha",
		BASIC_DURABILITY,
		15 + rand() / (RAND_MAX / (40 - 15)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomHasta()
{
	int length = 190 + rand() / (RAND_MAX / (230 - 190));
	int weigth = length * 5.5;
	int speed = (int)(weigth * 0.125);
	return new Weapon(
		"Hasta",
		BASIC_DURABILITY,
		10 + rand() / (RAND_MAX / (45 - 10)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomFasces()
{
	int length = 30 + rand() / (RAND_MAX / (55 - 30));
	int weigth = length * 25;
	int speed = (int)(weigth * 0.150);
	return new Weapon(
		"Fasces",
		BASIC_DURABILITY,
		20 + rand() / (RAND_MAX / (50 - 20)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomMace()
{
	int length = 30 + rand() / (RAND_MAX / (55 - 30));
	int weigth = length * 25;
	int speed = (int)(weigth * 0.150);
	return new Weapon(
		"Mace",
		BASIC_DURABILITY,
		10 + rand() / (RAND_MAX / (25 - 10)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomPugio()
{
	int length = 18 + rand() / (RAND_MAX / (28 - 18));
	int weigth = length * 14;
	int speed = (int)(weigth * 0.140);
	return new Weapon(
		"Pugio",
		BASIC_DURABILITY,
		10 + rand() / (RAND_MAX / (25 - 10)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomPilum()
{
	int length = 190 + rand() / (RAND_MAX / (230 - 190));
	int weigth = length * 10;
	int speed = (int)(weigth * 0.085);
	return new Weapon(
		"Hasta",
		BASIC_DURABILITY,
		25 + rand() / (RAND_MAX / (55 - 25)),
		length,
		weigth,
		speed
	);
}

Weapon* createRandomWeapon()
{
	int randomIndex = rand() % WEAPON_NUMBER;
	switch (randomIndex)
	{
	case 0: // Gladius
		return createRandomGladius();
	case 1: // Spatha
		return createRandomSpatha();
	case 2: // Hasta
		return createRandomHasta();
	case 3: // Fasces
		return createRandomFasces();
	case 4: // Mace
		return createRandomMace();
	case 5: // Pugio
		return createRandomPugio();
	case 6: // Pilum
		return createRandomPilum();
	default:
		output(localization.messages[Localized::ERROR_UNKNOWN_WEAPON_TYPE] + '\n');
		return new Weapon();
	}
}

void displayWeapon(Weapon& weapon)
{
	Converter converter;
	output(localization.messages[Localized::WEAPON_STATUS_TYPE] + ' ');
	output(weapon.name + '\n', 14);
	output(localization.messages[Localized::WEAPON_STATUS_DAMAGE] + ' ');
	output(to_string(weapon.damage) + '\n', 4);
	output(localization.messages[Localized::WEAPON_STATUS_LENGTH] + ' ');
	output(toStringPrecision((converter.toMetres(weapon.length))), 2);
	output(" (m)\n" + localization.messages[Localized::WEAPON_STATUS_WEIGTH] + ' ');
	output(toStringPrecision((converter.toKilograms(weapon.weigth))), 12);
	output(" (kg)\n" + localization.messages[Localized::WEAPON_STATUS_SPEED] + ' ');
	output(to_string(weapon.speed) + '\n', 6);
	output(localization.messages[Localized::WEAPON_STATUS_DURABILITY] + ' ');
	output(to_string(weapon.durability / 10) + "%\n\n", 11);
}

// __________ Save&Load __________

bool saveGame(Gladiator& player, Gladiator* bots)
{
	const string DIR_NAME = "Saves/", FILE_PLAYER = "Player", FILE_OPPONENTS = "Opponents", FORMAT = ".save";
	ofstream fout;

	// Creating the folder of saved games if it doesn't exist
	if (_mkdir(DIR_NAME.c_str()) == 0)
	{
		// Directories for save files with data
		fout.open(DIR_NAME + FILE_PLAYER + FORMAT);
		fout.close();
		fout.open(DIR_NAME + FILE_OPPONENTS + FORMAT);
		fout.close();
	}

	// Open file for saving player's character and empty it
	fout.open(DIR_NAME + FILE_PLAYER + FORMAT, ios::binary);
	// Check if file is open
	if (fout)
	{
		// Save player
		fout.write((char*)&player, sizeof(player));
		fout.write((char*)&*player.weapon, sizeof(*player.weapon));
		fout.close();
	}
	else // If save file couldn't be opened - save unsucessful
		return false;

	// Open file for saving enemies and empty it
	fout.open(DIR_NAME + FILE_OPPONENTS + FORMAT, ios::binary);
	// Check if file is open
	if (fout)
	{
		// Save bots
		for (int i = 0; i < OPPONENTS_NUMBER; i++)
		{
			fout.write((char*)&bots[i], sizeof(bots[i]));
			fout.write((char*)&*bots[i].weapon, sizeof(*bots[i].weapon));
		}
		// Close save files
		fout.close();
		return true;
	}
	// If save file couldn't be opened - save unsucessful
	return false;
}

bool loadGame(Gladiator& player, Gladiator* bots)
{
	if (loadPlayer(player) && loadBots(bots))
		return true;
	return false;
}

bool loadPlayer(Gladiator& player)
{
	// Open file with player's character
	const string DIR_NAME = "Saves/", FILE_NAME = "Player", FORMAT = ".save";
	ifstream fin(DIR_NAME + FILE_NAME + FORMAT, ios::binary);

	// Check if save file is open and not empty
	if (fin)
	{
		fin.read((char*)&player, sizeof(player));
		player.weapon = new Weapon();
		fin.read((char*)&*player.weapon, sizeof(*player.weapon));
		fin.close();

		// Load sucessful
		return true;
	}

	fin.close();
	// Load unsucessful
	return false;
}

bool loadBots(Gladiator* bots)
{
	// Open file for loading enemies
	const string DIR_NAME = "Saves/", FILE_NAME = "Opponents", FORMAT = ".save";
	ifstream fin(DIR_NAME + FILE_NAME + FORMAT, ios::binary);

	// Check if save file is open and not empty
	if (fin)
	{
		for (int i = 0; i < OPPONENTS_NUMBER; i++)
		{
			fin.read((char*)&bots[i], sizeof(bots[i]));
			bots[i].weapon = new Weapon();
			fin.read((char*)&*bots[i].weapon, sizeof(*bots[i].weapon));
		}
		fin.close();

		// Load sucessful
		return true;
	}

	// Load unsucessful
	return false;
}

// __________ Game process __________

void skipDay(Gladiator& player, Gladiator* bots, int n)
{
	int i;
	output(localization.messages[Localized::TIME_WAITED] + "\n\n");

	// Bots regen health
	for (i = 0; i < n; i++)
	{
		if (bots[i].health < 100)
			bots[i].health += BASIC_REGEN;
		if (bots[i].health > 100)
			bots[i].health = 100;
	}

	// Player regen health
	if (player.health < 100)
	{
		player.health += BASIC_REGEN;
		if (player.health > 100)
		{
			player.health = 100;
			output(localization.messages[Localized::TIME_FULL_HEALTH] + '\n', 10);
		}
		else
		{
			output(localization.messages[Localized::TIME_REGEN] + ' ');
			output(to_string(BASIC_REGEN) + '\n', 10);
			output(localization.messages[Localized::TIME_CURRENT_HEALTH] + ' ');
			output(to_string(player.health) + '\n', 10);
		}
	}
	else
		output(localization.messages[Localized::TIME_NOTHING] + '\n');

	output(OUTPUT_DIVIDER);
}

bool outputStartMenu(Gladiator& player, Gladiator* bots)
{
	int option;
	Language langOption;
	ofstream fout;
	bool languageChanged = false;
	output("\n\nGladiator Game\n", 4);
	output(OUTPUT_DIVIDER);
	while (true)
	{
		output(
			"1. " + localization.messages[Localized::START_MENU_START_GAME] + '\n' +
			"2. " + localization.messages[Localized::START_MENU_LOAD_GAME] + '\n' +
			"3. " + localization.messages[Localized::START_MENU_CHANGE_LANGUAGE] + '\n' +
			"4. " + localization.messages[Localized::EXIT] + "\n\n" +
			localization.messages[Localized::CHOOSE_OPTION] + ' '
		);
		cin >> option;
		cin.get();
		output(OUTPUT_DIVIDER);

		switch (option)
		{
		case 1: // New game
			createGladiator(player);
			for (int i = 0; i < OPPONENTS_NUMBER; i++)
				bots[i] = *createRandomGladiator();
			output(OUTPUT_DIVIDER);
			return true;

		case 2: // Load game
			if (loadGame(player, bots))
			{
				output(localization.messages[Localized::START_MENU_LOAD_GAME_SUCCESS] + "\n", 10);
				output(OUTPUT_DIVIDER);
				return true;
			}
			else
			{
				output(localization.messages[Localized::START_MENU_LOAD_GAME_ERROR] + "\n", 12);
				output(OUTPUT_DIVIDER);
				break;
			}

		case 3: // Change language
			while (!languageChanged)
			{
				output(
					"1. English\n" +
					string("2. Українська\n") +
					"3. Русский\n" +
					"4. Latin\n\n" +
					localization.messages[Localized::CHOOSE_OPTION] + ' '
				);
				cin >> option;
				output(OUTPUT_DIVIDER);

				switch (option)
				{
				case 1:
					langOption = Language::ENGLISH;
					languageChanged = true;
					break;
				case 2:
					langOption = Language::UKRAINIAN;
					languageChanged = true;
					break;
				case 3:
					langOption = Language::RUSSIAN;
					languageChanged = true;
					break;
				case 4:
					langOption = Language::LATIN;
					languageChanged = true;
					break;
				default: // If choosen option is not in the menu
					output(localization.messages[Localized::WRONG_INPUT] + "\n\n", 12);
					break;
				}
			}
			localization.setLanguage(langOption);
			fout.open("Data/Settings.conf");
			fout << langOption;
			fout.close();
			languageChanged = false;
			break;

		case 4: // Exit game
			output(localization.messages[Localized::EXIT_MESSAGE] + '\n', 10);
			output(OUTPUT_DIVIDER);
			return false;

		default: // If chosen option is not in the menu
			output(localization.messages[Localized::WRONG_INPUT] + "\n", 12);
			output(OUTPUT_DIVIDER);
			break;
		}
	}
}

bool outputGameMenu(Gladiator& player, Gladiator* bots)
{
	int option, i;
	bool fight;
	while (true)
	{
		output(
			"1. " + localization.messages[Localized::GAME_MENU_WAIT] + '\n' +
			"2. " + localization.messages[Localized::GAME_MENU_START_FIGHT] + '\n' +
			"3. " + localization.messages[Localized::GAME_MENU_VIEW_GLADIATOR] + '\n' +
			"4. " + localization.messages[Localized::GAME_MENU_VIEW_OPPONENTS] + '\n' +
			"5. " + localization.messages[Localized::GAME_MENU_SAVE_GAME] + '\n' +
			"6. " + localization.messages[Localized::EXIT] + "\n\n" +
			localization.messages[Localized::CHOOSE_OPTION] + ' '
		);
		cin >> option;
		output(OUTPUT_DIVIDER);

		switch (option)
		{
		case 1: // Wait a day
			skipDay(player, bots, OPPONENTS_NUMBER);
			break;

		case 2: // Start fight
			fight = true;
			while (fight)
			{
				if (player.health < 30)
				{
					output(localization.messages[Localized::FIGHT_PLAYER_WOUNDED] + "\n", 4);
					output(OUTPUT_DIVIDER);
					fight = false;
					break;
				}
				output(localization.messages[Localized::FIGHT_CHOOSE_OPPONENT] + "\n\n");
				for (i = 0; i < OPPONENTS_NUMBER; i++)
				{
					output(to_string(i + 1) + ". ");
					if (bots[i].health >= 30)
						output(bots[i].name + '\n');
					else
						output(bots[i].name + '\n', 4);
				}
				output("\n");
				output("4. " + localization.messages[Localized::EXIT] + "\n\n");
				output(localization.messages[Localized::CHOOSE_OPTION] + ' ');
				cin >> option;
				output(OUTPUT_DIVIDER);
				switch (option)
				{
				case 1: // First opponent
					if (bots[0].health < 30)
					{
						output(localization.messages[Localized::FIGHT_OPPONENT_WOUNDED] + "\n\n");
						break;
					}
					if (gladiatorFight(player, bots[0]))
					{
						output(localization.messages[Localized::FIGHT_PLAYER_WON] + "\n\n", 10);
						player.fame += bots[0].fame / 10;
						output(localization.messages[Localized::FIGHT_GAINED_LOOT] + '\n');
						output(localization.messages[Localized::FIGHT_GAINED_FAME] + ' '); output(to_string(bots[0].fame / 10) + "\n\n", 6);
						output(OUTPUT_DIVIDER);
					}
					else
					{
						output(localization.messages[Localized::FIGHT_PLAYER_LOST] + '\n', 4);
						bots[0].fame += player.fame / 10;
						output(OUTPUT_DIVIDER);
					}
					fight = false;
					break;

				case 2: // Second opponent
					if (bots[1].health < 30)
					{
						output(localization.messages[Localized::FIGHT_OPPONENT_WOUNDED] + "\n\n");
						break;
					}
					if (gladiatorFight(player, bots[1]))
					{
						output(localization.messages[Localized::FIGHT_PLAYER_WON] + "\n\n", 10);
						player.fame += bots[1].fame / 10;
						output(localization.messages[Localized::FIGHT_GAINED_LOOT] + '\n');
						output(localization.messages[Localized::FIGHT_GAINED_FAME] + ' ');
						output(to_string(bots[0].fame / 10) + "\n\n", 6);
						output(OUTPUT_DIVIDER);
					}
					else
					{
						output(localization.messages[Localized::FIGHT_PLAYER_LOST] + '\n', 4);
						bots[1].fame += player.fame / 10;
						output(OUTPUT_DIVIDER);
					}
					fight = false;
					break;

				case 3: // Third opponent
					if (bots[2].health < 30)
					{
						output(localization.messages[Localized::FIGHT_OPPONENT_WOUNDED] + "\n\n");
						break;
					}
					if (gladiatorFight(player, bots[2]))
					{
						output(localization.messages[Localized::FIGHT_PLAYER_WON] + "\n\n", 10);
						player.fame += bots[2].fame / 10;
						output(localization.messages[Localized::FIGHT_GAINED_LOOT] + '\n');
						output(localization.messages[Localized::FIGHT_GAINED_FAME] + ' '); output(to_string(bots[0].fame / 10) + "\n\n", 6);
						output(OUTPUT_DIVIDER);
					}
					else
					{
						output(localization.messages[Localized::FIGHT_PLAYER_LOST] + '\n', 4);
						bots[2].fame += player.fame / 10;
						output(OUTPUT_DIVIDER);
					}
					fight = false;
					break;

				case 4: // Exit
					fight = false;
					break;

				default: // If choosen option is not in the menu
					output(localization.messages[Localized::WRONG_INPUT] + "\n\n", 12);
					output(OUTPUT_DIVIDER);
					break;
				}
			}
			break;

		case 3: // Show player
			output(localization.messages[Localized::STATUS_YOUR_GLADIATOR] + "\n\n");
			displayGladiator(player);
			output(localization.messages[Localized::STATUS_YOUR_WEAPON] + "\n\n", 14);
			displayWeapon(*player.weapon);
			output(OUTPUT_DIVIDER);
			break;

		case 4: // Show enemies
			output(localization.messages[Localized::STATUS_OPPONENTS] + "\n\n");
			displayGladiatorBatch(bots, OPPONENTS_NUMBER);
			output(OUTPUT_DIVIDER);
			break;

		case 5: // Save
			if (saveGame(player, bots))
				output(localization.messages[Localized::GAME_MENU_SAVE_GAME_SUCCESS] + '\n', 10);
			else
				output(localization.messages[Localized::GAME_MENU_SAVE_GAME_ERROR] + '\n', 12);
			output(OUTPUT_DIVIDER);
			break;

		case 6: // Exit game
			output(localization.messages[Localized::EXIT_MESSAGE] + '\n', 10);
			output(OUTPUT_DIVIDER);
			return false;

		default: // If choosen option is not in the menu
			output(localization.messages[Localized::WRONG_INPUT] + "\n", 12);
			output(OUTPUT_DIVIDER);
			break;
		}
	}
}
