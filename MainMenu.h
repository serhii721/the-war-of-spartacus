#pragma once
#include "IMenu.h"

class MainMenu :
	public IMenu
{
private:
	vector<HWND> hItems;
public:
	enum Item
	{
		CONTINUE,
		LOAD_GAME,
		NEW_GAME,
		OPTIONS,
		SPECIALS,
		EXIT,
		ITEM_NUMBER
	};

	MainMenu();
	MainMenu(HWND);
	MainMenu(const MainMenu&);
	MainMenu& operator=(const MainMenu&);
	virtual ~MainMenu();

	void drawMenu(HDC, int, int) override;
	void resizeMenu(int, int) override;
	void updateLanguage() override;
	void handleInput(HWND, UINT, WPARAM, LPARAM) override;
};
