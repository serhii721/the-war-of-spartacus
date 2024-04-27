#include "stdafx.h"
#include "CityMenu.h"

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

CityMenu::CityMenu() :
	hItems(Item::ITEM_NUMBER),
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	currentSubMenuItem(ArenaItem::ARENA_ITEM_NUMBER),
	hSubMenuItems(),
	selectedOpponent(-1)
{ }

CityMenu::CityMenu(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	currentSubMenuItem(ArenaItem::ARENA_ITEM_NUMBER),
	selectedOpponent(-1)
{
	string buffer = "City of " + game.getWorldMap().getCurrentCity().getNameString();
	hItems[STAT_CITY_NAME] = CreateWindow("STATIC", buffer.c_str(), // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	char className[256] = "BUTTON";
	hItems[BUT_ARENA] = CreateWindow(className, "Arena", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_QUEST] = CreateWindow(className, "Quest", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MARKET] = CreateWindow(className, "Trader", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_REST] = CreateWindow(className, "Rest", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MAP] = CreateWindow(className, "Map", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_MENU] = CreateWindow(className, "Menu", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

CityMenu::CityMenu(const CityMenu& CM) :
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	currentSubMenuItem(ArenaItem::ARENA_ITEM_NUMBER),
	hSubMenuItems(),
	selectedOpponent(-1)
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
}

void CityMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/Background/";
	const string FORMAT = ".bmp";
	string path(""), buffer("");

	RECT rect;
	GetClientRect(hWnd, &rect);

	// 1. Background
	// Composing path based on current menu
	switch (currentSubMenu)
	{
	default:case ITEM_NUMBER:
		path = DIRECTORY + "cityBackground768" + FORMAT;
		buffer = "City of " + game.getWorldMap().getCurrentCity().getNameString();
		SendMessage(hItems[STAT_CITY_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());
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
	}

	// Loading image
	hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Filling background with selected image
	hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
	FillRect(hdc, &rect, hBackgroundBrush);

	// 2. Text
	switch (currentSubMenu)
	{
	default:case ITEM_NUMBER:
		for (HWND hItem : hItems)
			ShowWindow(hItem, SW_SHOW);
		break;
	case BUT_ARENA:
		switch (currentSubMenuItem)
		{
		default:case ARENA_ITEM_NUMBER: break;
		case ARENA_BUT_FIGHT:
			if (selectedOpponent != -1)
			{
				string buffer;
				NPC tmp(*game.getWorldMap().getCurrentCity().getArena().getGladiator(selectedOpponent));

				// TODO: Localization

				// Name
				buffer = tmp.getName();
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_NAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				// Level
				buffer = "Level: " + to_string(tmp.getLevel());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_LEVEL], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				// Right hand
				if (tmp.getRightHand())
				{
					buffer = tmp.getRightHand()->getTypeString();
					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

					buffer = "Damage: " + to_string(tmp.getRightHand()->getTotalDamage());
					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());
				}

				// Left hand
				if (tmp.getLeftHand())
				{
					buffer = tmp.getLeftHand()->getTypeString();
					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

					if (tmp.getLeftHand()->getType() != Weapon::Type::SHIELD)
					{
						buffer = "Damage: " + to_string(tmp.getLeftHand()->getTotalDamage());
						SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());
					}
					else
					{
						buffer = "Block defense: " + to_string(tmp.getLeftHand()->getShieldDefPercentAddition());
						SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());
					}
				}
				else
				{
					buffer = "Empty hand";
					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");
				}

				// Health
				buffer = "Health: " + to_string(tmp.getHP());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_HEALTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				// Armour
				if (tmp.getArmour())
				{
					buffer = tmp.getArmour()->getTypeString();
					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_ARMOUR_TYPE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

					buffer = "Armour defense: " + to_string(tmp.getArmour()->getTotalDefense());
					SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_ARMOUR_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());
				}

				// Stats
				buffer = "Strength: " + to_string(tmp.getStrength());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_STRENGTH], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Constitution: " + to_string(tmp.getConstitution());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_CONSTITUTION], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Dexterity: " + to_string(tmp.getDexterity());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_DEXTERITY], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Inteliigence: " + to_string(tmp.getIntelligence());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_INTELLIGENCE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Wisdom: " + to_string(tmp.getWisdom());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_WISDOM], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Charisma: " + to_string(tmp.getCharisma());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_CHARISMA], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Age: " + to_string(tmp.getAge());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_AGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());

				buffer = "Fame: " + to_string(tmp.getFame());
				SendMessage(hSubMenuItems[ArenaFightItem::ARENA_FIGHT_STATIC_FAME], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buffer.c_str());
			}
			break;
		}
		break;
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
		y = cy - sz / 1.5 * (ITEM_HEIGHT + DISTANCE);
		for (HWND hItem : hItems)
		{
			y += ITEM_HEIGHT + DISTANCE;
			if (hItem == hItems[BUT_MENU])
				y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hItem, x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
			UpdateWindow(hItem);
		}
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
				UpdateWindow(hSubItems[i]);
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
				UpdateWindow(hSubMenuItems[i]);
				y += ITEM_HEIGHT + DISTANCE;
			}

			y = DISTANCE * 2 + ITEM_HEIGHT * 2;
			x = cx + BUT_WIDTH * 0.5 + DISTANCE;

			// Opponent's stats
			if (selectedOpponent != -1)
			{
				MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_NAME], x, y - ITEM_HEIGHT - DISTANCE, STAT_WIDTH, ITEM_HEIGHT, TRUE);
				UpdateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_NAME]);

				MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL], x, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);
				UpdateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEVEL]);

				// LeftHand stats
				y = cy - (ITEM_HEIGHT + DISTANCE) * 3;
				x -= 200;
				MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
				UpdateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE]);

				MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);
				UpdateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_RIGHT_HAND_TYPE]);

				// RightHand stats
				x += 200 + STAT_WIDTH;
				MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE], x, y, 200, ITEM_HEIGHT, TRUE);
				UpdateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_TYPE]);

				MoveWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE], x, y + ITEM_HEIGHT + DISTANCE, 200, ITEM_HEIGHT, TRUE);
				UpdateWindow(hSubMenuItems[ARENA_FIGHT_STATIC_LEFT_HAND_DAMAGE]);

				// Other stats
				x -= STAT_WIDTH;
				y = cy;
				for (i = ARENA_FIGHT_STATIC_HEALTH; i <= ARENA_FIGHT_STATIC_FAME; i++)
				{
					MoveWindow(hSubMenuItems[i], x, y, STAT_WIDTH, ITEM_HEIGHT - 10, TRUE);
					UpdateWindow(hSubMenuItems[i]);
					y += ITEM_HEIGHT - 10 + DISTANCE / 2;
					if (i == ARENA_FIGHT_STATIC_ARMOUR_DEFENSE || i == ARENA_FIGHT_STATIC_CHARISMA)
						y += DISTANCE;
				}
			}
			x = 374;
			y = 672;

			MoveWindow(hSubMenuItems[ARENA_FIGHT_BUT_BACK], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			UpdateWindow(hSubMenuItems[ARENA_FIGHT_BUT_BACK]);

			MoveWindow(hSubMenuItems[ARENA_FIGHT_BUT_FIGHT], x + BUT_WIDTH + DISTANCE, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			UpdateWindow(hSubMenuItems[ARENA_FIGHT_BUT_FIGHT]);
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

	switch (m)
	{
	case WM_COMMAND:
		switch (currentSubMenu)
		{
		default:case ITEM_NUMBER:
			if ((HWND)lp == hItems[BUT_ARENA])
			{
				// Hiding all buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(ARENA_ITEM_NUMBER);

				// TODO: Apply localization
				hSubItems[ARENA_BUT_FIGHT] = (CreateWindow("BUTTON", "Fight", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0));
				hSubItems[ARENA_BUT_BET] = (CreateWindow("BUTTON", "Bet", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0));
				hSubItems[ARENA_BUT_TRAIN] = (CreateWindow("BUTTON", "Train", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0));
				hSubItems[ARENA_BUT_BACK] = (CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0));

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
			if ((HWND)lp == hItems[BUT_REST])
			{
				// TODO
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

				currentSubMenuItem = ArenaItem::ARENA_ITEM_NUMBER;
				currentSubMenu = Item::ITEM_NUMBER;
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
			default:case ArenaItem::ARENA_ITEM_NUMBER:
				if ((HWND)lp == hSubItems[ARENA_BUT_FIGHT])
				{
					// Hiding all buttons
					for (HWND hItem : hSubItems)
						ShowWindow(hItem, SW_HIDE);

					// Erasing previous sub menu items
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();

					// Creating new sub menu items
					hSubMenuItems.resize(ARENA_FIGHT_ITEM_NUMBER);

					for (int i = ArenaFightItem::ARENA_FIGHT_STATIC_NAME; i <= ArenaFightItem::ARENA_FIGHT_STATIC_FAME; i++)
						hSubMenuItems[i] = CreateWindow("STATIC", "", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, 0, hInst, 0);

					for (int i = ArenaFightItem::ARENA_FIGHT_BUT_OPPONENT1; i <= ArenaFightItem::ARENA_FIGHT_BUT_OPPONENT15; i++)
						hSubMenuItems[i] = CreateWindow("BUTTON",
							game.getWorldMap().getCurrentCity().getArena().getGladiator(i)->getName().c_str(),
							WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTOCHECKBOX | BS_LEFTTEXT,
							0, 0, 0, 0, hWnd, 0, hInst, 0);

					hSubMenuItems[ARENA_FIGHT_BUT_BACK] = CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
					hSubMenuItems[ARENA_FIGHT_BUT_FIGHT] = CreateWindow("BUTTON", "Fight", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

					currentSubMenuItem = ArenaItem::ARENA_BUT_FIGHT;

					updateWindow(hWnd);
				}
				if ((HWND)lp == hSubItems[ARENA_BUT_BET])
				{
					// TODO
				}
				if ((HWND)lp == hSubItems[ARENA_BUT_TRAIN])
				{
					// TODO
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
					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT1])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT1], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT1;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT2])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT2], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT2;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT3])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT3], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT3;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT4])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT4], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT4;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT5])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT5], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT5;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT6])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT6], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT6;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT7])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT7], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT7;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT8])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT8], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT8;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT9])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT9], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT9;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT10])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT10], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT10;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT11])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT11], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT11;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT12])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT12], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT12;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT13])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT13], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT13;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT14])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT14], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT14;
						updateWindow(hWnd);
					}

					if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT15])
					{
						for (HWND hSubMenuItem : hSubMenuItems)
							SendMessage(hSubMenuItem, BM_SETCHECK, 0, 0);
						SendMessage(hSubMenuItems[ARENA_FIGHT_BUT_OPPONENT15], BM_SETCHECK, 1, 0);
						selectedOpponent = ARENA_FIGHT_BUT_OPPONENT15;
						updateWindow(hWnd);
					}
				}

				// Starting fight
				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_FIGHT] &&
					selectedOpponent != -1 &&
					game.getPlayer().getHP() > 30)
				{
					// Destroying all buttons
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();

					game.getFighting().setScreen(Fighting::Screen::FIGHT_ARENA);
					game.setDisplayState(DisplayState::FIGHTING);
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

				// Return
				if ((HWND)lp == hSubMenuItems[ARENA_FIGHT_BUT_BACK] || LOWORD(wp) == IDCANCEL)
				{
					// Destroying all buttons
					for (HWND hItem : hSubMenuItems)
						if (hItem != NULL)
							DestroyWindow(hItem);
					hSubMenuItems.clear();

					// Showing main menu buttons
					for (HWND hItem : hSubItems)
						ShowWindow(hItem, SW_SHOW);

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
		}
		break;
	}	
}
