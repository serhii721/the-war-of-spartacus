#include "stdafx.h"
#include "MainMenu.h"

extern TCHAR str[256];
extern string buf;

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

MainMenu::MainMenu() :
	hItems(),
	hSubItems(),
	nms(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{ }

MainMenu::MainMenu(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	hSubItems(),
	nms(),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{
	char className[256] = "BUTTON";
	hItems[BUT_CONTINUE] = CreateWindow(className, "Continue", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);

	hItems[BUT_LOAD_GAME] = CreateWindow(className, "Load game", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_NEW_GAME] = CreateWindow(className, "New game", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SETTINGS] = CreateWindow(className, "Settings", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_SPECIALS] = CreateWindow(className, "Specials", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[BUT_EXIT] = CreateWindow(className, "Exit", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

MainMenu::MainMenu(const MainMenu& MM) : hSubItems()
{
	// Resizing items' vector
	int sz = MM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = MM.hItems[i];
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

	if (MM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(MM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (MM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(MM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	nms = make_unique<NewMenuStorage>();
}

MainMenu& MainMenu::operator=(const MainMenu& MM)
{
	if (&MM == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = MM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = MM.hItems[i];
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

	if (MM.nms)
	{
		if (!nms)
			nms = make_unique<NewMenuStorage>(*MM.nms);
		else
			*nms = *MM.nms;
	}
	else
		nms = nullptr;

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (MM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(MM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (MM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(MM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	return *this;
}

MainMenu::~MainMenu()
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

void MainMenu::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
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
		default:case Game::Background::MAIN_MENU:
			path = DIRECTORY + "menuBackground768" + FORMAT;
			for (HWND hItem : hItems)
				ShowWindow(hItem, SW_SHOW);
			break;
		case Game::Background::MAIN_MENU_LOAD:
			// TODO
			break;
		case Game::Background::MAIN_MENU_NEW_GAME: path = DIRECTORY + "characterCreationBackground768" + FORMAT; break;
		case Game::Background::MAIN_MENU_SETTINGS:
			// TODO
			break;
		case Game::Background::MAIN_MENU_SPECIALS:
			// TODO
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
	game.backgroundChangeCompleted();
}

void MainMenu::resizeMenu(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int sz, x, y, i;

	switch (game.getBackground())
	{
	default:case Game::Background::MAIN_MENU:
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

	case Game::Background::MAIN_MENU_NEW_GAME:
	{
		const int STAT_WIDTH = 125, EDIT_WIDTH = 40, BUT_WIDTH = 35, ITEM_HEIGHT = 25, DISTANCE = 5;
		cy -= 2 * (ITEM_HEIGHT + DISTANCE);
		y = cy;

		// Resizing static windows
		MoveWindow(hSubItems[STAT_CHARACTER_CREATION], 400, 40, 566, 55, TRUE);
		for (i = NewGameItem::STAT_NAME; i <= NewGameItem::STAT_CHARISMA; i++)
		{
			if (i != NewGameItem::STAT_UNNASSIGNED_ATTRIBUTES)
				MoveWindow(hSubItems[i], 557, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);
			else
			{
				MoveWindow(hSubItems[i], 557, y + ITEM_HEIGHT + DISTANCE, STAT_WIDTH + 40, ITEM_HEIGHT * 2, TRUE);
				y += ITEM_HEIGHT * 2 + DISTANCE;
			}
			y += ITEM_HEIGHT + DISTANCE;
		}

		// Resizing edit windows
		MoveWindow(hSubItems[EDIT_NAME], 687, cy, 116, ITEM_HEIGHT, TRUE);
		y = cy;
		for (i = NewGameItem::EDIT_AGE; i <= NewGameItem::EDIT_CHARISMA; i++)
		{
			y += ITEM_HEIGHT + DISTANCE;
			if (i != NewGameItem::EDIT_UNNASSIGNED_ATTRIBUTES)
				MoveWindow(hSubItems[i], 725, y, EDIT_WIDTH, ITEM_HEIGHT, TRUE);
			else
			{
				MoveWindow(hSubItems[i], 725, y + ITEM_HEIGHT + DISTANCE * 4, EDIT_WIDTH, ITEM_HEIGHT, TRUE);
				y += ITEM_HEIGHT * 2 + DISTANCE;
			}
		}

		// Resizing button windows
		y = cy + ITEM_HEIGHT + DISTANCE;
		for (i = NewGameItem::BUT_AGE_MINUS; i <= NewGameItem::BUT_CHARISMA_MINUS; i++)
		{
			if (i == NewGameItem::BUT_STRENGTH_MINUS)
				y += ITEM_HEIGHT * 3 + DISTANCE * 2;
			MoveWindow(hSubItems[i], 687, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			MoveWindow(hSubItems[i + 7], 768, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
			y += ITEM_HEIGHT + DISTANCE;
		}

		// Resizing menu navigation button windows
		y += ITEM_HEIGHT + DISTANCE;
		MoveWindow(hSubItems[NEW_GAME_BUT_BACK], 557, y, 120, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubItems[NEW_GAME_BUT_NEXT], 687, y, STAT_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;

	case Game::Background::MAIN_MENU_SETTINGS:
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

	case Game::Background::MAIN_MENU_SPECIALS:
	{
		const int DISTANCE = 100, ITEM_WIDTH = 300, ITEM_HEIGHT = 45;

		MoveWindow(hSubItems[SPECIALS_STAT_SPECIALS], cx - ITEM_WIDTH / 2, DISTANCE - (int)(ITEM_HEIGHT * 1.5), ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		MoveWindow(hSubItems[SPECIALS_STAT_TEXT], DISTANCE, DISTANCE, cx * 2 - DISTANCE * 2, cy * 2 - DISTANCE * 3, TRUE);

		MoveWindow(hSubItems[SPECIALS_BUT_BACK], cx - ITEM_WIDTH / 2, cy * 2 - DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	}
	break;
	}
}

void MainMenu::updateLanguage()
{
	// TODO: Apply localization
}

void MainMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	RECT windowRect;
	GetWindowRect(hWnd, &windowRect);

	switch (m)
	{
	case WM_COMMAND:
	{
		switch (game.getBackground())
		{
		case Game::Background::MAIN_MENU:
		{
			if ((HWND)lp == hItems[CONTINUE])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_LOAD_GAME])
			{
				// TODO
			}
			if ((HWND)lp == hItems[BUT_NEW_GAME])
			{
				nms = make_unique<NewMenuStorage>();

				// Hiding all buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				// Erasing previous sub menu items
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Creating new sub menu items
				hSubItems.resize(NewGameItem::NEW_GAME_ITEM_NUMBER);

				// TODO: Apply localization
				hSubItems[STAT_CHARACTER_CREATION] = CreateWindow("STATIC", "Character creation", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_NAME] = CreateWindow("STATIC", "Name:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_AGE] = CreateWindow("STATIC", "Age:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_UNNASSIGNED_ATTRIBUTES] = CreateWindow("STATIC", "Unnassigned attributes:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_STRENGTH] = CreateWindow("STATIC", "Strength:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_CONSTITUTION] = CreateWindow("STATIC", "Constitution:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_DEXTERITY] = CreateWindow("STATIC", "Dexterity:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_INTELLIGENCE] = CreateWindow("STATIC", "Intelligence:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_WISDOM] = CreateWindow("STATIC", "Wisdom:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[STAT_CHARISMA] = CreateWindow("STATIC", "Charisma:", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[EDIT_NAME] = CreateWindow("EDIT", "", WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_AGE] = CreateWindow("EDIT", to_string(nms->age).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES] = CreateWindow("EDIT", to_string(nms->unnassignedAttributes).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_STRENGTH] = CreateWindow("EDIT", to_string(nms->strength).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_CONSTITUTION] = CreateWindow("EDIT", to_string(nms->constitution).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_DEXTERITY] = CreateWindow("EDIT", to_string(nms->dexterity).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_INTELLIGENCE] = CreateWindow("EDIT", to_string(nms->intelligence).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_WISDOM] = CreateWindow("EDIT", to_string(nms->wisdom).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[EDIT_CHARISMA] = CreateWindow("EDIT", to_string(nms->charisma).c_str(), WS_CHILD | WS_VISIBLE | ES_READONLY | ES_CENTER, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[BUT_AGE_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_STRENGTH_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CONSTITUTION_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_DEXTERITY_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_INTELLIGENCE_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_WISDOM_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CHARISMA_MINUS] = CreateWindow("BUTTON", "-", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[BUT_AGE_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_STRENGTH_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CONSTITUTION_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_DEXTERITY_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_INTELLIGENCE_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_WISDOM_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[BUT_CHARISMA_PLUS] = CreateWindow("BUTTON", "+", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				hSubItems[NEW_GAME_BUT_BACK] = CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[NEW_GAME_BUT_NEXT] = CreateWindow("BUTTON", "Next", WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				game.setBackground(Game::Background::MAIN_MENU_NEW_GAME);

				SetFocus(hSubItems[EDIT_NAME]);

				updateWindow(hWnd);
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

				hSubItems[SETTINGS_STAT_VIDEO] = CreateWindow("STATIC", "Video settings (in progress)", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_STAT_SOUND] = CreateWindow("STATIC", "Sound: ", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_SOUND] = CreateWindow("BUTTON", "", WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SETTINGS_BUT_BACK] = CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				game.setBackground(Game::Background::MAIN_MENU_SETTINGS);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_SPECIALS])
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

				hSubItems[SPECIALS_STAT_SPECIALS] = CreateWindow("STATIC", "Specials", WS_CHILD | WS_VISIBLE | SS_CENTER | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SPECIALS_STAT_TEXT] = CreateWindow("STATIC", "Text", WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
				hSubItems[SPECIALS_BUT_BACK] = CreateWindow("BUTTON", "Back", WS_CHILD | WS_VISIBLE | BS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);

				game.setBackground(Game::Background::MAIN_MENU_SPECIALS);

				updateWindow(hWnd);
			}
			if ((HWND)lp == hItems[BUT_EXIT])
				DestroyWindow(hWnd);
		}
		break;

		case Game::Background::MAIN_MENU_NEW_GAME:
		{
			// Changing attributes
			// Age
			if ((HWND)lp == hSubItems[BUT_AGE_MINUS])
			{
				if (nms->age > MIN_AGE)
				{
					nms->age--;
					SetWindowText(hSubItems[EDIT_AGE], to_string(nms->age).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_AGE_PLUS])
			{
				if (nms->age < MAX_AGE)
				{
					nms->age++;
					SetWindowText(hSubItems[EDIT_AGE], to_string(nms->age).c_str());
				}
			}
			// Strength
			if ((HWND)lp == hSubItems[BUT_STRENGTH_MINUS])
			{
				if (nms->strength > MIN_STRENGTH)
				{
					nms->strength--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_STRENGTH], to_string(nms->strength).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_STRENGTH_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					nms->strength++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_STRENGTH], to_string(nms->strength).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Constitution
			if ((HWND)lp == hSubItems[BUT_CONSTITUTION_MINUS])
			{
				if (nms->constitution > MIN_CONSTITUTION)
				{
					nms->constitution--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_CONSTITUTION], to_string(nms->constitution).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_CONSTITUTION_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					nms->constitution++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_CONSTITUTION], to_string(nms->constitution).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Dexterity
			if ((HWND)lp == hSubItems[BUT_DEXTERITY_MINUS])
			{
				if (nms->dexterity > MIN_DEXTERITY)
				{
					nms->dexterity--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_DEXTERITY], to_string(nms->dexterity).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_DEXTERITY_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					nms->dexterity++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_DEXTERITY], to_string(nms->dexterity).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Intelligence
			if ((HWND)lp == hSubItems[BUT_INTELLIGENCE_MINUS])
			{
				if (nms->intelligence > MIN_INTELLIGENCE)
				{
					nms->intelligence--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_INTELLIGENCE], to_string(nms->intelligence).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_INTELLIGENCE_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					nms->intelligence++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_INTELLIGENCE], to_string(nms->intelligence).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Wisdom
			if ((HWND)lp == hSubItems[BUT_WISDOM_MINUS])
			{
				if (nms->wisdom > MIN_WISDOM)
				{
					nms->wisdom--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_WISDOM], to_string(nms->wisdom).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_WISDOM_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					nms->wisdom++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_WISDOM], to_string(nms->wisdom).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			// Charisma
			if ((HWND)lp == hSubItems[BUT_CHARISMA_MINUS])
			{
				if (nms->charisma > MIN_CHARISMA)
				{
					nms->charisma--;
					nms->unnassignedAttributes++;
					SetWindowText(hSubItems[EDIT_CHARISMA], to_string(nms->charisma).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}
			if ((HWND)lp == hSubItems[BUT_CHARISMA_PLUS])
			{
				if (nms->unnassignedAttributes > 0)
				{
					nms->charisma++;
					nms->unnassignedAttributes--;
					SetWindowText(hSubItems[EDIT_CHARISMA], to_string(nms->charisma).c_str());
					SetWindowText(hSubItems[EDIT_UNNASSIGNED_ATTRIBUTES], to_string(nms->unnassignedAttributes).c_str());
				}
			}

			// Return to main menu
			if ((HWND)lp == hSubItems[NEW_GAME_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				nms.reset();

				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}

			// Entering game
			if ((HWND)lp == hSubItems[NEW_GAME_BUT_NEXT])
			{
				// Player creation
				// Weapons
				unique_ptr<Weapon> rightHand = generateWeapon();
				unique_ptr<Weapon> leftHand;
				if (rightHand->getType() != Weapon::Type::AXE && rightHand->getType() != Weapon::Type::SPEAR)
				{
					leftHand = generateWeapon();
					if (!rightHand->isCompatibleWith(leftHand->getType()))
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
							} while (!rightHand->isCompatibleWith(leftHand->getType()));
						else if (leftHand)
						{
							leftHand.release();
							leftHand = nullptr;
						}
					}
				}
				else
					leftHand = nullptr;

				// Updating weapon damage depending on stat scale
				rightHand->update(nms->strength, nms->dexterity);
				if (leftHand)
					leftHand->update(nms->strength, nms->dexterity);

				// Armour
				unique_ptr<Armour> armour = generateArmour();

				// Updating armour defense depending on stat scale
				armour->update(nms->strength, nms->dexterity);

				if (SendMessage(hSubItems[EDIT_NAME], WM_GETTEXTLENGTH, 0, 0) > 0)
					SendMessage(hSubItems[EDIT_NAME], WM_GETTEXT, 256, (LPARAM)str);
				else
					strcpy_s(str, "Gladiator");

				game.setPlayer(
					Player(
						Fighter(
							Statistics(
								nms->strength,
								nms->constitution,
								nms->dexterity,
								nms->intelligence,
								nms->wisdom,
								nms->charisma,
								nms->age,
								MIN_FAME
							),
							BASIC_HP,
							BASIC_HP,
							move(rightHand),
							move(leftHand),
							move(armour)
						),
						Leveling(
							MIN_LEVEL,
							0,
							nms->unnassignedAttributes
						),
						string(str)
					)
				);
				game.getPlayer().updateMaxHP();

				nms.reset();

				// World creation
				unique_ptr<City> pCity;
				vector<City> cities;
				vector<int> cityLevels = {
					ROME_CITY_LEVEL, NAPLES_CITY_LEVEL, METAPONTO_CITY_LEVEL, BOJANO_CITY_LEVEL, ANCONA_CITY_LEVEL,
					PERUGIA_CITY_LEVEL, FLORENCE_CITY_LEVEL, BOLOGNA_CITY_LEVEL, GENOA_CITY_LEVEL, VENICE_CITY_LEVEL, MILAN_CITY_LEVEL
				};

				vector<unique_ptr<NPC>> gladiators;

				for (int i = 0; i < MAX_CITIES; i++)
				{
					// Creating opponents for arenas
					for (int j = 0; j < OPPONENTS_NUMBER; j++)
					{
						// Generating opponents of different levels for different cities
						gladiators.push_back(generateNPC(cityLevels[i]));
					}
					// Creating cities based of arenas
					pCity = make_unique<City>(Cities::ROME + i, Arena(gladiators), cityLevels[i]);
					cities.push_back(*pCity);
					gladiators.clear();
				}

				game.setWorldMap(WorldMap(hWnd, cities, Cities::PERUGIA));
				game.setFighting(Fighting(hWnd));

				game.getMenuManager().setMenu(new CityMenu(hWnd));
				game.setBackground(Game::Background::CITY_MENU);

				updateWindow(hWnd);
			}
		}
		break;

		case Game::Background::MAIN_MENU_SETTINGS:
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

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;

		case Game::Background::MAIN_MENU_SPECIALS:
		{
			if ((HWND)lp == hSubItems[SPECIALS_BUT_BACK] || LOWORD(wp) == IDCANCEL)
			{
				// Destroying all buttons
				for (HWND hItem : hSubItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hSubItems.clear();

				// Showing main menu buttons
				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_SHOW);

				game.setBackground(Game::Background::MAIN_MENU);

				updateWindow(hWnd);
				break;
			}
		}
		break;
		}
	}
	break;

	case WM_MOUSEMOVE:
	{
		static int i;
		// First method
		RECT rect;
		POINT pt;
		// Get cursor position
		GetCursorPos(&pt);
		ScreenToClient(hWnd, &pt);

		// Test
		HDC hdc;
		PAINTSTRUCT ps;
		buf = to_string(pt.x) + " " + to_string(pt.y);
		for (HWND hItem : hItems)
			if ((HWND)lp == hItem)
				SetWindowText(hWnd, buf.c_str());

		// Second method
		// Get cursor position
		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(TRACKMOUSEEVENT);
		tme.dwFlags = TME_HOVER | TME_LEAVE;
		tme.dwHoverTime = HOVER_DEFAULT;

		switch (game.getBackground())
		{
		case Game::Background::MAIN_MENU:
		{
			for (HWND hItem : hItems)
			{
				// Get button position
				GetWindowRect(hItem, &rect);
				MapWindowPoints(HWND_DESKTOP, hWnd, (LPPOINT)&rect, 2);
				//InvalidateRect(hWnd, &rect, 1);

				if (PtInRect(&rect, pt))
				{
					HBRUSH hBrush = CreateSolidBrush(COLOR_ROMAN_RED);
					SetWindowText(hItem, to_string(i).c_str());
					i++;
					hdc = BeginPaint(hWnd, &ps);
					DrawText(hdc, "Test", -1, &rect, DT_LEFT);
					FillRect(hdc, &rect, hBrush);
					EndPaint(hWnd, &ps);
					DeleteObject(hBrush);
				}
			}
		}
		break;

		case Game::Background::MAIN_MENU_LOAD:
		{
			// TODO
		}
		break;

		case Game::Background::MAIN_MENU_NEW_GAME:
		{
			// TODO
		}
		break;

		case Game::Background::MAIN_MENU_SETTINGS:
		{
			// TODO
		}
		break;

		case Game::Background::MAIN_MENU_SPECIALS:
		{
			// TODO
		}
		break;
		}
	}
	break;

	case WM_MOUSEHOVER:
	{
		SetWindowText(hWnd, "First stage succesful");
	}
	break;

	case WM_MOUSELEAVE:
	{
		SetWindowText(hWnd, "Second stage succesful");
	}
	break;
	}
}

void MainMenu::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
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

			if (item->CtlType == ODT_STATIC) // Static windows
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
