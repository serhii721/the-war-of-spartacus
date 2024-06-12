#pragma once
#include "IMenu.h"

class StoryScreen :
	public IMenu
{
private:
	vector<HWND> hItems;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

	bool isChoiceRequired;

public:
	enum Screen
	{
		WELCOME_SCREEN,
		START_GAME_SCREEN,
		FIRST_VICTORY,
		STOLEN_MONEY,
		SCREEN_NUMBER
	};
	enum WelcomeScreenItems
	{
		WELCOME_SCREEN_STAT_DATE,
		WELCOME_SCREEN_STAT_TEXT,
		WELCOME_SCREEN_ITEM_NUMBER
	};
	enum StolenMoneyScreenItems
	{
		STOLEN_MONEY_SCREEN_STAT_1,
		STOLEN_MONEY_SCREEN_STAT_2,
		STOLEN_MONEY_SCREEN_STAT_3,
		STOLEN_MONEY_SCREEN_STAT_ESCAPE,
		STOLEN_MONEY_SCREEN_STAT_STAY,
		STOLEN_MONEY_SCREEN_BUT_ESCAPE,
		STOLEN_MONEY_SCREEN_BUT_STAY,
		STOLEN_MONEY_SCREEN_ITEM_NUMBER
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
	int currentSubScreen;

public:
	void setCurrentScreen(Screen);
	void setCurrentSubScreen(int);

	Screen getCurrentScreen() const;
	int getCurrentSubScreen() const;

};
