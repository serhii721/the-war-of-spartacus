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

void getFoldersInDirectory(const string& directoryPath, vector<string>& folderNames)
{
	// Create namespace for easy access to functions
	namespace fs = std::experimental::filesystem;
	if (!fs::exists(directoryPath) || !fs::is_directory(directoryPath))
		throw new exception("Error: Couldn't open folder for loading");

	for (const auto& entry : fs::directory_iterator(directoryPath))
		if (fs::is_directory(entry))
			folderNames.push_back(entry.path().filename().string());
}

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
	int itemTier = 0, n = aproximateLevel;
	// For every 16 levels of NPC increasing it's equipment tier by 1
	while (n > 0)
	{
		itemTier++;
		n -= 16;
	}
	unique_ptr<Weapon> rightHand = generateWeapon(itemTier);
	unique_ptr<Weapon> leftHand;
	if (rightHand->getWeaponType() != Weapon::WeaponType::AXE && rightHand->getWeaponType() != Weapon::WeaponType::SPEAR)
	{
		leftHand = generateWeapon(itemTier);
		if (!rightHand->isCompatibleWith(leftHand->getWeaponType()))
		{
			if (rand() % 100 < 75)
			{
				if (leftHand)
					leftHand.release();
				leftHand = generateWeapon(itemTier, Weapon::SHIELD);
			}
			else if (rand() % 100 < 75)
				do
				{
					if (leftHand)
						leftHand.release();
					leftHand = generateWeapon(itemTier);
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
	unique_ptr<Armour> armour = generateArmour(itemTier);

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
			make_unique<Inventory>(),
			move(rightHand),
			move(leftHand),
			move(armour)
		),
		NamedNPC(
			randomFirstName,
			randomLastName
		),
		leveling,
		rand() % 3 + 1
	);
	npc.updateMaxHP();

	return make_unique<NPC>(npc);
}

unique_ptr<HarmlessNPC> generateTrader(int level)
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

	// 2. Generating inventory
	unique_ptr<Inventory> inventory = make_unique<Inventory>();

	// Add gold
	inventory->addItem(make_unique<Item>(Item(Item::ItemType::GOLD)), 2000 * level);
	// Add items
	for (int i = 1; i < MAX_INVENTORY_SIZE; i++)
	{
		if (rand() % 100 < 75) // 75% chance to generate weapon
		{
			if (rand() % 100 < 75) // 75% chance to generate normal weapon
				inventory->addItem(generateWeapon(level));
			else // 25% chance to generate shield
				inventory->addItem(generateWeapon(level, Weapon::WeaponType::SHIELD));
		}
		else // 25% chance to generate armour
			inventory->addItem(generateArmour(level));
	}

	HarmlessNPC trader(
		level,
		Statistics(),
		NamedNPC(
			randomFirstName,
			randomLastName
		),
		move(inventory)
	);

	return make_unique<HarmlessNPC>(trader);
}

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

