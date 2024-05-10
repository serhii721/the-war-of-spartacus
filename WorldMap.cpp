#include "stdafx.h"
#include "WorldMap.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

WorldMap::WorldMap() :
	hItems(Item::ITEM_NUMBER),
	cities(),
	currentCity(0),
	selectedCity(0),
	currentScreen(0),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{ }

WorldMap::WorldMap(HWND hWnd, const vector<City>& C, int ccurrentCity) :
	hItems(Item::ITEM_NUMBER),
	cities(C),
	currentCity(ccurrentCity),
	selectedCity(ccurrentCity),
	currentScreen(0),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{
	hItems[STAT_MAP] = CreateWindow("STATIC", "Map", // TODO: Localization
		WS_CHILD | WS_VISIBLE | SS_CENTER,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	int i = BUT_ROME_MAP;
	for (; i <= BUT_MILAN_MAP; i++)
		hItems[i] = CreateWindow("BUTTON", localization.getCityName(C[i]).c_str(),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
			0, 0, 0, 0, hWnd, 0, hInst, 0);

	for (i = BUT_ROME_LIST; i <= BUT_MILAN_LIST; i++)
		hItems[i] = CreateWindow("BUTTON", localization.getCityName(C[i - BUT_ROME_LIST]).c_str(),
			WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON | BS_AUTOCHECKBOX | BS_LEFTTEXT,
			0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[BUT_TRAVEL_LIST] = CreateWindow("BUTTON", "Travel", // TODO: Localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[BUT_ENTER_LIST] = CreateWindow("BUTTON", "Enter", // TODO: Localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0);
}

WorldMap::WorldMap(const WorldMap& WM) :
	cities(WM.cities),
	currentCity(WM.currentCity),
	selectedCity(WM.selectedCity),
	currentScreen(WM.currentScreen)
{
	// Resizing items' vector
	int sz = WM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = WM.hItems[i];
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

	if (WM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(WM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (WM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(WM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
}

WorldMap& WorldMap::operator=(const WorldMap& WM)
{
	if (&WM == this) return *this;

	cities = WM.cities;
	currentCity = WM.currentCity;
	selectedCity = WM.selectedCity;
	currentScreen = WM.currentScreen;
	hBackgroundImage = WM.hBackgroundImage;
	hBackgroundBrush = WM.hBackgroundBrush;

	// Resizing items' vector
	int sz = WM.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = WM.hItems[i];
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

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (WM.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(WM.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (WM.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(WM.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	return *this;
}

WorldMap::~WorldMap()
{
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

City& WorldMap::getCurrentCity()
{
	return cities[currentCity];
}

void WorldMap::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
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
		default:case Game::Background::WORLD_MAP:
			path = DIRECTORY + "mapBackground768" + FORMAT;
			for (HWND hItem : hItems)
				ShowWindow(hItem, SW_SHOW);
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

	// 2. Text
	switch (game.getBackground())
	{
	default:case Game::Background::WORLD_MAP:
		if (currentCity == selectedCity)
		{
			ShowWindow(hItems[BUT_TRAVEL_LIST], SW_HIDE);
			ShowWindow(hItems[BUT_ENTER_LIST], SW_SHOW);
		}
		else
		{
			ShowWindow(hItems[BUT_TRAVEL_LIST], SW_SHOW);
			ShowWindow(hItems[BUT_ENTER_LIST], SW_HIDE);
		}

		for (int i = Cities::ROME; i <= Cities::MILAN; i++)
		{
			buf = localization.getCityName(cities[i]);
			SendMessage(hItems[i], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
		}
		break;
	}
}

void WorldMap::resizeWindow(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	int x, y, i;

	const int ITEM_HEIGHT = 40, ITEM_WIDTH = 300, BUT_WIDTH = 336, DISTANCE = 3;
	const int FRAME_HEIGHT = 28, FRAME_WIDTH = 110;


	MoveWindow(hItems[BUT_ROME_MAP], 417, 519, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_NAPLES_MAP], 542, 602, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_METAPONTO_MAP], 729, 665, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_BOJANO_MAP], 558, 528, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_ANCONA_MAP], 458, 315, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_PERUGIA_MAP], 395, 365, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_FLORENCE_MAP], 296, 335, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_BOLOGNA_MAP], 314, 238, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_GENOA_MAP], 147, 207, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_VENICE_MAP], 351, 108, FRAME_WIDTH, FRAME_HEIGHT, TRUE);
	MoveWindow(hItems[BUT_MILAN_MAP], 119, 91, FRAME_WIDTH, FRAME_HEIGHT, TRUE);

	x = 982, y = 126;
	for (i = BUT_ROME_LIST; i <= BUT_MILAN_LIST; i++)
	{
		MoveWindow(hItems[i], x, y, BUT_WIDTH, ITEM_HEIGHT, TRUE);
		y += ITEM_HEIGHT + DISTANCE;
	}

	MoveWindow(hItems[STAT_MAP], x, 9, BUT_WIDTH, 72, TRUE);

	MoveWindow(hItems[BUT_TRAVEL_LIST], x, 644, BUT_WIDTH, 67, TRUE);

	MoveWindow(hItems[BUT_ENTER_LIST], x, 644, BUT_WIDTH, 67, TRUE);

	for (HWND hItem : hItems)
		UpdateWindow(hItem);
}

void WorldMap::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		// Selecting city
		{
		if ((HWND)lp == hItems[BUT_ROME_LIST] || (HWND)lp == hItems[BUT_ROME_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_ROME_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_ROME_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_NAPLES_LIST] || (HWND)lp == hItems[BUT_NAPLES_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_NAPLES_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_NAPLES_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_METAPONTO_LIST] || (HWND)lp == hItems[BUT_METAPONTO_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_METAPONTO_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_METAPONTO_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_BOJANO_LIST] || (HWND)lp == hItems[BUT_BOJANO_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_BOJANO_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_BOJANO_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_ANCONA_LIST] || (HWND)lp == hItems[BUT_ANCONA_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_ANCONA_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_ANCONA_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_PERUGIA_LIST] || (HWND)lp == hItems[BUT_PERUGIA_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_PERUGIA_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_PERUGIA_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_FLORENCE_LIST] || (HWND)lp == hItems[BUT_FLORENCE_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_FLORENCE_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_FLORENCE_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_BOLOGNA_LIST] || (HWND)lp == hItems[BUT_BOLOGNA_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_BOLOGNA_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_BOLOGNA_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_GENOA_LIST] || (HWND)lp == hItems[BUT_GENOA_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_GENOA_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_GENOA_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_VENICE_LIST] || (HWND)lp == hItems[BUT_VENICE_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_VENICE_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_VENICE_MAP;
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_MILAN_LIST] || (HWND)lp == hItems[BUT_MILAN_MAP])
		{
			for (HWND hItem : hItems)
				SendMessage(hItem, BM_SETCHECK, 0, 0);
			SendMessage(hItems[BUT_MILAN_LIST], BM_SETCHECK, 1, 0);
			selectedCity = BUT_MILAN_MAP;
			updateWindow(hWnd);
		}
		}
		if ((HWND)lp == hItems[BUT_TRAVEL_LIST])
		{
			currentCity = selectedCity;
			logStr += "You have traveled to " + localization.getCityName(getCurrentCity()) + "\r\n";
			updateWindow(hWnd);
		}
		if ((HWND)lp == hItems[BUT_ENTER_LIST])
		{
			for (HWND hItem : hItems)
				ShowWindow(hItem, SW_HIDE);

			game.setDisplayState(DisplayState::MENU);
			game.setBackground(Game::Background::CITY_MENU);
			updateWindow(hWnd);
		}
		break;
	}
}
