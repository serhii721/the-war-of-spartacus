#pragma once

class IMenu
{
public:
	virtual ~IMenu();

	virtual void drawMenu(HDC, int, int) = 0;
	virtual void resizeMenu(int, int) = 0;
	virtual void updateLanguage() = 0;
	virtual void handleInput(HWND, UINT, WPARAM, LPARAM) = 0;
};
