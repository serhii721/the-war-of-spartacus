#include "stdafx.h"
#include "CityMenu.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

CityMenu::CityMenu() :
	hItems(Item::ITEM_NUMBER),
	hSubItems(),
	hSubMenuItems(),
	selectedOpponent(-1),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	pas()
{ }

CityMenu::CityMenu(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	hSubItems(),
	selectedOpponent(-1),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL),
	pas()
{
	// Getting the city name
	buf = l.getMessage(Localized::CITY) + " " + l.getCityName(game.getWorldMap().getCurrentCity());
	hItems[STAT_CITY_NAME] = CreateWindow("STATIC", buf.c_str(),
		WS_CHILD | WS_VISIBLE | SS_OWNERDRAW | SS_CENTER,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[EDIT_MESSAGES_LOG] = CreateWindow("EDIT", logStr.c_str(),
		WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	char className[256] = "BUTTON";
	hItems[BUT_ARENA] = CreateWindow(className, l.getMessage(Localized::ARENA).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[BUT_QUEST] = CreateWindow(className, l.getMessage(Localized::QUEST).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MARKET] = CreateWindow(className, l.getMessage(Localized::MARKET).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_CHARACTER] = CreateWindow(className, l.getMessage(Localized::VIEW_CHARACTER).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_REST] = CreateWindow(className, l.getMessage(Localized::REST).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MAP] = CreateWindow(className, l.getMessage(Localized::MAP).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MENU] = CreateWindow(className, l.getMessage(Localized::MENU).c_str(),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

CityMenu::CityMenu(const CityMenu& CM) :
	hSubItems(),
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

	hSubItems = vector<HWND>();

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
	if (game.isBackgroundChanged())
	{
		// Composing path based on current menu
		switch (game.getBackground())
		{
		default:case Game::Background::CITY_MENU: path = DIRECTORY + "cityBackground768" + FORMAT; break;
		case Game::Background::CITY_MENU_ARENA: path = DIRECTORY + "arenaBackground768" + FORMAT; break;
		case Game::Background::CITY_MENU_ARENA_FIGHT: path = DIRECTORY + "arenaFightBackground768" + FORMAT; break;
		case Game::Background::CITY_MENU_ARENA_BET: path = DIRECTORY + "arenaBetBackground768" + FORMAT; break;
		case Game::Background::CITY_MENU_QUEST: path = DIRECTORY + "questBackground768" + FORMAT; break;
		case Game::Background::CITY_MENU_MARKET: path = DIRECTORY + "marketBackground768" + FORMAT; break;
		case Game::Background::CITY_MENU_CHARACTER: path = DIRECTORY + "characterInventoryBackground768" + FORMAT; break;
		}

		// Loading image
		hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		// Filling background with selected image
		hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
		FillRect(hdc, &rect, hBackgroundBrush);
	}

	// 2. Text
	switch (game.getBackground())
	{
	default:case Game::Background::CITY_MENU:
		for (HWND hItem : hItems)
			ShowWindow(hItem, SW_SHOW);
		buf = l.getMessage(Localized::CITY) + " " + l.getCityName(game.getWorldMap().getCurrentCity());
		SendMessage(hItems[STAT_CITY_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		// Updating log
		SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)(TCHAR*)logStr.c_str());
		SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
		break;

	case Game::Background::CITY_MENU_ARENA:
		// Updating log
		SendMessage(hItems[EDIT_MESSAGES_LOG], WM_SETTEXT, 0, (LPARAM)(TCHAR*)logStr.c_str());
		SendMessage(hItems[EDIT_MESSAGES_LOG], EM_SCROLL, SB_BOTTOM, 0);
		break;

	case Game::Background::CITY_MENU_ARENA_FIGHT:
	{
		if (game.isBackgroundChanged())
		{
			// # 1. Player
			{
				Player& rPlayer = game.getPlayer();
				// Name
				buf = rPlayer.getName();
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				// Level
				buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rPlayer.getLevel());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				// Right hand
				if (rPlayer.getRightHand())
				{
					buf = l.getWeaponTypeName(*rPlayer.getRightHand());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}

				// Left hand
				if (rPlayer.getLeftHand())
				{
					buf = l.getWeaponTypeName(*rPlayer.getLeftHand());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					if (rPlayer.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
					{
						buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}
					else
					{
						buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rPlayer.getLeftHand()->getShieldDefPercentAddition()) + "%";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rPlayer.getLeftHand()->getShieldProbAddition()) + "%";
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}
				}
				else
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::EMPTY_HAND).c_str());

					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
				}

				// Health
				buf = l.getMessage(Localized::HEALTH) + ": " + to_string(rPlayer.getHP());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				// Armour
				if (rPlayer.getArmour())
				{
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getArmourTypeName(*rPlayer.getArmour()).c_str());

					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}

				// Stats
				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(rPlayer.getStrength());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(rPlayer.getConstitution());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(rPlayer.getDexterity());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(rPlayer.getIntelligence());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(rPlayer.getWisdom());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(rPlayer.getCharisma());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::AGE) + ": " + to_string(rPlayer.getAge());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::FAME) + ": " + to_string(rPlayer.getFame());
				SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// # 2. Opponent
			if (selectedOpponent != -1)
			{
				NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

				for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_LEFT_HAND_TYPE; i++)
					ShowWindow(hSubMenuItems[i], SW_SHOW);
				if (rOpponent.getLeftHand())
				{
					if (rOpponent.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], SW_SHOW);
					else
					{
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_SHOW);
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_SHOW);
					}
				}
				for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
					ShowWindow(hSubMenuItems[i], SW_SHOW);
			}
		}
	}
	break;

	case Game::Background::CITY_MENU_CHARACTER:
	{
		Player& rPlayer = game.getPlayer();
		// TODO: Apply localization
		// Name
		buf = rPlayer.getName();
		SendMessage(hSubItems[CHARACTER_STAT_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Level
		rPlayer.gainExperience(1908);
		buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rPlayer.getLevel());
		SendMessage(hSubItems[CHARACTER_STAT_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Experience
		buf = l.getMessage(Localized::EXPERIENCE) + ": " + to_string(rPlayer.getExperience()) + " / " + to_string(rPlayer.calculateExperienceForOneLevel(rPlayer.getLevel() + 1));
		SendMessage(hSubItems[CHARACTER_STAT_EXPERIENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Age
		buf = l.getMessage(Localized::AGE) + ": " + to_string(rPlayer.getAge());
		SendMessage(hSubItems[CHARACTER_STAT_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Fame
		buf = l.getMessage(Localized::FAME) + ": " + to_string(rPlayer.getFame());
		SendMessage(hSubItems[CHARACTER_STAT_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Health
		buf = l.getMessage(Localized::HEALTH) + ":(" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
		SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Stats
		buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
		SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
		SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
		SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
		SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
		SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
		SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
		SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		// Right hand
		if (rPlayer.getRightHand())
		{
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_SHOW);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_SHOW);
			Weapon& rRightHand = *rPlayer.getRightHand();
			buf = l.getWeaponTypeName(rRightHand);
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rRightHand.getTotalDamage());
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rRightHand.getStrengthAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rRightHand.getDexterityAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::EMPTY_HAND).c_str());
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_STRENGTH_SCALE], SW_HIDE);
			ShowWindow(hSubItems[CHARACTER_STAT_RIGHT_HAND_DEXTERITY_SCALE], SW_HIDE);
		}

		// Left hand
		if (rPlayer.getLeftHand())
		{
			Weapon& rLeftHand = *rPlayer.getLeftHand();
			buf = l.getWeaponTypeName(rLeftHand);
			SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			if (rLeftHand.getWeaponType() != Weapon::WeaponType::SHIELD)
			{
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_SHOW);

				buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rLeftHand.getTotalDamage());
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rLeftHand.getStrengthAdditionPercentage()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rLeftHand.getDexterityAdditionPercentage()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_HIDE);
			}
			else
			{
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], SW_SHOW);

				buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rLeftHand.getShieldDefPercentAddition()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rLeftHand.getShieldProbAddition()) + "%";
				SendMessage(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_STAT_LEFT_HAND_DEXTERITY_SCALE], SW_HIDE);
			}
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::EMPTY_HAND).c_str());
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
			buf = l.getArmourTypeName(rArmour);
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rArmour.getTotalDefense());
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = l.getMessage(Localized::STRENGTH_SCALE) + ": " + to_string(rArmour.getStrengthAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_STRENGTH_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			buf = l.getMessage(Localized::DEXTERITY_SCALE) + ": " + to_string(rArmour.getDexterityAdditionPercentage()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEXTERITY_SCALE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			if (rPlayer.getArmour()->getArmourType() == Armour::ArmourType::LIGHT)
				buf = l.getMessage(Localized::EVASION_CHANCE) + ": " + to_string(rArmour.getEvasionProbAddition()) + "%";
			else
				buf = l.getMessage(Localized::STUN_RESISTANCE_CHANCE) + ": " + to_string(rArmour.getStunProbSubtraction()) + "%";
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_ABILITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		}
		else
		{
			SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::NO_ARMOUR).c_str());
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

		SendMessage(hSubItems[CHARACTER_BUT_RESET_CHANGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::RESET_CHANGES).c_str());

		SendMessage(hSubItems[CHARACTER_BUT_APPLY_CHANGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::APPLY_CHANGES).c_str());

		SendMessage(hSubItems[CHARACTER_BUT_BACK], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::BACK).c_str());

		// Buttons display
		// Increasing attributes
		if (pas.unnassignedAttributes > 0)
		{
			double average = pas.calculateAverage() + ATTRIBUTE_MAX_DIFFERENCE;
			// Strength
			if (pas.strength > average)
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
			// Constitution
			if (pas.constitution > average)
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
			// Dexterity
			if (pas.dexterity > average)
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
			// Intelligence
			if (pas.intelligence > average)
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
			// Wisdom
			if (pas.wisdom > average)
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
			// Charisma
			if (pas.charisma > average)
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
			else
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
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
	}
	break;
	}
	game.backgroundChangeCompleted();
}

