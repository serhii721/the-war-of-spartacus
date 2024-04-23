#pragma once
#include "IMenu.h"

class GameMenu :
	public IMenu
{
private:
	vector<HWND> hItems;

	int currentSubMenu;
	vector<HWND> hSubItems;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;

public:
	enum Item
	{
		BUT_RESUME,
		BUT_SAVE,
		BUT_LOAD,
		BUT_SETTINGS,
		BUT_EXIT_MENU,
		BUT_EXIT_DESKTOP,
		ITEM_NUMBER
	};
	enum SettingsItem
	{
		SETTINGS_STAT_VIDEO,
		SETTINGS_STAT_SOUND,
		SETTINGS_BUT_SOUND,
		SETTINGS_BUT_BACK,
		SETTINGS_ITEM_NUMBER
	};

	GameMenu();
	GameMenu(HWND);
	GameMenu(const GameMenu&);
	GameMenu& operator=(const GameMenu&);
	virtual ~GameMenu();

	void drawMenu(HWND, HDC, int, int) override;
	void resizeMenu(int, int) override;
	void updateLanguage() override;
	void handleInput(HWND, UINT, WPARAM, LPARAM) override;
};
