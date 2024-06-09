#include "stdafx.h"
#include "StoryScreen.h"

extern TCHAR str[256];
extern string buf;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

StoryScreen::StoryScreen() : hItems(), hBackgroundImage(NULL), hBackgroundBrush(NULL), currentScreen(SCREEN_NUMBER) { }

StoryScreen::StoryScreen(HWND hWnd) : hItems(), hBackgroundImage(NULL), hBackgroundBrush(NULL), currentScreen(SCREEN_NUMBER) { }

StoryScreen::StoryScreen(const StoryScreen& SC)
{
	// Resizing items' vector
	int sz = SC.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = SC.hItems[i];
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

	if (SC.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(SC.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (SC.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(SC.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

StoryScreen& StoryScreen::operator=(const StoryScreen& SC)
{
	if (&SC == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = SC.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = SC.hItems[i];
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

	// Image
	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (SC.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(SC.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (SC.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(SC.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	return *this;
}

StoryScreen::~StoryScreen()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

void StoryScreen::drawMenu(HWND hWnd, HDC hdc, int cx, int cy)
{
	switch (currentScreen)
	{
	case WELCOME_SCREEN:
	{

	}
	break;
	}
}

void StoryScreen::resizeMenu(int cx, int cy)
{
	int sz, x, y, i;
	switch (currentScreen)
	{
	case WELCOME_SCREEN:
	{
		y = cy - 120;
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300;
		MoveWindow(hItems[WELCOME_SCREEN_STAT_DATE], cx - ITEM_WIDTH / 2, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[WELCOME_SCREEN_STAT_TEXT], cx - 250, y + ITEM_HEIGHT + DISTANCE, 500, 150, TRUE);
	}
	break;

	case START_GAME_SCREEN:
	{
		y = cy - 120;
		const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300;
		MoveWindow(hItems[START_GAME_STAT_TEXT], cx - 250, y, 500, 220, TRUE);
	}
	break;
	}
}

void StoryScreen::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
	{
		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{

		}
		break;

		case START_GAME_SCREEN:
		{

		}
		break;
		}
	}
	break;
	case WM_LBUTTONDOWN:
	{
		// Delay so player won't accidentaly skip a scene
		Sleep(STORY_SLEEP_TIME);
		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{
			// Destroying all windows
			for (HWND hItem : hItems)
				if (hItem != NULL)
					DestroyWindow(hItem);
			hItems.clear();

			game.setDisplayState(DisplayState::MENU);

			updateWindow(hWnd);
		}
		break;

		case START_GAME_SCREEN:
		{
			// Destroying all windows
			for (HWND hItem : hItems)
				if (hItem != NULL)
					DestroyWindow(hItem);
			hItems.clear();

			game.setDisplayState(DisplayState::MENU);

			updateWindow(hWnd);
		}
		break;
		}
	}
	break;
	}
}

bool StoryScreen::stylizeWindow(HWND hWnd, UINT m, WPARAM wp, LPARAM lp, LRESULT& result)
{
	switch (m)
	{
	case WM_DRAWITEM:
	{
		LPDRAWITEMSTRUCT item = (LPDRAWITEMSTRUCT)lp;
		HDC hdc = item->hDC;

		GetClassName(item->hwndItem, str, sizeof(str) / sizeof(str[0]));

		SelectObject(hdc, game.getFont(Game::FontSize::MEDIUM));
		SetBkMode(hdc, TRANSPARENT);

		// Get text
		int len = GetWindowTextLength(item->hwndItem);
		buf.resize(len + 1); // Resize buffer to contain button text
		GetWindowTextA(item->hwndItem, &buf[0], len + 1); // Write text into buffer

		SetTextColor(hdc, COLOR_WHITE); // Set basic text color

		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{
			if (item->hwndItem == hItems[WELCOME_SCREEN_STAT_DATE])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				DrawTextA(item->hDC, buf.c_str(), len, &item->rcItem, DT_SINGLELINE | DT_VCENTER | DT_CENTER); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
			if (item->hwndItem == hItems[WELCOME_SCREEN_STAT_TEXT])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				// Text padding
				RECT rect = item->rcItem;
				int padding = 10;
				rect.left += padding;
				rect.top += padding;
				rect.right -= padding;
				rect.bottom -= padding;

				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
		}
		break;

		case START_GAME_SCREEN:
		{
			if (item->hwndItem == hItems[START_GAME_STAT_TEXT])
			{
				SelectObject(hdc, game.getFont(Game::FontSize::LARGE));
				SetBkMode(hdc, TRANSPARENT);

				FillRect(hdc, &item->rcItem, CreateSolidBrush(COLOR_DARK_BLUE)); // Fill background
				// Text padding
				RECT rect = item->rcItem;
				int padding = 10;
				rect.left += padding;
				rect.top += padding;
				rect.right -= padding;
				rect.bottom -= padding;

				DrawTextA(item->hDC, buf.c_str(), len, &rect, DT_VCENTER | DT_LEFT | DT_WORDBREAK); // Display text
				DrawEdge(hdc, &item->rcItem, EDGE_SUNKEN, BF_RECT); // Draw edge
				return true;
			}
		}
		break;
		}
	}
	break;
	}
	return false;
}

void StoryScreen::displayScreen(HWND hWnd, Screen screen)
{
	game.setDisplayState(DisplayState::STORY_SCREEN);

	// Destroying all previous windows
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);
	hItems.clear();

	// Create new windows
	switch (screen)
	{
	case WELCOME_SCREEN:
	{
		hItems.resize(WELCOME_SCREEN_ITEM_NUMBER);

		hItems[WELCOME_SCREEN_STAT_DATE] = CreateWindow("STATIC", l.getMessage(Localized::STORY_WELCOME_SCREEN_DATE).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[WELCOME_SCREEN_STAT_TEXT] = CreateWindow("STATIC", l.getMessage(Localized::STORY_WELCOME_SCREEN_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;

	case START_GAME_SCREEN:
	{
		hItems.resize(START_GAME_SCREEN_ITEM_NUMBER);

		hItems[START_GAME_STAT_TEXT] = CreateWindow("STATIC", l.getMessage(Localized::STORY_START_GAME_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;
	}
	currentScreen = screen;
	updateWindow(hWnd);
}

void StoryScreen::setCurrentScreen(Screen cs) { currentScreen = cs; }

StoryScreen::Screen StoryScreen::getCurrentScreen() const { return currentScreen; }
