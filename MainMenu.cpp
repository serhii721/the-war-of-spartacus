#include "stdafx.h"
#include "MainMenu.h"

extern HINSTANCE hInst;
extern Localization localization;

MainMenu::MainMenu() : hItems() { }

MainMenu::MainMenu(HWND hWnd) : hItems(ITEM_NUMBER)
{
	char className[256] = "BUTTON";
	hItems[CONTINUE] = CreateWindow(className, "Continue", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[LOAD_GAME] = CreateWindow(className, "Load game", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[NEW_GAME] = CreateWindow(className, "New game", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[OPTIONS] = CreateWindow(className, "Options", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[SPECIALS] = CreateWindow(className, "Specials", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
	hItems[EXIT] = CreateWindow(className, "Exit", // TODO: Apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0
	);
}

MainMenu::MainMenu(const MainMenu& MM)
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

	return *this;
}

MainMenu::~MainMenu()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);
}

void MainMenu::drawMenu(HDC, int, int) { }

void MainMenu::resizeMenu(int cx, int cy)
{
	const int ITEM_HEIGHT = 45, DISTANCE = 10, ITEM_WIDTH = 300;
	int sz = hItems.size();
	int x = cx - ITEM_WIDTH / 2,
		y = cy - sz / 2 * (ITEM_HEIGHT + DISTANCE);
	for (HWND hItem : hItems)
	{
		y += ITEM_HEIGHT + DISTANCE;
		MoveWindow(hItem, x, y, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		UpdateWindow(hItem);
	}
}

void MainMenu::updateLanguage()
{
	// TODO: Apply localization
}

void MainMenu::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		// TODO
		if ((HWND)lp == hItems[CONTINUE])
		{

		}
		if ((HWND)lp == hItems[LOAD_GAME])
		{

		}
		if ((HWND)lp == hItems[NEW_GAME])
		{

		}
		if ((HWND)lp == hItems[OPTIONS])
		{

		}
		if ((HWND)lp == hItems[SPECIALS])
		{

		}
		if ((HWND)lp == hItems[EXIT])
			DestroyWindow(hWnd);
		break;
	}
}
