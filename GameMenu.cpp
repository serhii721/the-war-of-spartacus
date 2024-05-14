#include "stdafx.h"
#include "GameMenu.h"

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

GameMenu::GameMenu() :
	hItems(),
	hSubItems(),
	currentSubMenu(Item::ITEM_NUMBER),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{ }

GameMenu::GameMenu(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	currentSubMenu(Item::ITEM_NUMBER),
	hSubItems(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{
	char className[256] = "BUTTON";
	hItems[BUT_RESUME] = CreateWindow(className, "Resume", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SAVE] = CreateWindow(className, "Save game", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_LOAD] = CreateWindow(className, "Load game", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SETTINGS] = CreateWindow(className, "Settings", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_EXIT_MENU] = CreateWindow(className, "Exit to menu", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

GameMenu::GameMenu(const GameMenu& GM) : currentSubMenu(Item::ITEM_NUMBER), hSubItems()
{
	// Resizing items' vector
	int sz = GM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = GM.hItems[i];
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

	if (GM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(GM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (GM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(GM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

GameMenu& GameMenu::operator=(const GameMenu& GM)
{
	if (&GM == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = GM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = GM.hItems[i];
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
	currentSubMenu = GM.currentSubMenu;

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (GM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(GM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (GM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(GM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	return *this;
}

GameMenu::~GameMenu()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	for (HWND hItem : hSubItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

void GameMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	// 1. Background
	if (game.isBackgroundChanged())
	{
		const string DIRECTORY = "Data/Image/Background/";
		const string FORMAT = ".bmp";
		string path("");
		RECT rect;

		GetClientRect(hWnd, &rect);

		// Composing path based on current menu
		switch (game.getBackground())
		{
		default:case Game::Background::GAME_MENU: path = DIRECTORY + "menuBackground768" + FORMAT; break;
		case Game::Background::GAME_MENU_SAVE: // TODO
			break;
		case Game::Background::GAME_MENU_LOAD: // TODO
			break;
		case Game::Background::GAME_MENU_SETTINGS: // TODO
			break;
		}
		// Loading image
		if (hBackgroundImage != NULL)
			DeleteObject(hBackgroundImage);
		hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
		// Filling background with selected image
		if (hBackgroundBrush != NULL)
			DeleteObject(hBackgroundBrush);
		hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
		FillRect(hdc, &rect, hBackgroundBrush);
		game.backgroundChangeCompleted();
	}
}

void GameMenu::resizeMenu(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int sz, x, y;

	switch (currentSubMenu)
	{
	// Game menu
	case Item::ITEM_NUMBER:
	{
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300;
		sz = hItems.size();
		x = cx - ITEM_WIDTH / 2;
		y = cy - sz / 2 * (ITEM_HEIGHT + DISTANCE);
		for (HWND hItem : hItems)
		{
			y += ITEM_HEIGHT + DISTANCE;
			MoveWindow(hItem, x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		}
	}
	break;

	// Save
	case Item::BUT_SAVE:
	{
		// TODO
	}
	break;

	// Load
	case Item::BUT_LOAD:
	{
		// TODO
	}
	break;

	// Settings
	case Item::BUT_SETTINGS:
	{
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300, BUT_SIZE = 45;
		sz = hItems.size();
		x = cx - ITEM_WIDTH / 2;
		y = cy - sz / 2 * (ITEM_HEIGHT + DISTANCE);

		// Video settings (in progress)
		MoveWindow(hSubItems[SETTINGS_STAT_VIDEO], x, y, ITEM_WIDTH / 3, ITEM_HEIGHT, TRUE);
		y += ITEM_HEIGHT + DISTANCE;

		// Sound settings
		MoveWindow(hSubItems[SETTINGS_STAT_SOUND], x, y, ITEM_WIDTH - BUT_SIZE - DISTANCE, ITEM_HEIGHT, TRUE);

		// Sound checkbox
		MoveWindow(hSubItems[SETTINGS_BUT_SOUND], x + ITEM_WIDTH - BUT_SIZE, y, BUT_SIZE, BUT_SIZE, TRUE);
		y += ITEM_HEIGHT + DISTANCE;

		// Back
		MoveWindow(hSubItems[SETTINGS_BUT_BACK], x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;
	}
}

void GameMenu::updateLanguage()
{
	// TODO: update localization
}

void GameMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);

	switch (m)
	{
	case WM_COMMAND:
		switch (currentSubMenu)
		{
		default:case ITEM_NUMBER:
			if ((HWND)lp == hItems[BUT_RESUME] || LOWORD(wp) == IDCANCEL)
			{
				game.getMenuManager().setMenu(new CityMenu(hWnd));
				game.setBackground(Game::Background::CITY_MENU);
				updateWindow(hWnd);
				break;
			}
			if ((HWND)lp == hItems[BUT_SAVE])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_LOAD])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_SETTINGS])
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
				hSubItems.resize(SettingsItem::SETTINGS_ITEM_NUMBER);

				hSubItems[SETTINGS_STAT_VIDEO] = (CreateWindow("STATIC", "Video settings (in progress)", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, 0, hInst, 0));
				hSubItems[SETTINGS_STAT_SOUND] = (CreateWindow("STATIC", "Sound: ", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, 0, hInst, 0));
				hSubItems[SETTINGS_BUT_SOUND] = (CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 0, 0, 0, 0, hWnd, 0, hInst, 0));
				hSubItems[SETTINGS_BUT_BACK] = (CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE, 0, 0, 0, 0, hWnd, 0, hInst, 0));

				currentSubMenu = Item::BUT_SETTINGS;
				game.setBackground(Game::Background::GAME_MENU_SETTINGS);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_EXIT_MENU])
			{
				game.getMenuManager().setMenu(new MainMenu(hWnd));
				game.setBackground(Game::Background::MAIN_MENU);
				updateWindow(hWnd);
				break;
			}
			break;
		case BUT_SETTINGS:
		{
			if ((HWND)lp == hSubItems[SETTINGS_BUT_SOUND])
			{
				// TODO
			}

			if ((HWND)lp == hSubItems[SETTINGS_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				currentSubMenu = Item::ITEM_NUMBER;
				game.setBackground(Game::Background::GAME_MENU_SETTINGS);

				updateWindow(hWnd);
				break;
			}
		}
			break;
		}
		break;
	}
}