unique_ptr<Weapon> generateWeapon(int tier, Weapon::WeaponType ttype)
{
	int minValue, maxValue;
	switch (tier)
	{
	case 1:
		minValue = MIN_VALUE_ITEM_LEVEL1;
		maxValue = MAX_VALUE_ITEM_LEVEL1;
		break;
	case 2:
		minValue = MIN_VALUE_ITEM_LEVEL2;
		maxValue = MAX_VALUE_ITEM_LEVEL2;
		break;
	case 3:
		minValue = MIN_VALUE_ITEM_LEVEL3;
		maxValue = MAX_VALUE_ITEM_LEVEL3;
		break;
	case 4:
		minValue = MIN_VALUE_ITEM_LEVEL4;
		maxValue = MAX_VALUE_ITEM_LEVEL4;
		break;
	case 5:
		minValue = MIN_VALUE_ITEM_LEVEL5;
		maxValue = MAX_VALUE_ITEM_LEVEL5;
		break;
	default:
		throw out_of_range("Invalid weapon level. Must be between 1 and 5");
	}
	int value = minValue + (rand() % (maxValue - minValue + 1));

	if (ttype != Weapon::SHIELD)
	{
		Weapon::WeaponType newWeaponType = ttype != Weapon::NUMBER ? ttype : Weapon::WeaponType(rand() % (Weapon::NUMBER - 1));

		int handsNeededForWeapon = 1;
		// Axe and spear are two handed weapons, they have double the damage and value but occupy two weapon slots
		if (newWeaponType == Weapon::WeaponType::AXE || newWeaponType == Weapon::WeaponType::SPEAR)
		{
			handsNeededForWeapon = 2;
			value *= 2;
		}

		int maxStrAdditionPerc = getWeaponScaleLimit(newWeaponType, Attribute::STRENGTH, Limit::MAX),
			maxDexAdditionPerc = getWeaponScaleLimit(newWeaponType, Attribute::DEXTERITY, Limit::MAX);

		return make_unique<Weapon>(
			Item(
				Item::ItemType::WEAPON,
				value
			),
			tier,
			(MIN_WEAPON_DAMAGE + rand() % WEAPON_RAND_DAM_ADDITION) * handsNeededForWeapon, // Damage
			// `Weapon::NUMBER - 1` is a shield
			newWeaponType, // Type
			0, // Damage addition
			(maxStrAdditionPerc * 3 / 5 + (rand() % (maxStrAdditionPerc / 10 + 1))) * (tier * (100 / MAX_WEAPON_TIER)) / 100, // Strength damage addition
			(maxDexAdditionPerc * 3 / 5 + (rand() % (maxDexAdditionPerc / 10 + 1))) * (tier * (100 / MAX_WEAPON_TIER)) / 100, // Dexterity damage addition
			0, // Shield's probability addition
			0, // Shield's defense percent addition
			"" // Name
		);
	}
	else // A shield only created manually
		return make_unique<Weapon>(
			Item(
				Item::ItemType::WEAPON,
				value
			),
			tier,
			0, // Damage
			Weapon::SHIELD,
			0, // Damage addition
			0, // Strength percent addition
			0, // Dexterity percent addition
			(MIN_SHIELD_PROB_ADDITION + rand() % SHIELD_RAND_PROB_ADDITION) * (tier * (100 / MAX_WEAPON_TIER)) / 100, // Shield's probability addition
			(MIN_SHIELD_DEF_PERC_ADDITION + rand() % SHIELD_RAND_DEF_PERC_ADDITION) * (tier * (100 / MAX_WEAPON_TIER)) / 100, // Shield's defense percent addition
			"" // Name
		);
}

unique_ptr<Armour> generateArmour(int tier, Armour::ArmourType ttype)
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

	int minValue, maxValue;
	switch (tier)
	{
	case 1:
		minValue = MIN_VALUE_ITEM_LEVEL1;
		maxValue = MAX_VALUE_ITEM_LEVEL1;
		break;
	case 2:
		minValue = MIN_VALUE_ITEM_LEVEL2;
		maxValue = MAX_VALUE_ITEM_LEVEL2;
		break;
	case 3:
		minValue = MIN_VALUE_ITEM_LEVEL3;
		maxValue = MAX_VALUE_ITEM_LEVEL3;
		break;
	case 4:
		minValue = MIN_VALUE_ITEM_LEVEL4;
		maxValue = MAX_VALUE_ITEM_LEVEL4;
		break;
	case 5:
		minValue = MIN_VALUE_ITEM_LEVEL5;
		maxValue = MAX_VALUE_ITEM_LEVEL5;
		break;
	default:
		throw out_of_range("Invalid armour level. Must be between 1 and 5");
	}
	int value = minValue + (rand() % (maxValue - minValue + 1));

	return make_unique<Armour>(
		Item(
			Item::ItemType::ARMOUR,
			value
		),
		tier,
		MIN_ARMOUR_DEFENSE + 5 + rand() % ARMOUR_RAND_DEF_ADDITION, // Defense
		type,
		0, // Defense addition
		(maxStrDefAddition * 3 / 5 + (rand() % maxStrDefAddition / 10 + 1)) * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Strength defense addition
		(maxDexDefAddition * 3 / 5 + (rand() % maxDexDefAddition / 10 + 1)) * (tier * (100 / MAX_ARMOUR_TIER)) / 100, // Dexterity defense addition
		evasionProbAddition,
		stunProbSubstraction
	);
}

