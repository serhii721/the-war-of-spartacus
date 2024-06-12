#include "stdafx.h"
#include "StoryScreen.h"

extern TCHAR str[256];
extern string buf;

extern HINSTANCE hInst;
extern Localization l;
extern Game game;

StoryScreen::StoryScreen() : hItems(), hBackgroundImage(NULL), hBackgroundBrush(NULL), currentScreen(SCREEN_NUMBER), isChoiceRequired(false), currentSubScreen(-1) { }

StoryScreen::StoryScreen(HWND hWnd) : hItems(), hBackgroundImage(NULL), hBackgroundBrush(NULL), currentScreen(SCREEN_NUMBER), isChoiceRequired(false), currentSubScreen(-1) { }

StoryScreen::StoryScreen(const StoryScreen& SC) : isChoiceRequired(SC.isChoiceRequired), currentSubScreen(SC.currentSubScreen)
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

	isChoiceRequired = SC.isChoiceRequired;
	currentSubScreen = SC.currentSubScreen;

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
	const string DIRECTORY = "Data/Image/Story/";
	const string FORMAT = ".bmp";
	string path("");
	int i;

	RECT rect;
	GetClientRect(hWnd, &rect);

	// TODO: different background
	// 1. Background
	if (game.isBackgroundChanged())
	{
		switch (currentScreen)
		{
		case WELCOME_SCREEN:
		{

		}
		break;

		case STOLEN_MONEY:
		{
			switch (currentSubScreen)
			{
				// TODO
			}
		}
		break;

		case GAINED_FREEDOM:
		{
			switch (currentSubScreen)
			{
			case GAINED_FREEDOM_SCREEN_STAT_1: path = DIRECTORY + "gainedFreedom" + FORMAT; break;
			//case GAINED_FREEDOM_SCREEN_STAT_2: break;
			//case GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS: break;
			//case GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION: break;
			default: path = DIRECTORY + "blackScreen" + FORMAT; break;
			}
		}
		break;

		default: path = DIRECTORY + "blackScreen" + FORMAT; break;
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
		MoveWindow(hItems[0], cx - 250, cy - 120, 500, 220, TRUE);
	}
	break;

	case FIRST_VICTORY:
	{
		MoveWindow(hItems[0], cx - 250, cy - 120, 500, 150, TRUE);
	}
	break;

	case STOLEN_MONEY:
	{
		const int ITEM_HEIGHT = 280, ITEM_WIDTH = 500, BUTTON_HEIGHT = 30, BUTTON_WIDTH = 150, DISTANCE = 10;
		// Static windows
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_1], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_2], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_3], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_STAY], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_STAT_ESCAPE], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Buttons
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE], cx - BUTTON_WIDTH - DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
		MoveWindow(hItems[STOLEN_MONEY_SCREEN_BUT_STAY], cx + DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
	}
	break;

	case CENTURION_OFFER:
	{
		const int ITEM_HEIGHT = 350, ITEM_WIDTH = 500, BUTTON_HEIGHT = 30, BUTTON_WIDTH = 200, DISTANCE = 10;
		// Static windows
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_1], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_2], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_ACCEPT], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_STAT_DECLINE], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Buttons
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT], cx - BUTTON_WIDTH - DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
		MoveWindow(hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE], cx + DISTANCE, cy + 200, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
	}
	break;

	case GAINED_FREEDOM:
	{
		const int ITEM_HEIGHT = 350, ITEM_WIDTH = 500, BUTTON_HEIGHT = 35, BUTTON_WIDTH = 250, DISTANCE = 20;
		// Static windows
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_1], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_2], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION], cx - 250, cy - 200, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

		// Buttons
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS], cx - BUTTON_WIDTH - DISTANCE, cy + 250, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
		MoveWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION], cx + DISTANCE, cy + 250, BUTTON_WIDTH, BUTTON_HEIGHT, TRUE);
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
		case STOLEN_MONEY:
		{
			if ((HWND)lp == hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE])
			{
				// Player escapes and becomes free
				game.setProgressionStage(Game::Progression::CHOSE_REBELLION);
				game.getPlayer().setFreedom(true);
				currentSubScreen = STOLEN_MONEY_SCREEN_STAT_ESCAPE;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
			}

			if ((HWND)lp == hItems[STOLEN_MONEY_SCREEN_BUT_STAY])
			{
				// Player stays as gladiator on arena
				game.setProgressionStage(Game::Progression::STAYED_AFTER_GLADIATOR_OFFER);
				currentSubScreen = STOLEN_MONEY_SCREEN_STAT_STAY;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
			}
		}
		break;

		case CENTURION_OFFER:
		{
			if ((HWND)lp == hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT])
			{
				// Player joins legion
				game.setProgressionStage(Game::Progression::CHOSE_LEGION);
				game.getPlayer().setFreedom(true);
				currentSubScreen = CENTURION_OFFER_SCREEN_STAT_ACCEPT;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
			}

			if ((HWND)lp == hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE])
			{
				// Player stays as gladiator on arena
				game.setProgressionStage(Game::Progression::STAYED_AFTER_CENTURION_OFFER);
				currentSubScreen = CENTURION_OFFER_SCREEN_STAT_DECLINE;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
			}
		}
		break;

		case GAINED_FREEDOM:
		{
			if ((HWND)lp == hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS])
			{
				// Player joins rebels
				game.setProgressionStage(Game::Progression::CHOSE_REBELLION_AS_FREE);
				currentSubScreen = GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}

			if ((HWND)lp == hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION])
			{
				// Player joins legion
				game.setProgressionStage(Game::Progression::CHOSE_LEGION_AS_FREE);
				currentSubScreen = GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION;
				isChoiceRequired = false;

				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
				game.updateBackground();
				updateWindow(hWnd);
			}
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
			game.updateBackground();

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
			game.updateBackground();

			updateWindow(hWnd);
		}
		break;

		case FIRST_VICTORY:
		{
			// Destroying all windows
			for (HWND hItem : hItems)
				if (hItem != NULL)
					DestroyWindow(hItem);
			hItems.clear();

			game.setDisplayState(DisplayState::MENU);
			game.updateBackground();

			updateWindow(hWnd);
		}
		break;

		case STOLEN_MONEY:
		{
			if (!isChoiceRequired)
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
			}
			else if (currentSubScreen < STOLEN_MONEY_SCREEN_STAT_3)
			{
				currentSubScreen++;
				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
			}
			else
			{
				ShowWindow(hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE], SW_SHOW);
				ShowWindow(hItems[STOLEN_MONEY_SCREEN_BUT_STAY], SW_SHOW);
			}
			updateWindow(hWnd);
		}
		break;

		case CENTURION_OFFER:
		{
			if (!isChoiceRequired)
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
			}
			else if (currentSubScreen < CENTURION_OFFER_SCREEN_STAT_2)
			{
				currentSubScreen++;
				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
			}
			else
			{
				ShowWindow(hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT], SW_SHOW);
				ShowWindow(hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE], SW_SHOW);
			}
			updateWindow(hWnd);
		}
		break;

		case GAINED_FREEDOM:
		{
			if (!isChoiceRequired)
			{
				// Destroying all windows
				for (HWND hItem : hItems)
					if (hItem != NULL)
						DestroyWindow(hItem);
				hItems.clear();

				game.setDisplayState(DisplayState::MENU);
				game.updateBackground();
			}
			else if (currentSubScreen < GAINED_FREEDOM_SCREEN_STAT_2)
			{
				currentSubScreen++;
				for (HWND item : hItems)
					ShowWindow(item, SW_HIDE);
				ShowWindow(hItems[currentSubScreen], SW_SHOW);
			}
			else
			{
				ShowWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS], SW_SHOW);
				ShowWindow(hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION], SW_SHOW);
			}
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
			if (item->hwndItem == hItems[0])
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

		case FIRST_VICTORY:
		{
			if (item->hwndItem == hItems[0])
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

		case STOLEN_MONEY:
		{
			for (int i = STOLEN_MONEY_SCREEN_STAT_1; i <= STOLEN_MONEY_SCREEN_STAT_STAY; i++)
			{
				if (item->hwndItem == hItems[i])
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
		}
		break;

		case CENTURION_OFFER:
		{
			for (int i = CENTURION_OFFER_SCREEN_STAT_1; i <= CENTURION_OFFER_SCREEN_STAT_DECLINE; i++)
			{
				if (item->hwndItem == hItems[i])
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
		}
		break;

		case GAINED_FREEDOM:
		{
			for (int i = GAINED_FREEDOM_SCREEN_STAT_1; i <= GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION; i++)
			{
				if (item->hwndItem == hItems[i])
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

	isChoiceRequired = false;
	currentSubScreen = -1;

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
		hItems.resize(1);

		hItems[0] = CreateWindow("STATIC", l.getMessage(Localized::STORY_START_GAME_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;

	case FIRST_VICTORY:
	{
		hItems.resize(1);

		hItems[0] = CreateWindow("STATIC", l.getMessage(Localized::STORY_FIRST_VICTORY_TEXT).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
	}
	break;

	case STOLEN_MONEY:
	{
		hItems.resize(STOLEN_MONEY_SCREEN_ITEM_NUMBER);

		hItems[STOLEN_MONEY_SCREEN_STAT_1] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_TEXT1).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_2] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_TEXT2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_3] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_TEXT3).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_ESCAPE] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_ESCAPE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_STAT_STAY] = CreateWindow("STATIC", l.getMessage(Localized::STORY_STOLEN_MONEY_STAY).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_BUT_ESCAPE] = CreateWindow("BUTTON", l.getMessage(Localized::ESCAPE).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[STOLEN_MONEY_SCREEN_BUT_STAY] = CreateWindow("BUTTON", l.getMessage(Localized::STAY).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = STOLEN_MONEY_SCREEN_STAT_1;
	}
	break;

	case CENTURION_OFFER:
	{
		hItems.resize(CENTURION_OFFER_SCREEN_ITEM_NUMBER);

		hItems[CENTURION_OFFER_SCREEN_STAT_1] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_TEXT1).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_STAT_2] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_TEXT2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_STAT_ACCEPT] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_ACCEPT).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_STAT_DECLINE] = CreateWindow("STATIC", l.getMessage(Localized::STORY_CENTURION_OFFER_DECLINE).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_BUT_ACCEPT] = CreateWindow("BUTTON", l.getMessage(Localized::ACCEPT_OFFER).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[CENTURION_OFFER_SCREEN_BUT_DECLINE] = CreateWindow("BUTTON", l.getMessage(Localized::DECLINE_OFFER).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = CENTURION_OFFER_SCREEN_STAT_1;
	}
	break;

	case GAINED_FREEDOM:
	{
		hItems.resize(CENTURION_OFFER_SCREEN_ITEM_NUMBER);

		hItems[GAINED_FREEDOM_SCREEN_STAT_1] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_TEXT1).c_str(), WS_CHILD | WS_VISIBLE | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_STAT_2] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_TEXT2).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_REBELS] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_CHOSE_REBELS).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_STAT_CHOSE_LEGION] = CreateWindow("STATIC", l.getMessage(Localized::STORY_GAINED_FREEDOM_CHOSE_LEGION).c_str(), WS_CHILD | SS_OWNERDRAW, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_REBELS] = CreateWindow("BUTTON", l.getMessage(Localized::CHOOSE_REBELS).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);
		hItems[GAINED_FREEDOM_SCREEN_BUT_CHOOSE_LEGION] = CreateWindow("BUTTON", l.getMessage(Localized::CHOOSE_LEGION).c_str(), WS_CHILD | BS_OWNERDRAW | BS_PUSHBUTTON, 0, 0, 0, 0, hWnd, 0, hInst, 0);

		isChoiceRequired = true;
		currentSubScreen = GAINED_FREEDOM_SCREEN_STAT_1;
	}
	break;
	}
	currentScreen = screen;
	game.updateBackground();
	updateWindow(hWnd);
}

void StoryScreen::setCurrentScreen(Screen cs) { currentScreen = cs; }

void StoryScreen::setCurrentSubScreen(int screen) { currentSubScreen = screen; }

StoryScreen::Screen StoryScreen::getCurrentScreen() const { return currentScreen; }

int StoryScreen::getCurrentSubScreen() const { return currentSubScreen; }
