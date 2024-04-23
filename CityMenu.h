#pragma once
#include "IMenu.h"

class CityMenu
	: public IMenu
{
private:
	vector<HWND> hItems;

	int currentSubMenu;
	vector<HWND> hSubItems;

	int currentSubMenuItem;
	vector<HWND> hSubMenuItems;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

public:
	enum Item
	{
		BUT_ARENA,
		BUT_QUEST,
		BUT_MARKET,
		BUT_REST,
		BUT_MAP,
		BUT_MENU,
		ITEM_NUMBER
	};
	enum ArenaItem
	{
		ARENA_BUT_FIGHT,
		ARENA_BUT_BET,
		ARENA_BUT_TRAIN,
		ARENA_BUT_BACK,
		ARENA_ITEM_NUMBER
	};
	enum ArenaFightItem
	{
		ARENA_FIGHT_BUT_OPPONENT1,
		ARENA_FIGHT_BUT_OPPONENT2,
		ARENA_FIGHT_BUT_OPPONENT3,
		ARENA_FIGHT_BUT_OPPONENT4,
		ARENA_FIGHT_BUT_OPPONENT5,
		ARENA_FIGHT_BUT_OPPONENT6,
		ARENA_FIGHT_BUT_OPPONENT7,
		ARENA_FIGHT_BUT_OPPONENT8,
		ARENA_FIGHT_BUT_OPPONENT9,
		ARENA_FIGHT_BUT_OPPONENT10,
		ARENA_FIGHT_BUT_OPPONENT11,
		ARENA_FIGHT_BUT_OPPONENT12,
		ARENA_FIGHT_BUT_OPPONENT13,
		ARENA_FIGHT_BUT_OPPONENT14,
		ARENA_FIGHT_BUT_OPPONENT15,
		ARENA_FIGHT_BUT_BACK,
		ARENA_FIGHT_BUT_FIGHT,
		ARENA_FIGHT_ITEM_NUMBER
	};

	CityMenu();
	CityMenu(HWND);
	CityMenu(const CityMenu&);
	CityMenu& operator=(const CityMenu&);
	virtual ~CityMenu();

	void drawMenu(HWND, HDC, int, int) override;
	void resizeMenu(int, int) override;
	void updateLanguage() override;
	void handleInput(HWND, UINT, WPARAM, LPARAM) override;
};