//void playSound(SoundEnum soundEnum)
//{
//	 1. Open file
//	 Compose path to sound based on enum
//	const string DIRECTORY = "Data/Sound/";
//	const string FORMAT = ".mp3";
//	string path("");
//
//	switch (soundEnum)
//	{
//	default:case SoundEnum::SOUND_BUTTON_CLICK: path = DIRECTORY + "buttonClick" + FORMAT; break;
//	}
//
//	 Compose full command string
//	buf = "open \"" + path + "\" type mpegvideo alias sound";
//	 Create command string
//	const char* command = buf.c_str();
//	MCIERROR error = mciSendString(command, NULL, 0, NULL);
//
//	 TODO: handle error
//	if (error)
//	{
//		char errorText[128];
//		mciGetErrorString(error, errorText, sizeof(errorText));
//		return;
//	}
//
//	 2. Play sound
//	command = "play sound";
//
//	error = mciSendString(command, NULL, 0, NULL);
//
//	 TODO: jandle error
//	if (error)
//	{
//		char errorText[128];
//		mciGetErrorString(error, errorText, sizeof(errorText));
//		return;
//	}
//}

// __________ Game process __________

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

		// Setting language (default = English)
		buf = "Data/Language/Settings.conf";
		if (std::experimental::filesystem::exists(buf))
		{
			ifstream fin(buf, ios::binary);
			int savedLanguage;
			fin >> savedLanguage;
			fin.close();

			l.setLanguage(static_cast<Language>(savedLanguage));
		}
		else
			l.setLanguage();
		// Setting menu
		game.getMenuManager().setMenu(new MainMenu(hwnd));
		break;

	case WM_COMMAND:
		// Processing user input with desired function accordingly to selected menu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_LBUTTONDOWN:
		// Processing user input with desired function accordingly to selected menu
		// Used to hide additional windows when user clicks on an empty space
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_SIZE:
		// Resizing and rearranging windows on screen accordingly to selected menu
		sx = LOWORD(lParam);
		sy = HIWORD(lParam);
		game.resizeWindow(sx / 2, sy / 2);
		break;

	case WM_PAINT:
		// Drawing background and changing text on windows accordingly to selected menu
		hdc = BeginPaint(hwnd, &ps);
		game.drawWindow(hwnd, hdc, sx / 2, sy / 2);
		EndPaint(hwnd, &ps);
		break;

	case WM_DRAWITEM:
	{
		// Drawing windows style accordingly to selected menu
		// If item has a custom style it is drawn using stylizeWindow()
		// Otherwise it is drawn using generic style
		if (!game.stylizeWindow(hwnd, message, wParam, lParam))
		{
			LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lParam;
			HDC hdc = item->hDC;

			GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

			// Set text font and background
			SelectObject(hdc, game.getFont(Game::FontSize::MEDIUM));
			SetBkMode(hdc, TRANSPARENT);

			// Get text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

			SetTextColor(hdc, COLOR_WHITE); // Set basic text color

			if (item->CtlType == ODT_STATIC) // Static windows
			{
				//SetTextColor(hdc, RGB(0,0,0)); // Set basic text color
				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
			}
			else if (strcmp(str, ("Edit")) == 0) // Edit windows
			{
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT);
			}
			else if (item->CtlType == ODT_BUTTON) // Button windows
			{
				if (item->itemState & ODS_SELECTED)
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
				}
				else
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED)); // Fill background
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT); // Draw edge
				}
			}
		}
		return true;
	}
	break;

	case WM_MOUSEMOVE:
		// TODO: improve | delete ?
		// An attempt to handle user placing cursor over button without clicking. Currently only used in MainMenu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_MOUSEHOVER:
		// TODO: improve | delete ?
		// An attempt to handle user placing cursor over button without clicking. Currently only used in MainMenu
		game.handleInput(hwnd, message, wParam, lParam);
		break;

	case WM_MOUSELEAVE:
		// TODO: improve | delete ?
		// An attempt to handle user placing cursor over button without clicking. Currently only used in MainMenu
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
