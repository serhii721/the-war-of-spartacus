#include "stdafx.h"
#include "CityMenu.h"

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

CityMenu::CityMenu() : hItems(), hSubItems(), currentSubMenu(Item::ITEM_NUMBER) { }

CityMenu::CityMenu(HWND hWnd) : hItems(Item::ITEM_NUMBER), currentSubMenu(Item::ITEM_NUMBER), hSubItems()
{
	char className[256] = "BUTTON";
	hItems[BUT_ARENA] = CreateWindow(className, "Arena", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_QUEST] = CreateWindow(className, "Quest", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_TRADER] = CreateWindow(className, "Trader", // TODO: Apply localization
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

CityMenu::CityMenu(const CityMenu& CM) : currentSubMenu(Item::ITEM_NUMBER), hSubItems()
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

	hSubItems = vector<HWND>();
	currentSubMenu = CM.currentSubMenu;

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
}

void CityMenu::drawMenu(HDC, int, int) { }

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
		const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 300, ITEM_HEIGHT = 60, DISTANCE = 15;
		cy -= 3 * (ITEM_HEIGHT + DISTANCE);
		y = cy;
		sz = hItems.size();
		x = cx - BUT_WIDTH * 2;
		for (i = ARENA_BUT_FIGHT; i < 4; i++)
		{
			y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hSubItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			UpdateWindow(hSubItems[i]);
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
	case Item::BUT_TRADER:
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

				// Updating window to show new buttons
				SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top));
			}
			if ((HWND)lp == hItems[BUT_QUEST])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_TRADER])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_REST])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_MAP])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_MENU] || LOWORD(wp) == IDCANCEL)
			{
				game.getMenuManager().setMenu(new GameMenu(hWnd));
				// Updating window to show new buttons
				SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top));
			}
			break;

		case BUT_ARENA:
			if ((HWND)lp == hSubItems[ARENA_BUT_FIGHT])
			{
				// TODO
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

				// Updating window to show new buttons
				SendMessage(hWnd, WM_SIZE, SIZE_RESTORED, MAKELPARAM(windowRect.right - windowRect.left, windowRect.bottom - windowRect.top));
				break;
			}
			break;
		}
		break;
	}	
}