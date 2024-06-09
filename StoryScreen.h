#pragma once
#include "IMenu.h"

class StoryScreen :
	public IMenu
{
private:
	vector<HWND> hItems;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

public:
	enum Screen
	{
		WELCOME_SCREEN,
		START_GAME_SCREEN,
		SCREEN_NUMBER
	};
	enum WelcomeScreenItems
	{
		WELCOME_SCREEN_STAT_DATE,
		WELCOME_SCREEN_STAT_TEXT,
		WELCOME_SCREEN_ITEM_NUMBER
	};
	enum StartGameScreenItems
	{
		START_GAME_STAT_TEXT,
		START_GAME_SCREEN_ITEM_NUMBER
	};

	StoryScreen();
	StoryScreen(HWND);
	StoryScreen(const StoryScreen&);
	StoryScreen& operator=(const StoryScreen&);
	virtual ~StoryScreen();

	void drawMenu(HWND, HDC, int, int) override;
	void resizeMenu(int, int) override;
	void handleInput(HWND, UINT, WPARAM, LPARAM) override;
	bool stylizeWindow(HWND, UINT, WPARAM, LPARAM, LRESULT&) override;
	void displayScreen(HWND, Screen);

private:
	Screen currentScreen;

public:
	void setCurrentScreen(Screen);

	Screen getCurrentScreen() const;

};
