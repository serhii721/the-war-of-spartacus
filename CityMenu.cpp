#include "stdafx.h"
#include "CityMenu.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

CityMenu::CityMenu() :
	hItems(Item::ITEM_NUMBER),
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	currentSubMenuItem(ArenaItem::ARENA_ITEM_NUMBER),
	hSubMenuItems(),
	selectedOpponent(-1),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	pas()
{ }

CityMenu::CityMenu(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	currentSubMenuItem(ArenaItem::ARENA_ITEM_NUMBER),
	selectedOpponent(-1),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	pas()
{
	// Getting the city name
	buf = "City of " + localization.getCityName(game.getWorldMap().getCurrentCity());
	hItems[STAT_CITY_NAME] = CreateWindow("STATIC", buf.c_str(), // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_CENTER,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	logStr = "Messages log:\r\n\r\n";
	hItems[EDIT_MESSAGES_LOG] = CreateWindow("EDIT", logStr.c_str(),
		WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	char className[256] = "BUTTON";
	hItems[BUT_ARENA] = CreateWindow(className, "Arena", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[BUT_QUEST] = CreateWindow(className, "Quest", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MARKET] = CreateWindow(className, "Trader", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_CHARACTER] = CreateWindow(className, "View character", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_REST] = CreateWindow(className, "Rest", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MAP] = CreateWindow(className, "Map", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MENU] = CreateWindow(className, "Menu", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

CityMenu::CityMenu(const CityMenu& CM) :
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	currentSubMenuItem(ArenaItem::ARENA_ITEM_NUMBER),
	hSubMenuItems(),
	selectedOpponent(-1),
	pas()
{
	// Resizing items' vector
	int sz = CM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = CM.hItems[i];
		GetClassName(hItem, className, sizeof(className));
		GetWindowText(hItem, windowText, sizeof(windowText));
		GetWindowRect(hItem, &rect);

		// Creating new window
		hItems[i] = CreateWindow(
			className,
			windowText,
			GetWindowLongPtr(hItem, GWL_STYLE),
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			GetParent(hItem),
			0,
			hInst,
			0
		);
	}

	if (CM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(CM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (CM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(CM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

CityMenu& CityMenu::operator=(const CityMenu& CM)
{
	if (&CM == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = CM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = CM.hItems[i];
		GetClassName(hItem, className, sizeof(className));
		GetWindowText(hItem, windowText, sizeof(windowText));
		GetWindowRect(hItem, &rect);

		// Creating new window
		hItems[i] = CreateWindow(
			className,
			windowText,
			GetWindowLongPtr(hItem, GWL_STYLE),
			rect.left,
			rect.top,
			rect.right - rect.left,
			rect.bottom - rect.top,
			GetParent(hItem),
			0,
			hInst,
			0
		);
	}

	currentSubMenu = CM.currentSubMenu;
	hSubItems = vector<HWND>();

	currentSubMenuItem = CM.currentSubMenuItem;
	hSubMenuItems = vector<HWND>();

	selectedOpponent = CM.selectedOpponent;

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (CM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(CM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (CM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(CM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	pas = CM.pas;

	return *this;
}

CityMenu::~CityMenu()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	for (HWND hItem : hSubItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	for (HWND hItem : hSubMenuItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

void CityMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/Background/";
	const string FORMAT = ".bmp";
	string path("");
	int i;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// 1. Background
	// Composing path based on current menu
	switch (currentSubMenu)
	{
	default:case ITEM_NUMBER:
		path = DIRECTORY + "cityBackground768" + FORMAT;
		buf = "City of " + localization.getCityName(game.getWorldMap().getCurrentCity());
		SendMessage(hItems[STAT_CITY_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		break;
	case BUT_ARENA:
		switch (currentSubMenuItem)
		{
		default:case ARENA_ITEM_NUMBER: path = DIRECTORY + "arenaBackground768" + FORMAT; break;
		case ARENA_BUT_FIGHT: path = DIRECTORY + "arenaFightBackground768" + FORMAT; break;
		case ARENA_BUT_BET: path = DIRECTORY + "arenaBetBackground768" + FORMAT; break;
		}
		break;
	case BUT_QUEST: path = DIRECTORY + "questBackground768" + FORMAT; break;
	case BUT_MARKET: path = DIRECTORY + "marketBackground768" + FORMAT; break;
	case BUT_CHARACTER: path = DIRECTORY + "characterInventoryBackground768" + FORMAT; break;
	}

	// Loading image
	hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Filling background with selected image
	hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
	FillRect(hdc, &rect, hBackgroundBrush);

	// 2. Text
	// Updating log
	SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)(TCHAR*)logStr.c_str());
	SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
	switch (currentSubMenu)
	{
	default:case ITEM_NUMBER:
		for (HWND hItem : hItems)
			ShowWindow(hItem, SW_SHOW);
		break;
	case BUT_ARENA:
		switch (currentSubMenuItem)
		{
		default: case ARENA_ITEM_NUMBER:
			break;

		case ARENA_BUT_FIGHT:
		{
			// # 1. Player
			Player& rPlayer = game.getPlayer();
			// Name
			buf = rPlayer.getName();
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			// Level
			buf = "Level: " + to_string(rPlayer.getLevel());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			// Right hand
			if (rPlayer.getRightHand())
			{
				buf = localization.getWeaponTypeName(*rPlayer.getRightHand());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Damage: " + to_string(rPlayer.getRightHand()->getTotalDamage());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// Left hand
			if (rPlayer.getLeftHand())
			{
				buf = localization.getWeaponTypeName(*rPlayer.getLeftHand());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				if (rPlayer.getLeftHand()->getType() != Weapon::Type::SHIELD)
				{
					buf = "Damage: " + to_string(rPlayer.getLeftHand()->getTotalDamage());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				else
				{
					buf = "Block defense: " + to_string(rPlayer.getLeftHand()->getShieldDefPercentAddition());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
			}
			else
			{
				buf = "Empty hand";
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
			}

			// Health
			buf = "Health: " + to_string(rPlayer.getHP());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			// Armour
			if (rPlayer.getArmour())
			{
				buf = localization.getArmourTypeName(*rPlayer.getArmour());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Armour defense: " + to_string(rPlayer.getArmour()->getTotalDefense());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// Stats
			buf = "Strength: " + to_string(rPlayer.getStrength());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Constitution: " + to_string(rPlayer.getConstitution());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Dexterity: " + to_string(rPlayer.getDexterity());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Inteliigence: " + to_string(rPlayer.getIntelligence());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Wisdom: " + to_string(rPlayer.getWisdom());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Charisma: " + to_string(rPlayer.getCharisma());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Age: " + to_string(rPlayer.getAge());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Fame: " + to_string(rPlayer.getFame());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			// # 2. Opponent
			if (selectedOpponent != -1)
			{
				for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
					ShowWindow(hSubMenuItems[i], SW_SHOW);
				NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

				// Name
				buf = localization.getNPCName(rOpponent);
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				// Level
				buf = "Level: " + to_string(rOpponent.getLevel());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				// Right hand
				if (rOpponent.getRightHand())
				{
					buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}

				// Left hand
				if (rOpponent.getLeftHand())
				{
					buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
					{
						buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}
					else
					{
						buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}
				}
				else
				{
					buf = "Empty hand";
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
				}

				// Health
				buf = "Health: " + to_string(rOpponent.getHP());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				// Armour
				if (rOpponent.getArmour())
				{
					buf = localization.getArmourTypeName(*rOpponent.getArmour());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}

				// Stats
				buf = "Strength: " + to_string(rOpponent.getStrength());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Constitution: " + to_string(rOpponent.getConstitution());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Dexterity: " + to_string(rOpponent.getDexterity());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Wisdom: " + to_string(rOpponent.getWisdom());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Charisma: " + to_string(rOpponent.getCharisma());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Age: " + to_string(rOpponent.getAge());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Fame: " + to_string(rOpponent.getFame());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			else
				for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
					ShowWindow(hSubMenuItems[i], SW_HIDE);
			break;
		}
		break;
		}
		break;
	case BUT_CHARACTER:
	{
		Player& rPlayer = game.getPlayer();
		// TODO: Apply localization
		// Name
		buf = rPlayer.getName();
		SendMessage(hSubItems[CHARACTER_STAT_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Level
		buf = "Level: " + to_string(rPlayer.getLevel());
		SendMessage(hSubItems[CHARACTER_STAT_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Experience
		buf = "Experience: " + to_string(rPlayer.getExperience()) + " / 200";
		SendMessage(hSubItems[CHARACTER_STAT_EXPERIENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Age
		buf = "Age: " + to_string(rPlayer.getAge());
		SendMessage(hSubItems[CHARACTER_STAT_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Fame
		buf = "Fame: " + to_string(rPlayer.getFame());
		SendMessage(hSubItems[CHARACTER_STAT_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Health
		buf = "Health: (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
		SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Stats
		buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
		SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Strength: " + to_string(pas.strength);
		SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Constitution: " + to_string(pas.constitution);
		SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Dexterity: " + to_string(pas.dexterity);
		SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Inteliigence: " + to_string(pas.intelligence);
		SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Wisdom: " + to_string(pas.wisdom);
		SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Charisma: " + to_string(pas.charisma);
		SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Right hand
		if (rPlayer.getRightHand())
		{
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_SHOW);
			Weapon& rRightHand = *rPlayer.getRightHand();
			buf = localization.getWeaponTypeName(rRightHand);
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Damage: " + to_string(rRightHand.getTotalDamage());
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Strength scale: " + to_string(rRightHand.getStrengthAdditionPercentage());
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Dexterity scale: " + to_string(rRightHand.getDexterityAdditionPercentage());
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"Right hand");
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_HIDE);
		}

		// Left hand
		if (rPlayer.getLeftHand())
		{
			Weapon& rLeftHand = *rPlayer.getLeftHand();
			buf = localization.getWeaponTypeName(rLeftHand);
			SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			if (rLeftHand.getType() != Weapon::Type::SHIELD)
			{
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_SHOW);

				buf = "Damage: " + to_string(rLeftHand.getTotalDamage());
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Strength scale: " + to_string(rLeftHand.getStrengthAdditionPercentage());
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Dexterity scale: " + to_string(rLeftHand.getDexterityAdditionPercentage());
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_HIDE);
			}
			else
			{
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_SHOW);

				buf = "Block defense: " + to_string(rLeftHand.getShieldDefPercentAddition());
				SendMessage(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Block chance: " + to_string(rLeftHand.getShieldProbAddition());
				SendMessage(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
			}
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"Left hand");
			ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_HIDE);
		}

		// Armour
		if (rPlayer.getArmour())
		{
			Armour& rArmour = *rPlayer.getArmour();
			buf = localization.getArmourTypeName(rArmour);
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Defense: " + to_string(rArmour.getTotalDefense());
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Strength scale: " + to_string(rArmour.getStrengthAdditionPercentage());
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = "Dexterity scale: " + to_string(rArmour.getDexterityAdditionPercentage());
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			if (rPlayer.getArmour()->getType() == Armour::Type::LIGHT)
				buf = "Evasion chance: " + to_string(rArmour.getEvasionProbAddition());
			else
				buf = "Stun resistance chance: " + to_string(rArmour.getStunProbSubtraction());
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"No armour");
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], SW_HIDE);
		}

		// Buttons
		for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
			SendMessage(hSubItems[i], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"+");

		for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
			SendMessage(hSubItems[i], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"-");

		buf = "Reset changes";
		SendMessage(hSubItems[CHARACTER_BUT_RESET_CHANGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Apply changes";
		SendMessage(hSubItems[CHARACTER_BUT_APPLY_CHANGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = "Back";
		SendMessage(hSubItems[CHARACTER_BUT_BACK], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Buttons display
		// Increasing attributes
		if (pas.unnassignedAttributes > 0)
		{
			for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
				ShowWindow(hSubItems[i], SW_SHOW);
		}
		else
		{
			for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
				ShowWindow(hSubItems[i], SW_HIDE);
		}
		// Decreasing attributes
		// Strength
		if (pas.strength > rPlayer.getStrength())
			ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_HIDE);
		// Constitution
		if (pas.constitution > rPlayer.getConstitution())
			ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_HIDE);
		// Dexterity
		if (pas.dexterity > rPlayer.getDexterity())
			ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_HIDE);
		// Intelligence
		if (pas.intelligence > rPlayer.getIntelligence())
			ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_HIDE);
		// Wisdom
		if (pas.wisdom > rPlayer.getWisdom())
			ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_HIDE);
		// Charisma
		if (pas.charisma > rPlayer.getCharisma())
			ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_SHOW);
		else
			ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_HIDE);

		// Apply changes, reset changes
		if (pas.strength > rPlayer.getStrength() ||
			pas.constitution > rPlayer.getConstitution() ||
			pas.dexterity > rPlayer.getDexterity() ||
			pas.intelligence > rPlayer.getIntelligence() ||
			pas.wisdom > rPlayer.getWisdom() ||
			pas.charisma > rPlayer.getCharisma()
			)
		{
			ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
		}
		else
		{
			ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
		}

		break;
	}
	}
}

void CityMenu::resizeMenu(int cx, int cy)
{
	int sz, x, y, i;
	switch (currentSubMenu)
	{
		// City menu
	case Item::ITEM_NUMBER:
	{
		const int ITEM_HEIGHT = 60, DISTANCE = 15, ITEM_WIDTH = 300;
		sz = hItems.size();
		x = cx - ITEM_WIDTH * 2;
		y = cy - (int)(sz / 1.7 * (ITEM_HEIGHT + DISTANCE));
		y += (ITEM_HEIGHT + DISTANCE) / 2;
		for (HWND hItem : hItems)
		{
			if (hItem == hItems[BUT_MAP])
				y += (ITEM_HEIGHT + DISTANCE) / 2;
			MoveWindow(hItem, x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
			y += ITEM_HEIGHT + DISTANCE;
		}
		MoveWindow(hItems[EDIT_MESSAGES_LOG], 830, 510, 500, 200, TRUE);
	}
	break;

	// Arena
	case Item::BUT_ARENA:
	{
		switch (currentSubMenuItem)
		{
		default:case ArenaItem::ARENA_ITEM_NUMBER:
		{
			const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 60, DISTANCE = 15;
			cy -= 3 * (ITEM_HEIGHT + DISTANCE);
			y = cy;
			x = cx - BUT_WIDTH * 2;

			for (i = ARENA_BUT_FIGHT; i < 4; i++)
			{
				y += ITEM_HEIGHT + DISTANCE;
				MoveWindow(hSubItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			}
		}
		break;

		case ArenaItem::ARENA_BUT_FIGHT:
		{
			const int STAT_WIDTH = 170, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 30, DISTANCE = 9;
			//cy -= 3 * (ITEM_HEIGHT + DISTANCE);
			y = DISTANCE * 2 + ITEM_HEIGHT * 2;
			x = cx - BUT_WIDTH * 2 - DISTANCE * 2;

			// Opponents list
			for (i = ARENA_FIGHT_BUT_OPPONENT1; i <= ARENA_FIGHT_BUT_OPPONENT15; i++)
			{
				MoveWindow(hSubMenuItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
				y += ITEM_HEIGHT + DISTANCE;
			}

			// Player's stats
			y = DISTANCE * 2 + ITEM_HEIGHT * 2;
			x = cx + (int)(BUT_WIDTH * 0.5) + DISTANCE * 12;

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], x, y - ITEM_HEIGHT - DISTANCE, STAT_WIDTH, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], x, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);

			// RightHand stats
			y = cy - (ITEM_HEIGHT + DISTANCE) * 3;
			x -= 120;
			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

			// LeftHand stats
			x += 40 + STAT_WIDTH;
			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

			// Other stats
			x -= STAT_WIDTH - 80;
			y = cy;
			for (i = ARENA_FIGHT_STATIC_PLAYER_HEALTH; i <= ARENA_FIGHT_STATIC_PLAYER_FAME; i++)
			{
				MoveWindow(hSubMenuItems[i], x, y, STAT_WIDTH, ITEM_HEIGHT - 10, TRUE);
				y += ITEM_HEIGHT - 10 + DISTANCE / 2;
				if (i == ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE || i == ARENA_FIGHT_STATIC_PLAYER_CHARISMA)
					y += DISTANCE;
			}

			// Opponent's stats
			y = DISTANCE * 2 + ITEM_HEIGHT * 2;
			x = cx + (int)(BUT_WIDTH * 0.5) + DISTANCE - 340;
			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], x, y - ITEM_HEIGHT - DISTANCE, STAT_WIDTH, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], x, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);

			// LeftHand stats
			y = cy - (ITEM_HEIGHT + DISTANCE) * 3;
			x -= 120;
			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

			// RightHand stats
			x += 40 + STAT_WIDTH;
			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

			// Other stats
			x -= STAT_WIDTH - 80;
			y = cy;
			for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
			{
				MoveWindow(hSubMenuItems[i], x, y, STAT_WIDTH, ITEM_HEIGHT - 10, TRUE);
				y += ITEM_HEIGHT - 10 + DISTANCE / 2;
				if (i == ARENA_FIGHT_STATIC_ARMOUR_DEFENSE || i == ARENA_FIGHT_STATIC_CHARISMA)
					y += DISTANCE;
			}

			// Back and Fight buttons
			x = 374;
			y = 672;

			MoveWindow(hSubMenuItems[ARENA_FIGHT_BUT_BACK], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_BUT_FIGHT], x + BUT_WIDTH + DISTANCE, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
		}
		break;

		case ArenaItem::ARENA_BUT_BET:
			// TODO
			break;
		}
	}
	break;

	// Quest
	case Item::BUT_QUEST:
	{
		// TODO
	}
	break;

	// Trader
	case Item::BUT_MARKET:
	{
		// TODO
	}
	break;
	// Character stats and inventory
	case Item::BUT_CHARACTER:
	{
		const int BIG_STAT_WIDTH = 200, BIG_STAT_HEIGHT = 30,
				SMALL_STAT_WIDTH = 170, SMALL_STAT_HEIGHT = 20,
				BIG_DISTANCE = 9, SMALL_DISTANCE = 4,
				BUT_SIZE = 20;

		// Player's stats
		x = 87, y = 68;
		// Name, level, experience
		for (i = CHARACTER_STAT_NAME; i <= CHARACTER_STAT_EXPERIENCE; i++)
		{
			MoveWindow(hSubItems[i], x, y, SMALL_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Age, Fame, health
		y += 205;
		for (i = CHARACTER_STAT_AGE; i <= CHARACTER_STAT_HEALTH; i++)
		{
			MoveWindow(hSubItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + SMALL_DISTANCE;
		}

		// Attributes
		y += BIG_DISTANCE - SMALL_DISTANCE;
		int y2 = y + SMALL_STAT_HEIGHT + SMALL_DISTANCE;

		MoveWindow(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], x, y, SMALL_STAT_WIDTH + (SMALL_DISTANCE + BUT_SIZE) * 2, SMALL_STAT_HEIGHT, TRUE);
		y += SMALL_STAT_HEIGHT + SMALL_DISTANCE;

		for (i = CHARACTER_STAT_STRENGTH; i <= CHARACTER_STAT_CHARISMA; i++)
		{
			MoveWindow(hSubItems[i], x, y, SMALL_STAT_WIDTH, SMALL_STAT_HEIGHT, TRUE);
			y += SMALL_STAT_HEIGHT + SMALL_DISTANCE;
		}

		// Minus and plus buttons
		x += SMALL_STAT_WIDTH + SMALL_DISTANCE;
		for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
		{
			// Minus button
			MoveWindow(hSubItems[i], x, y2, BUT_SIZE, BUT_SIZE, TRUE);
			// Plus button
			MoveWindow(hSubItems[i - 6], x + BUT_SIZE + SMALL_DISTANCE, y2, BUT_SIZE, BUT_SIZE, TRUE);
			y2 += SMALL_STAT_HEIGHT + SMALL_DISTANCE;
		}

		// Reset button
		x -= SMALL_STAT_WIDTH + SMALL_DISTANCE;
		y += SMALL_DISTANCE * 3;
		MoveWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], x, 672, SMALL_STAT_WIDTH + (SMALL_DISTANCE + BUT_SIZE) * 2, BIG_STAT_HEIGHT, TRUE);

		// Left hand
		x = 364, y = 195;
		for (i = CHARACTER_STAT_LEFT_HAND_TYPE; i <= CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubItems[i], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Right hand
		x += BIG_STAT_WIDTH + 10;
		y = 195;
		for (i = CHARACTER_STAT_RIGHT_HAND_TYPE; i <= CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE; i++)
		{
			MoveWindow(hSubItems[i], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Armour
		x += BIG_STAT_WIDTH + 10;
		y = 195;
		for (i = CHARACTER_STAT_ARMOUR_TYPE; i <= CHARACTER_STAT_ARMOUR_ABILITY; i++)
		{
			MoveWindow(hSubItems[i], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			y += BIG_STAT_HEIGHT + BIG_DISTANCE;
		}

		// Apply changes
		MoveWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], 682, 672, 300, BIG_STAT_HEIGHT, TRUE);

		// Back button
		MoveWindow(hSubItems[CHARACTER_BUT_BACK], 364, 672, 300, BIG_STAT_HEIGHT, TRUE);
	}
	break;
	}
}

void CityMenu::updateLanguage()
{
	// TODO: update localization
}

void CityMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);
	static bool isExhausted = false;

	switch (m)
	{
	case WM_COMMAND:
		switch (currentSubMenu)
		{
		default:case ITEM_NUMBER:
			if ((HWND)lp == hItems[BUT_ARENA])
			{
				// Hiding all windows except log
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);
				ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_SHOW);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(ARENA_ITEM_NUMBER);

				// TODO: Apply localization
				hSubItems[ARENA_BUT_FIGHT] = CreateWindow("BUTTON", "Fight",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BET] = CreateWindow("BUTTON", "Bet",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_TRAIN] = CreateWindow("BUTTON", "Train",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BACK] = CreateWindow("BUTTON", "Back",
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);

				currentSubMenu = BUT_ARENA;

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_QUEST])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_MARKET])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_CHARACTER])
			{
				// Hiding all windows
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(CHARACTER_ITEM_NUMBER);

				int i;
				for (i = CHARACTER_STAT_NAME; i <= CHARACTER_STAT_CHARISMA; i++)
					hSubItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_BACK; i++)
					hSubItems[i] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				Player& rPlayer = game.getPlayer();
				pas.unnassignedAttributes = rPlayer.getUnnassignedAttributes();
				pas.strength = rPlayer.getStrength();
				pas.constitution = rPlayer.getConstitution();
				pas.dexterity = rPlayer.getDexterity();
				pas.intelligence = rPlayer.getIntelligence();
				pas.wisdom = rPlayer.getWisdom();
				pas.charisma = rPlayer.getCharisma();

				currentSubMenu = BUT_CHARACTER;

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_REST])
			{
				Player& rPlayer = game.getPlayer();
				if (rPlayer.getHP() < rPlayer.getFullHP())
				{
					int regen = BASIC_REGEN * rPlayer.getFullHP() / 100;

					if (rPlayer.getHP() + regen >= rPlayer.getFullHP())
					{
						regen = rPlayer.getFullHP() - rPlayer.getHP();
						rPlayer.setHP(rPlayer.getFullHP());
						logStr += "You have rested and restored " + to_string(regen) + " health points\r\n"
							+ "You are fully healed (" + to_string(rPlayer.getFullHP()) + " hp)\r\n\r\n";
					}
					else
					{
						rPlayer.setHP(rPlayer.getHP() + regen);
						logStr += "You have rested and restored " + to_string(regen) + " health points (" + to_string(rPlayer.getHP()) + " hp)\r\n\r\n";
					}
					isExhausted = false;
				}
				else
					logStr += "You are fully healed and don't need to rest\r\n\r\n";
				SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)(TCHAR*)logStr.c_str());
				SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
			}
			if ((HWND)lp == hItems[BUT_MAP])
			{
				// Destroying all buttons
				for (HWND hItem : hSubMenuItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubMenuItems.clear();

				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				game.setDisplayState(DisplayState::WORLD_MAP);
				updateWindow(hWnd);

				currentSubMenuItem = ARENA_ITEM_NUMBER;
				currentSubMenu = ITEM_NUMBER;
				break;
			}
			if ((HWND)lp == hItems[BUT_MENU] || LOWORD(wp) == IDCANCEL)
			{
				game.getMenuManager().setMenu(new GameMenu(hWnd));
				updateWindow(hWnd);
			}
			break;

		case BUT_ARENA:
			switch (currentSubMenuItem)
			{
			default: case ArenaItem::ARENA_ITEM_NUMBER:
				if ((HWND)lp == hSubItems[ARENA_BUT_FIGHT])
				{
					// Hiding all windows
					ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_HIDE);
					for (HWND hItem : hSubItems)
						ShowWindow(hItem, SW_HIDE);

					// Erasing previous sub menu items
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();

					// Creating new sub menu items
					hSubMenuItems.resize(ARENA_FIGHT_ITEM_NUMBER);

					// Outputing player
					int i = ARENA_FIGHT_STATIC_NAME;
					for (; i <= ARENA_FIGHT_STATIC_PLAYER_FAME; i++)
						hSubMenuItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

					Player& rPlayer = game.getPlayer();
					// Name
					buf = rPlayer.getName();
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Level
					buf = "Level: " + to_string(rPlayer.getLevel());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Right hand
					if (rPlayer.getRightHand())
					{
						buf = localization.getWeaponTypeName(*rPlayer.getRightHand());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Damage: " + to_string(rPlayer.getRightHand()->getTotalDamage());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					// Left hand
					if (rPlayer.getLeftHand())
					{
						buf = localization.getWeaponTypeName(*rPlayer.getLeftHand());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						if (rPlayer.getLeftHand()->getType() != Weapon::Type::SHIELD)
						{
							buf = "Damage: " + to_string(rPlayer.getLeftHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}
						else
						{
							buf = "Block defense: " + to_string(rPlayer.getLeftHand()->getShieldDefPercentAddition());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}
					}
					else
					{
						buf = "Empty hand";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
					}

					// Health
					buf = "Health: " + to_string(rPlayer.getHP());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Armour
					if (rPlayer.getArmour())
					{
						buf = localization.getArmourTypeName(*rPlayer.getArmour());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Armour defense: " + to_string(rPlayer.getArmour()->getTotalDefense());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					// Stats
					buf = "Strength: " + to_string(rPlayer.getStrength());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Constitution: " + to_string(rPlayer.getConstitution());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Dexterity: " + to_string(rPlayer.getDexterity());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Inteliigence: " + to_string(rPlayer.getIntelligence());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Wisdom: " + to_string(rPlayer.getWisdom());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Charisma: " + to_string(rPlayer.getCharisma());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Age: " + to_string(rPlayer.getAge());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = "Fame: " + to_string(rPlayer.getFame());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Outputing opponents
					Arena& rArena = game.getWorldMap().getCurrentCity().getArena();
					for (i = ARENA_FIGHT_BUT_OPPONENT1; i <= ARENA_FIGHT_BUT_OPPONENT15; i++)
					{
						const NPC& rOpponent = *rArena.getGladiator(i);
						buf = localization.getNPCName(rOpponent) + " (lvl: " + to_string(rOpponent.getLevel()) + ")";
						hSubMenuItems[i] = CreateWindow("BUTTON",
							buf.c_str(),
							WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTORADIOBUTTON | BS_OWNERDRAW | BS_LEFTTEXT,
							0, 0, 0, 0, hWnd, 0, hInst, 0);
					}

					hSubMenuItems[ARENA_FIGHT_BUT_BACK] = CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
					hSubMenuItems[ARENA_FIGHT_BUT_FIGHT] = CreateWindow("BUTTON", "Fight", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

					currentSubMenuItem = ARENA_BUT_FIGHT;

					updateWindow(hWnd);
				}

				if ((HWND)lp == hSubItems[ARENA_BUT_BET])
				{
					// TODO
				}

				if ((HWND)lp == hSubItems[ARENA_BUT_TRAIN])
				{
					if (!isExhausted)
					{
						Player& rPlayer = game.getPlayer();
						int experience = rand() % 100 + 1;
						int level = rPlayer.getLevel();

						rPlayer.gainExperience(experience);
						logStr += "You have trained and gained " + to_string(experience) + " experience\r\n\r\n";
						if (level < rPlayer.getLevel())
							logStr += "You have leved up to level " +
							to_string(rPlayer.getLevel()) +
							" (" + to_string(rPlayer.getUnnassignedAttributes()) + " unnassigned attributes)\r\n\r\n";

						isExhausted = true;
					}
					else
						logStr += "You have to rest before you can train\r\n\r\n";
					SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)(TCHAR*)logStr.c_str());
					SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
				}

				if ((HWND)lp == hSubItems[ARENA_BUT_BACK] || LOWORD(wp) == IDCANCEL)
				{
					// Destroying all buttons
					for (HWND hItem : hSubItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubItems.clear();

					// Showing main menu buttons
					for (HWND hItem : hItems)
						ShowWindow(hItem, SW_SHOW);

					currentSubMenu = ITEM_NUMBER;

					updateWindow(hWnd);
					break;
				}
				break;

			case ArenaItem::ARENA_BUT_FIGHT:
			{
				// Selecting opponent
				{
					int i;
					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT1])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT1;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT2])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT2;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT3])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT3;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT4])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT4;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT5])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT5;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT6])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT6;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT7])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT7;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT8])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT8;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT9])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT9;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT10])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT10;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT11])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT11;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT12])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT12;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT13])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT13;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT14])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT14;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT15])
					{
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT15;
						NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

						for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
							ShowWindow(hSubMenuItems[i], SW_SHOW);
						// Outputing opponent's information
						// Name
						buf = localization.getNPCName(rOpponent);
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Level
						buf = "Level: " + to_string(rOpponent.getLevel());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Right hand
						if (rOpponent.getRightHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getRightHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Damage: " + to_string(rOpponent.getRightHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Left hand
						if (rOpponent.getLeftHand())
						{
							buf = localization.getWeaponTypeName(*rOpponent.getLeftHand());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							if (rOpponent.getLeftHand()->getType() != Weapon::Type::SHIELD)
							{
								buf = "Damage: " + to_string(rOpponent.getLeftHand()->getTotalDamage());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
							else
							{
								buf = "Block defense: " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
								SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							}
						}
						else
						{
							buf = "Empty hand";
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
						}

						// Health
						buf = "Health: " + to_string(rOpponent.getHP());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						// Armour
						if (rOpponent.getArmour())
						{
							buf = localization.getArmourTypeName(*rOpponent.getArmour());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

							buf = "Armour defense: " + to_string(rOpponent.getArmour()->getTotalDefense());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}

						// Stats
						buf = "Strength: " + to_string(rOpponent.getStrength());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Constitution: " + to_string(rOpponent.getConstitution());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Dexterity: " + to_string(rOpponent.getDexterity());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Inteliigence: " + to_string(rOpponent.getIntelligence());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Wisdom: " + to_string(rOpponent.getWisdom());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Charisma: " + to_string(rOpponent.getCharisma());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Age: " + to_string(rOpponent.getAge());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = "Fame: " + to_string(rOpponent.getFame());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}
				}

				// Starting fight
				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_FIGHT])
				{
					if (selectedOpponent != -1 && game.getPlayer().getHP() > 30)
					{
						// Destroying all buttons
						for (HWND hItem : hSubMenuItems)
							if (hItem != NULL)
								DestroyWindow(hItem);
						hSubMenuItems.clear();

						game.setDisplayState(DisplayState::FIGHTING);
						game.getFighting().setScreen(Fighting::Screen::FIGHT_ARENA);
						game.setBackground(Game::Background::FIGHTING_ARENA);
						updateWindow(hWnd);
						game.getFighting().fight(
							hWnd,
							game.getPlayer(),
							game.getWorldMap().getCurrentCity().getArena()
							.getGladiator(selectedOpponent)
						);

						currentSubMenuItem = ArenaItem::ARENA_ITEM_NUMBER;
						currentSubMenu = Item::ITEM_NUMBER;
						break;
					}
					else if (selectedOpponent == -1)
						MessageBox(hWnd, "You have to choose opponent which you want to fight", "Opponent is not selected", MB_OK | MB_ICONINFORMATION);
					else
						MessageBox(hWnd, "You have to rest before you can fight", "You are heavily injured", MB_OK | MB_ICONINFORMATION);
				}

				// Return
				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_BACK] || LOWORD(wp) == IDCANCEL)
				{
					// Destroying all buttons
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();

					// Showing menu buttons
					for (HWND hItem : hSubItems)
						ShowWindow(hItem, SW_SHOW);
					ShowWindow(hItems[EDIT_MESSAGES_LOG], SW_SHOW);

					currentSubMenuItem = ARENA_ITEM_NUMBER;
					selectedOpponent = -1;

					updateWindow(hWnd);
					break;
				}
			}
				break;

			case ArenaItem::ARENA_BUT_BET:
				// TODO
				break;
			}
			break;
		case Item::BUT_CHARACTER:
			Player& rPlayer = game.getPlayer();
			int i;

			// Plus buttons
			// Strength
			if ((HWND)lp == hSubItems[CHARACTER_BUT_STRENGTH_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.strength++;
				pas.updateMaxHP();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Strength: " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Health: (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Constitution
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.constitution++;
				pas.updateMaxHP();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Constitution: " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Health: (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Dexterity
			if ((HWND)lp == hSubItems[CHARACTER_BUT_DEXTERITY_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.dexterity++;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Dexterity: " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Intelligence
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.intelligence++;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Inteliigence: " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Wisdom
			if ((HWND)lp == hSubItems[CHARACTER_BUT_WISDOM_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.wisdom++;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Wisdom: " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Charisma
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CHARISMA_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.charisma++;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Charisma: " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// Minus buttons
			// Strength
			if ((HWND)lp == hSubItems[CHARACTER_BUT_STRENGTH_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.strength--;
				pas.updateMaxHP();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
				if (pas.strength == rPlayer.getStrength())
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_HIDE);

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() ||
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
				}

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Strength: " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Health: (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Constitution
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.constitution--;
				pas.updateMaxHP();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
				if (pas.constitution == rPlayer.getConstitution())
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_HIDE);

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
				}

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Constitution: " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Health: (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Dexterity
			if ((HWND)lp == hSubItems[CHARACTER_BUT_DEXTERITY_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.dexterity--;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
				if (pas.dexterity == rPlayer.getDexterity())
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_HIDE);

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
				}

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Dexterity: " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Intelligence
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.intelligence--;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
				if (pas.intelligence == rPlayer.getIntelligence())
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_HIDE);

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
				}

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Inteliigence: " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Wisdom
			if ((HWND)lp == hSubItems[CHARACTER_BUT_WISDOM_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.wisdom--;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
				if (pas.wisdom == rPlayer.getWisdom())
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_HIDE);

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
				}

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Wisdom: " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Charisma
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CHARISMA_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.charisma--;

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
				if (pas.charisma == rPlayer.getCharisma())
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_HIDE);

				// Apply changes, reset changes
				if (pas.strength == rPlayer.getStrength() &&
					pas.constitution == rPlayer.getConstitution() &&
					pas.dexterity == rPlayer.getDexterity() &&
					pas.intelligence == rPlayer.getIntelligence() &&
					pas.wisdom == rPlayer.getWisdom() &&
					pas.charisma == rPlayer.getCharisma()
					)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
					ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
				}

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = "Charisma: " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// Reset changes
			if ((HWND)lp == hSubItems[CHARACTER_BUT_RESET_CHANGES])
			{
				pas.unnassignedAttributes = rPlayer.getUnnassignedAttributes();
				pas.strength = rPlayer.getStrength();
				pas.constitution = rPlayer.getConstitution();
				pas.dexterity = rPlayer.getDexterity();
				pas.intelligence = rPlayer.getIntelligence();
				pas.wisdom = rPlayer.getWisdom();
				pas.charisma = rPlayer.getCharisma();

				// Button appearance
				for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
					ShowWindow(hSubItems[i], SW_SHOW);

				for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);

				// Text
				buf = "Unnassigned attributes: " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Strength: " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Constitution: " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Dexterity: " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Inteliigence: " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Wisdom: " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Charisma: " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = "Health: (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// Apply changes
			if ((HWND)lp == hSubItems[CHARACTER_BUT_APPLY_CHANGES])
			{
				rPlayer.setUnnassignedAttributes(pas.unnassignedAttributes);
				rPlayer.setStrength(pas.strength);
				rPlayer.setConstitution(pas.constitution);
				rPlayer.setDexterity(pas.dexterity);
				rPlayer.setIntelligence(pas.intelligence);
				rPlayer.setWisdom(pas.wisdom);
				rPlayer.setCharisma(pas.charisma);
				rPlayer.setHP(pas.hp);
				rPlayer.setFullHP(pas.fullHP);

				// Button appearance
				if (rPlayer.getUnnassignedAttributes() > 0)
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_SHOW);

				for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);
			}

			if ((HWND)lp == hSubItems[CHARACTER_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				currentSubMenu = ITEM_NUMBER;

				updateWindow(hWnd);
				break;
			}
			break;
		}
		break;
	}	
}