void CityMenu::resizeMenu(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int sz, x, y, i;
	switch (game.getBackground())
	{
	case Game::Background::CITY_MENU:
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

	case Game::Background::CITY_MENU_ARENA:
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

	case Game::Background::CITY_MENU_ARENA_FIGHT:
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

		// Shield stats
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], x, y + 2 * (ITEM_HEIGHT + DISTANCE), 200, ITEM_HEIGHT, TRUE);

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

		// RightHand stats
		y = cy - (ITEM_HEIGHT + DISTANCE) * 3;
		x -= 120;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

		// LeftHand stats
		x += 40 + STAT_WIDTH;
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);

		// Shield stats
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);
		MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], x, y + 2 * (ITEM_HEIGHT + DISTANCE), 200, ITEM_HEIGHT, TRUE);

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

	case Game::Background::CITY_MENU_ARENA_BET:
	{
		// TODO
	}
	break;

	case Game::Background::CITY_MENU_QUEST:
	{
		// TODO
	}
	break;

	case Game::Background::CITY_MENU_MARKET:
	{
		// TODO
	}
	break;

	case Game::Background::CITY_MENU_CHARACTER:
	{
		const int BIG_STAT_WIDTH = 250, BIG_STAT_HEIGHT = 30,
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
			// Shield stats
			if (i == CHARACTER_STAT_LEFT_HAND_DAMAGE)
				MoveWindow(hSubItems[CHARACTER_STAT_SHIELD_DEFENSE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
			if (i == CHARACTER_STAT_LEFT_HAND_STRENGTH_SCALE)
				MoveWindow(hSubItems[CHARACTER_STAT_SHIELD_BLOCK_CHANCE], x, y, BIG_STAT_WIDTH, BIG_STAT_HEIGHT, TRUE);
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
	int i, j;
	static bool isExhausted = false;

	switch (m)
	{
	case WM_COMMAND:
		switch (game.getBackground())
		{
		default:case Game::Background::CITY_MENU:
		{
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
				hSubItems[ARENA_BUT_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::FIGHT).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BET] = CreateWindow("BUTTON", l.getMessage(Localized::BET).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_TRAIN] = CreateWindow("BUTTON", l.getMessage(Localized::TRAIN).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);
				hSubItems[ARENA_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(),
					WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
					0, 0, 0, 0, hWnd, 0, hInst, 0
				);

				game.setBackground(Game::Background::CITY_MENU_ARENA);

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
				pas.hp = rPlayer.getHP();
				pas.fullHP = rPlayer.getFullHP();
				pas.unnassignedAttributes = rPlayer.getUnnassignedAttributes();
				pas.strength = rPlayer.getStrength();
				pas.constitution = rPlayer.getConstitution();
				pas.dexterity = rPlayer.getDexterity();
				pas.intelligence = rPlayer.getIntelligence();
				pas.wisdom = rPlayer.getWisdom();
				pas.charisma = rPlayer.getCharisma();

				game.setBackground(Game::Background::CITY_MENU_CHARACTER);

				updateWindow(hWnd);
				game.updateBackground();
			}
			if ((HWND)lp == hItems[BUT_REST])
			{
				Player& rPlayer = game.getPlayer();
				if (rPlayer.getHP() < rPlayer.getFullHP())
				{
					int regen = BASIC_REGEN * rPlayer.getFullHP() / 100;

					// Heal player
					if (rPlayer.getHP() + regen >= rPlayer.getFullHP())
					{
						regen = rPlayer.getFullHP() - rPlayer.getHP();
						rPlayer.setHP(rPlayer.getFullHP());
						logStr += l.getMessage(Localized::REST_REGEN) + " " + to_string(regen) + " " + l.getMessage(Localized::HEALTH_POINTS_GENITIVE) + "\r\n"
							+ l.getMessage(Localized::REST_REGEN_FULL) + " (" + to_string(rPlayer.getFullHP()) + " " + l.getMessage(Localized::HP) + ")\r\n\r\n";
					}
					else
					{
						rPlayer.setHP(rPlayer.getHP() + regen);
						logStr += l.getMessage(Localized::REST_REGEN) + " " + to_string(regen) + " " + l.getMessage(Localized::HEALTH_POINTS_GENITIVE) + " (" + to_string(rPlayer.getHP()) + l.getMessage(Localized::HP) + ")\r\n\r\n";
					}

					// Heal opponents across all arenas
					for (i = Cities::ROME; i < Cities::CITIES_NUMBER; i++)
					{
						// Get opponents
						vector<unique_ptr<NPC>>& opponents = game.getWorldMap().getCity(i).getArena().getGladiators();

						// Check every opponent
						for (j = 0; j < opponents.size(); j++)
						{
							NPC& opponent = *opponents[j];
							int hp = opponent.getHP();
							int fullHP = opponent.getFullHP();
							if (hp < fullHP)
							{
								if (hp + BASIC_REGEN < fullHP)
									opponent.setHP(hp + BASIC_REGEN);
								else
									opponent.setHP(fullHP);
							}
						}
					}
					isExhausted = false;
				}
				else
					logStr += l.getMessage(Localized::REST_FULL) + "\r\n\r\n";
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
				game.setBackground(Game::Background::WORLD_MAP);
				updateWindow(hWnd);
				break;
			}
			if ((HWND)lp == hItems[BUT_MENU] || LOWORD(wp) == IDCANCEL)
			{
				game.getMenuManager().setMenu(new GameMenu(hWnd));
				game.setBackground(Game::Background::GAME_MENU);
				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::CITY_MENU_ARENA:
		{
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
				{
					for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_PLAYER_FAME; i++)
						hSubMenuItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

					Player& rPlayer = game.getPlayer();

					if (rPlayer.getLeftHand())
					{
						if (rPlayer.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
						{
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], SW_SHOW);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], SW_HIDE);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], SW_HIDE);
						}
						else
						{
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], SW_HIDE);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], SW_SHOW);
							ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], SW_SHOW);
						}
					}

					// Name
					buf = rPlayer.getName();
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Level
					buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rPlayer.getLevel());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Right hand
					if (rPlayer.getRightHand())
					{
						buf = l.getWeaponTypeName(*rPlayer.getRightHand());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					// Left hand
					if (rPlayer.getLeftHand())
					{
						buf = l.getWeaponTypeName(*rPlayer.getLeftHand());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						if (rPlayer.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
						{
							buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}
						else
						{
							buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rPlayer.getLeftHand()->getShieldDefPercentAddition());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
							buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rPlayer.getLeftHand()->getShieldProbAddition());
							SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
						}
					}
					else
					{
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::EMPTY_HAND).c_str());

						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_LEFT_HAND_DAMAGE], SW_HIDE);
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_DEFENSE], SW_HIDE);
						ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_SHIELD_BLOCK_CHANCE], SW_HIDE);
					}

					// Health
					buf = l.getMessage(Localized::HEALTH) + ": " + to_string(rPlayer.getHP());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					// Armour
					if (rPlayer.getArmour())
					{
						buf = l.getArmourTypeName(*rPlayer.getArmour());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

						buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
						SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
					}

					// Stats
					buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(rPlayer.getStrength());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(rPlayer.getConstitution());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(rPlayer.getDexterity());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(rPlayer.getIntelligence());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::WISDOM) + ": " + to_string(rPlayer.getWisdom());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(rPlayer.getCharisma());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::AGE) + ": " + to_string(rPlayer.getAge());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

					buf = l.getMessage(Localized::FAME) + ": " + to_string(rPlayer.getFame());
					SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_PLAYER_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}

				// Outputing opponents
				Arena& rArena = game.getWorldMap().getCurrentCity().getArena();
				for (i = ARENA_FIGHT_BUT_OPPONENT1; i <= ARENA_FIGHT_BUT_OPPONENT15; i++)
				{
					const NPC& rOpponent = *rArena.getGladiator(i);
					buf = l.getNPCName(rOpponent) + " (" + l.getMessage(Localized::LVL) + ": " + to_string(rOpponent.getLevel()) + ")";
					hSubMenuItems[i] = CreateWindow("BUTTON",
						buf.c_str(),
						WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTORADIOBUTTON | BS_OWNERDRAW | BS_LEFTTEXT,
						0, 0, 0, 0, hWnd, 0, hInst, 0);
				}

				hSubMenuItems[ARENA_FIGHT_BUT_BACK] = CreateWindow("BUTTON", l.getMessage(Localized::BACK).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubMenuItems[ARENA_FIGHT_BUT_FIGHT] = CreateWindow("BUTTON", l.getMessage(Localized::FIGHT).c_str(), WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_FAME; i++)
					ShowWindow(hSubMenuItems[i], SW_HIDE);

				game.setBackground(Game::Background::CITY_MENU_ARENA_FIGHT);

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
					logStr += l.getMessage(Localized::TRAIN_GAIN) + " " + to_string(experience) + " " + l.getMessage(Localized::EXPERIENCE_GENITIVE) + "\r\n\r\n";
					if (level < rPlayer.getLevel())
						logStr += l.getMessage(Localized::LEVELED_UP) + " " +
						to_string(rPlayer.getLevel()) +
						" (" + to_string(rPlayer.getUnnassignedAttributes()) + " " + l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ")\r\n\r\n";

					isExhausted = true;
				}
				else
					logStr += l.getMessage(Localized::TRAIN_NEED_REST) + "\r\n\r\n";
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

				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::CITY_MENU_ARENA_FIGHT:
		{
			// Selecting opponent
			{
				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT1])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT1);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT2])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT2);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT3])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT3);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT4])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT4);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT5])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT5);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT6])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT6);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT7])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT7);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT8])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT8);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT9])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT9);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT10])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT10);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT11])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT11);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT12])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT12);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT13])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT13);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT14])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT14);

				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT15])
					outputOpponent(hWnd, ARENA_FIGHT_BUT_OPPONENT15);
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
					game.setBackground(Game::Background::FIGHTING_ARENA);
					updateWindow(hWnd);

					// Fight
					City& currentCity = game.getWorldMap().getCurrentCity();
					Arena& currentArena = currentCity.getArena();

					FightStatus fightStatus = game.getFighting().fight(
						hWnd,
						game.getPlayer(),
						currentArena.getGladiator(selectedOpponent)
					);

					// Fight result
					switch (fightStatus)
					{
					// In case of opponent's death he's replaced on arena with new gladiator
					case FightStatus::OPPONENT_LOST: currentArena.changeGladiator(selectedOpponent, currentCity.getLevel());
						break;
					case FightStatus::OPPONNENT_SURRENDERED:
						break;
					case FightStatus::PLAYER_SURRENDERED:
						break;
					case FightStatus::PLAYER_LOST:
						break;
					}

					// Return to city
					game.setBackground(Game::Background::CITY_MENU);
					selectedOpponent = -1;
					break;
				}
				else if (selectedOpponent == -1)
					MessageBox(hWnd, l.getMessage(Localized::HAVE_TO_CHOOSE_OPPONENT).c_str(), l.getMessage(Localized::OPPONENT_NOT_SELECTED).c_str(), MB_OK | MB_ICONINFORMATION);
				else
					MessageBox(hWnd, l.getMessage(Localized::FIGHT_NEED_REST).c_str(), l.getMessage(Localized::FIGHT_INJURED).c_str(), MB_OK | MB_ICONINFORMATION);
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

				game.setBackground(Game::Background::CITY_MENU_ARENA);
				selectedOpponent = -1;

				updateWindow(hWnd);
			}
		}
		break;

		case ArenaItem::ARENA_BUT_BET:
		{
			// TODO
		}
		break;

		case Game::Background::CITY_MENU_CHARACTER:
		{
			Player& rPlayer = game.getPlayer();
			int i;
			double average = ATTRIBUTE_MAX_DIFFERENCE;

			// Plus buttons
			// Strength
			if ((HWND)lp == hSubItems[CHARACTER_BUT_STRENGTH_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.strength++;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + " : (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}
			// Constitution
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.constitution++;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}
			// Dexterity
			if ((HWND)lp == hSubItems[CHARACTER_BUT_DEXTERITY_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.dexterity++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}
			// Intelligence
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.intelligence++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}
			// Wisdom
			if ((HWND)lp == hSubItems[CHARACTER_BUT_WISDOM_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.wisdom++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}
			// Charisma
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CHARISMA_PLUS])
			{
				pas.unnassignedAttributes--;
				pas.charisma++;
				average += pas.calculateAverage();

				// Buttons appearance
				ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_SHOW);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_SHOW);
				if (pas.unnassignedAttributes == 0)
				{
					for (i = CHARACTER_BUT_STRENGTH_PLUS; i <= CHARACTER_BUT_CHARISMA_PLUS; i++)
						ShowWindow(hSubItems[i], SW_HIDE);
				}
				else
				{
					if (pas.strength <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					if (pas.constitution <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					if (pas.dexterity <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					if (pas.intelligence <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					if (pas.wisdom <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					if (pas.charisma <= average)
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
					else
						ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
				}
				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}

			// Minus buttons
			// Strength
			if ((HWND)lp == hSubItems[CHARACTER_BUT_STRENGTH_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.strength--;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.strength <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
				if (pas.strength == rPlayer.getStrength())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

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
					game.updateBackground();
				}

				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Constitution
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.constitution--;
				pas.updateMaxHP();
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.constitution <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
				if (pas.constitution == rPlayer.getConstitution())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

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
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Dexterity
			if ((HWND)lp == hSubItems[CHARACTER_BUT_DEXTERITY_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.dexterity--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.dexterity <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
				if (pas.dexterity == rPlayer.getDexterity())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

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
					game.updateBackground();
				}

				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Intelligence
			if ((HWND)lp == hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.intelligence--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.intelligence <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
				if (pas.intelligence == rPlayer.getIntelligence())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

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
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Wisdom
			if ((HWND)lp == hSubItems[CHARACTER_BUT_WISDOM_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.wisdom--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.wisdom <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
				if (pas.wisdom == rPlayer.getWisdom())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.charisma > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);
					game.updateBackground();
				}

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
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}
			// Charisma
			if ((HWND)lp == hSubItems[CHARACTER_BUT_CHARISMA_MINUS])
			{
				pas.unnassignedAttributes++;
				pas.charisma--;
				average += pas.calculateAverage();

				// Buttons appearance
				if (pas.charisma <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
				if (pas.charisma == rPlayer.getCharisma())
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_MINUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.strength > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.constitution > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.dexterity > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.intelligence > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
					game.updateBackground();
				}
				if (pas.wisdom > average)
				{
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
					game.updateBackground();
				}

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
					game.updateBackground();
				}

				// Text
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			}

			// Reset changes
			if ((HWND)lp == hSubItems[CHARACTER_BUT_RESET_CHANGES])
			{
				// Update stats
				pas.unnassignedAttributes = rPlayer.getUnnassignedAttributes();
				pas.strength = rPlayer.getStrength();
				pas.constitution = rPlayer.getConstitution();
				pas.dexterity = rPlayer.getDexterity();
				pas.intelligence = rPlayer.getIntelligence();
				pas.wisdom = rPlayer.getWisdom();
				pas.charisma = rPlayer.getCharisma();
				average += pas.calculateAverage();

				// Button appearance
				if (pas.strength <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_STRENGTH_PLUS], SW_HIDE);
				if (pas.constitution <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_CONSTITUTION_PLUS], SW_HIDE);
				if (pas.dexterity <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_DEXTERITY_PLUS], SW_HIDE);
				if (pas.intelligence <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_INTELLIGENCE_PLUS], SW_HIDE);
				if (pas.wisdom <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_WISDOM_PLUS], SW_HIDE);
				if (pas.charisma <= average)
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_SHOW);
				else
					ShowWindow(hSubItems[CHARACTER_BUT_CHARISMA_PLUS], SW_HIDE);

				for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);

				// Text
				// Update weapon damage
				if (rPlayer.getRightHand())
				{
					rPlayer.getRightHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getRightHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				if (rPlayer.getLeftHand())
				{
					rPlayer.getLeftHand()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rPlayer.getLeftHand()->getTotalDamage());
					SendMessage(hSubItems[CHARACTER_STAT_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update armour defense
				if (rPlayer.getArmour())
				{
					rPlayer.getArmour()->update(pas.strength, pas.dexterity);
					buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rPlayer.getArmour()->getTotalDefense());
					SendMessage(hSubItems[CHARACTER_STAT_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
				}
				// Update stats
				buf = l.getMessage(Localized::UNNASSIGNED_ATTRIBUTES) + ": " + to_string(pas.unnassignedAttributes);
				SendMessage(hSubItems[CHARACTER_STAT_UNNASSIGNED_ATTRIBUTES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(pas.strength);
				SendMessage(hSubItems[CHARACTER_STAT_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(pas.constitution);
				SendMessage(hSubItems[CHARACTER_STAT_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(pas.dexterity);
				SendMessage(hSubItems[CHARACTER_STAT_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(pas.intelligence);
				SendMessage(hSubItems[CHARACTER_STAT_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::WISDOM) + ": " + to_string(pas.wisdom);
				SendMessage(hSubItems[CHARACTER_STAT_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(pas.charisma);
				SendMessage(hSubItems[CHARACTER_STAT_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				buf = l.getMessage(Localized::HEALTH) + ": (" + to_string(pas.hp) + " / " + to_string(pas.fullHP) + " )";
				SendMessage(hSubItems[CHARACTER_STAT_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

				game.updateBackground();
			}

			// Apply changes
			if ((HWND)lp == hSubItems[CHARACTER_BUT_APPLY_CHANGES])
			{
				// Update player stats
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
				for (i = CHARACTER_BUT_STRENGTH_MINUS; i <= CHARACTER_BUT_CHARISMA_MINUS; i++)
					ShowWindow(hSubItems[i], SW_HIDE);

				ShowWindow(hSubItems[CHARACTER_BUT_RESET_CHANGES], SW_HIDE);
				ShowWindow(hSubItems[CHARACTER_BUT_APPLY_CHANGES], SW_HIDE);

				game.updateBackground();
			}

			if ((HWND)lp == hSubItems[CHARACTER_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Changing back weapon damage and armour defense based on stats in case player left without applying changes
				if (rPlayer.getRightHand())
					rPlayer.getRightHand()->update(rPlayer.getStrength(), rPlayer.getDexterity());
				if (rPlayer.getLeftHand())
					rPlayer.getLeftHand()->update(rPlayer.getStrength(), rPlayer.getDexterity());
				if (rPlayer.getArmour())
					rPlayer.getArmour()->update(rPlayer.getStrength(), rPlayer.getDexterity());

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;
		}
	break;
	}	
}

void CityMenu::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
		case WM_DRAWITEM:
		{
			LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lp;
			HDC hdc = item->hDC;

			GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

			// Set text font and background
			SelectObject(hdc, game.getFont(Game::FontSize::SMALL));
			SetBkMode(hdc, TRANSPARENT);

			// Assing background and text color
			SetTextColor(hdc, COLOR_WHITE);
			FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED));

			// Draw text
			int len = GetWindowTextLength(item->hwndItem);
			buf.resize(len + 1); // Resize buffer to contain button text
			GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer
			DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text on button

			// Checking window type to draw it using correct styles

			if (game.getBackground() == Game::Background::CITY_MENU_ARENA_FIGHT &&
				selectedOpponent != -1 &&
				item->hwndItem == hSubMenuItems[selectedOpponent])
			{
				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
				DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
			}
			else if (item->CtlType == ODT_STATIC) // Static windows
			{
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT);
			}
			else if (strcmp(str, ("Edit")) == 0) // Edit windows
			{
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT);
			}
			else if (item->CtlType == ODT_BUTTON) // Button windows
			{
				if (item->itemState & ODS_SELECTED)
				{
					FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_ROMAN_RED_PUSHED));
					DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER);
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
				}
				else
					DrawEdge(hdc, &item->rcItem, EDGE_RAISED, BF_RECT);
			}
		}
		break;
	}
}

void CityMenu::outputOpponent(HWND hWnd, int n)
{
	RECT rect;
	int i;
	// Update button selection
	// If previously button was selected it must be updated
	if (selectedOpponent != -1)
	{
		GetWindowRect(hSubMenuItems[selectedOpponent], &rect);
		MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
		InvalidateRect(hWnd, &rect, 1);
	}

	selectedOpponent = n;

	GetWindowRect(hSubMenuItems[selectedOpponent], &rect);
	MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
	InvalidateRect(hWnd, &rect, 1);

	// Outputing opponent's information
	NPC& rOpponent = *game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent);

	for (i = ARENA_FIGHT_STATIC_NAME; i <= ARENA_FIGHT_STATIC_LEFT_HAND_TYPE; i++)
		ShowWindow(hSubMenuItems[i], SW_SHOW);
	if (rOpponent.getLeftHand())
	{
		if (rOpponent.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], SW_SHOW);
		else
		{
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_SHOW);
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_SHOW);
		}
	}
	for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
		ShowWindow(hSubMenuItems[i], SW_SHOW);

	// Name
	buf = l.getNPCName(rOpponent);
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	// Level
	buf = l.getMessage(Localized::LEVEL) + ": " + to_string(rOpponent.getLevel());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	// Right hand
	if (rOpponent.getRightHand())
	{
		buf = l.getWeaponTypeName(*rOpponent.getRightHand());
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rOpponent.getRightHand()->getTotalDamage());
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
	}

	// Left hand
	if (rOpponent.getLeftHand())
	{
		buf = l.getWeaponTypeName(*rOpponent.getLeftHand());
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		if (rOpponent.getLeftHand()->getWeaponType() != Weapon::WeaponType::SHIELD)
		{
			buf = l.getMessage(Localized::DAMAGE) + ": " + to_string(rOpponent.getLeftHand()->getTotalDamage());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_HIDE);
			ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_HIDE);
			game.updateBackground();
		}
		else
		{
			buf = l.getMessage(Localized::BLOCK_DEFENSE) + ": " + to_string(rOpponent.getLeftHand()->getShieldDefPercentAddition());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
			buf = l.getMessage(Localized::BLOCK_CHANCE) + ": " + to_string(rOpponent.getLeftHand()->getShieldProbAddition());
			SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		}
	}
	else
	{
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)l.getMessage(Localized::EMPTY_HAND).c_str());

		ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], SW_HIDE);
		ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_DEFENSE], SW_HIDE);
		ShowWindow(hSubMenuItems[ARENA_FIGHT_STATIC_SHIELD_BLOCK_CHANCE], SW_HIDE);
		game.updateBackground();
	}

	// Health
	buf = l.getMessage(Localized::HEALTH) + ": " + to_string(rOpponent.getHP());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	// Armour
	if (rOpponent.getArmour())
	{
		buf = l.getArmourTypeName(*rOpponent.getArmour());
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

		buf = l.getMessage(Localized::ARMOUR_DEFENSE) + ": " + to_string(rOpponent.getArmour()->getTotalDefense());
		SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
	}

	// Stats
	buf = l.getMessage(Localized::STRENGTH) + ": " + to_string(rOpponent.getStrength());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::CONSTITUTION) + ": " + to_string(rOpponent.getConstitution());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::DEXTERITY) + ": " + to_string(rOpponent.getDexterity());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::INTELLIGENCE) + ": " + to_string(rOpponent.getIntelligence());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::WISDOM) + ": " + to_string(rOpponent.getWisdom());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::CHARISMA) + ": " + to_string(rOpponent.getCharisma());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::AGE) + ": " + to_string(rOpponent.getAge());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	buf = l.getMessage(Localized::FAME) + ": " + to_string(rOpponent.getFame());
	SendMessage(hSubMenuItems[ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
}
