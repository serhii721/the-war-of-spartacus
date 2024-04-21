#pragma once
#include "IMenu.h"

class CityMenu
	: public IMenu
{
private:
	vector<HWND> hItems;

	int currentSubMenu;
	vector<HWND> hSubItems;

public:
	enum Item
	{
		BUT_ARENA,
		BUT_QUEST,
		BUT_TRADER,
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

	CityMenu();
	CityMenu(HWND);
	CityMenu(const CityMenu&);
	CityMenu& operator=(const CityMenu&);
	virtual ~CityMenu();

	void drawMenu(HDC, int, int) override;
	void resizeMenu(int, int) override;
	void updateLanguage() override;
	void handleInput(HWND, UINT, WPARAM, LPARAM) override;
};
