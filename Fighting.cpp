#include "stdafx.h"
#include "Fighting.h"

extern HINSTANCE hInst;
extern Localization localization;
extern Game game;

Fighting::Fighting() :
	hItems(Item::ITEM_NUMBER),
	pOpponentCopy(nullptr),
	currentScreen(Screen::SCREEN_ITEM_NUMBER),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{ }

Fighting::Fighting(HWND hWnd) :
	hItems(Item::ITEM_NUMBER),
	pOpponentCopy(nullptr),
	currentScreen(Screen::SCREEN_ITEM_NUMBER),
	hBackgroundImage(NULL),
	hBackgroundBrush(NULL)
{
	for (int i = Item::STATIC_START; i <= Item::STATIC_FIGHT_RESULT; i++)
		hItems[i] = CreateWindow("STATIC", "", // TODO: apply localization
			WS_CHILD | WS_VISIBLE,
			0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[Item::EDIT_LOG_MESSAGES] = CreateWindow("EDIT", "", // TODO: apply localization
		WS_CHILD | WS_VISIBLE | ES_READONLY | ES_MULTILINE | ES_AUTOVSCROLL | ES_WANTRETURN,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[Item::BUT_SPARE_OPPONENT] = CreateWindow("BUTTON", "Spare opponent", // TODO: apply localization
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[Item::BUT_EXECUTE_OPPONENT] = CreateWindow("BUTTON", "Execute opponent", // TODO: apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[Item::BUT_SURRENDER] = CreateWindow("BUTTON", "Surrender", // TODO: apply localization
		WS_CHILD | WS_VISIBLE,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[Item::BUT_CONTINUE] = CreateWindow("BUTTON", "Continue fight", // TODO: apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0);

	hItems[Item::BUT_END_FIGHT] = CreateWindow("BUTTON", "End fight", // TODO: apply localization
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		0, 0, 0, 0, hWnd, 0, hInst, 0);


	ShowWindow(hItems[Item::BUT_SPARE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[Item::BUT_EXECUTE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[Item::BUT_SURRENDER], SW_HIDE);
	ShowWindow(hItems[Item::BUT_CONTINUE], SW_HIDE);
	ShowWindow(hItems[Item::BUT_END_FIGHT], SW_HIDE);
}

Fighting::Fighting(const Fighting& F) :
	hItems(),
	currentScreen(F.currentScreen),
	hBackgroundImage(F.hBackgroundImage),
	hBackgroundBrush(F.hBackgroundBrush)
{
	// Resizing items' vector
	int sz = F.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = F.hItems[i];
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

	if (F.pOpponentCopy)
	{
		if (pOpponentCopy)
			pOpponentCopy = make_shared<NPC>(*F.pOpponentCopy);
		else
			*pOpponentCopy = *F.pOpponentCopy;
	}
	else
		pOpponentCopy = nullptr;
}

Fighting& Fighting::operator=(const Fighting& F)
{
	if (&F == this) return *this;

	// Destroying the previous items of the menu
	for (HWND hItem : hItems)
		if (hItem != NULL)
			DestroyWindow(hItem);

	// Resizing items' vector
	int sz = F.hItems.size();
	hItems.resize(sz);

	HWND hItem;
	RECT rect;
	char className[256], windowText[256];
	for (int i = 0; i < sz; i++)
	{
		// Obtaining the previous parameters
		hItem = F.hItems[i];
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

	if (F.pOpponentCopy)
	{
		if (pOpponentCopy)
			pOpponentCopy = make_shared<NPC>(*F.pOpponentCopy);
		else
			*pOpponentCopy = *F.pOpponentCopy;
	}
	else
		pOpponentCopy = nullptr;

	currentScreen = F.currentScreen;
	hBackgroundImage = F.hBackgroundImage;
	hBackgroundBrush = F.hBackgroundBrush;

	return *this;
}

Fighting::~Fighting() { }

void Fighting::setScreen(Screen s)
{
	currentScreen = s;
}

FightStatus Fighting::fight(HWND hWnd, Player& rPlayer, shared_ptr<NPC> pOpponent)
{
	pOpponentCopy = pOpponent;
	AttackResult result = AttackResult::WERE_DODGED;
	FightStatus status = FightStatus::CONTINUE;

	int damage = 0, defense = 0;
	string buf;

	updateWindow(hWnd);

	// Show Player and Opponent health
	int playerHP = game.getPlayer().getHP(),
		opponentHP = pOpponentCopy->getHP();

	string pHP = "Health: " + to_string(playerHP),
		oHP = "Health: " + to_string(opponentHP);

	SendMessage(hItems[Item::STATIC_PLAYER_HP], WM_SETTEXT, 0, (LPARAM)(TCHAR*)pHP.c_str());
	SendMessage(hItems[Item::STATIC_OPPONENT_HP], WM_SETTEXT, 0, (LPARAM)(TCHAR*)oHP.c_str());

	// Show Player and Opponent damage
	if (rPlayer.getRightHand())
		damage += rPlayer.getRightHand()->getTotalDamage();
	if (rPlayer.getLeftHand())
		damage += rPlayer.getLeftHand()->getTotalDamage();
	buf = "Damage: " + to_string(damage);
	SendMessage(hItems[Item::STATIC_PLAYER_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
	damage = 0;

	if (pOpponent->getRightHand())
		damage += pOpponent->getRightHand()->getTotalDamage();
	if (pOpponent->getLeftHand())
		damage += pOpponent->getLeftHand()->getTotalDamage();
	buf = "Damage: " + to_string(damage);
	SendMessage(hItems[Item::STATIC_OPPONENT_DAMAGE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());
	damage = 0;

	// Show Player and Opponent defense
	if (rPlayer.getArmour())
		defense = rPlayer.getArmour()->getTotalDefense();
	buf = "Defense: " + to_string(defense);
	SendMessage(hItems[Item::STATIC_PLAYER_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	defense = 0;
	if (pOpponent->getArmour())
		defense = pOpponent->getArmour()->getTotalDefense();
	buf = "Defense: " + to_string(defense);
	SendMessage(hItems[Item::STATIC_OPPONENT_DEFENSE], WM_SETTEXT, 0, (LPARAM)(TCHAR*)buf.c_str());

	// # 1. Determining whether the opponent attacks first
	if (pOpponent->getWisdom() > rPlayer.getWisdom())
	{

		SendMessage(hItems[Item::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"Opponent has higher wisdom, so he attacks first\r\n");
		updateWindow(hWnd);
		Sleep(1000);

		// Opponent's attack
		pOpponent->attack(rPlayer, result, damage);

		getAttackResult(Attacker::OPPONENT, result, damage);
		updateWindow(hWnd);
		Sleep(1000);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, *pOpponent);

		if (status != FightStatus::CONTINUE)
		{
			getFightResult(status, rPlayer.getHP(), pOpponent->getHP());
			ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_SHOW);
			ShowWindow(hItems[Item::BUT_END_FIGHT], SW_SHOW);
			updateWindow(hWnd);
		}
	}
	else
	{
		SendMessage(hItems[Item::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"You have higher wisdom, so you attack first\r\n");
		updateWindow(hWnd);
		Sleep(1000);
	}

	// # 2. Fighting
	while (status == FightStatus::CONTINUE)
	{
		// Checking whether the player is not stunned
		if (result != AttackResult::STUNNED)
		{
			// Player's attack
			rPlayer.attack(*pOpponent, result, damage);

			// Output of the result of player's attack
			getAttackResult(Attacker::PLAYER, result, damage);
			updateWindow(hWnd);
			Sleep(1000);

			// Checking the status of the fighting
			status = checkFightStatus(rPlayer, *pOpponent);

			if (status != FightStatus::CONTINUE)
			{
				getFightResult(status, rPlayer.getHP(), pOpponent->getHP());
				ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_SHOW);
				ShowWindow(hItems[Item::BUT_END_FIGHT], SW_SHOW);
				updateWindow(hWnd);
				break;
			}
		}
		else
			result = AttackResult::DEALT_DAMAGE;

		// Checking whether the opponent is stunned
		if (result == AttackResult::STUNNED)
			continue; // Skip opponent's attack

		// Opponent's attack
		pOpponent->attack(rPlayer, result, damage);

		getAttackResult(Attacker::OPPONENT, result, damage);
		updateWindow(hWnd);
		Sleep(1000);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, *pOpponent);

		if (status != FightStatus::CONTINUE)
		{
			getFightResult(status, rPlayer.getHP(), pOpponent->getHP());
			ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_SHOW);
			ShowWindow(hItems[Item::BUT_END_FIGHT], SW_SHOW);
			updateWindow(hWnd);
			break;
		}
	}
	pOpponentCopy = nullptr;
	return status;
}

void Fighting::getAttackResult(const Attacker attacker, const AttackResult rresult, const int ddamage)
{
	// TODO: Localize
	char logText[1024];
	GetWindowText(hItems[Item::EDIT_LOG_MESSAGES], logText, sizeof(logText));

	string result(logText);

	switch (attacker)
	{
	case Attacker::PLAYER:
		switch (rresult)
		{
		case AttackResult::DEALT_DAMAGE:
			result += "You have dealt " + to_string(ddamage) +  " damage";
			break;
		case AttackResult::DEALT_CRIT_DAMAGE:
			result += "You have dealt " + to_string(ddamage) + " critical damage";
			break;
		case AttackResult::STUNNED:
			result += "You have stunned the opponent and dealt " + to_string(ddamage) + " damage";
			break;
		case AttackResult::WERE_DODGED:
			result += "The opponent has dodged";
			break;
		case AttackResult::WERE_BLOCKED:
			result += "The opponent has blocked and taken " + to_string(ddamage) + " damage";
			break;
		case AttackResult::WERE_COUNTERATTAKED:
			result += "The opponent has counterattacked and dealt " + to_string(ddamage) + " damage";
			break;
		default:
			// TODO: handle error
			break;
		}
		break;

	case Attacker::OPPONENT:
		switch (rresult)
		{
		case AttackResult::DEALT_DAMAGE:
			result += "Opponent have dealt you " + to_string(ddamage) + " damage";
			break;
		case AttackResult::DEALT_CRIT_DAMAGE:
			result += "Opponent have dealt you " + to_string(ddamage) + " critical damage";
			break;
		case AttackResult::STUNNED:
			result += "Opponent have stunned you and dealt " + to_string(ddamage) + " damage";
			break;
		case AttackResult::WERE_DODGED:
			result += "You have dodged";
			break;
		case AttackResult::WERE_BLOCKED:
			result += "You have blocked and taken " + to_string(ddamage) + " damage";
			break;
		case AttackResult::WERE_COUNTERATTAKED:
			result += "You have counterattacked and dealt " + to_string(ddamage) + " damage";
			break;
		default:
			// TODO: handle error
			return;
		}
		break;
	}
	result += "\r\n";
	SendMessage(hItems[Item::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)result.c_str());
	SendMessage(hItems[Item::EDIT_LOG_MESSAGES], EM_SCROLL, SB_BOTTOM, 0);

	int playerHP = game.getPlayer().getHP(),
		opponentHP = pOpponentCopy->getHP();

	string pHP = "Health: " + to_string(playerHP),
		   oHP = "Health: " + to_string(opponentHP);

	SendMessage(hItems[Item::STATIC_PLAYER_HP], WM_SETTEXT, 0, (LPARAM)(TCHAR*)pHP.c_str());
	SendMessage(hItems[Item::STATIC_OPPONENT_HP], WM_SETTEXT, 0, (LPARAM)(TCHAR*)oHP.c_str());
}

FightStatus Fighting::checkFightStatus(const Player & rPlayer, const NPC & rOpponent)
{
	if (rOpponent.getHP() < 10)
	{
		if (!rOpponent.isAlive())
			return FightStatus::OPPONENT_LOST;

		// Offer to surrender to the opponent
		// TODO: if (yes)
		//ShowWindow(hItems[Item::BUT_SPARE_OPPONENT], SW_SHOW);
		//ShowWindow(hItems[Item::BUT_EXECUTE_OPPONENT], SW_SHOW);
		// return FightStatus::OPPONNENT_SURRENDERED;
	}

	if (rPlayer.getHP() < 10)
	{
		if (!rPlayer.isAlive())
			return FightStatus::PLAYER_LOST;

		// Offer to surrender to the player
		// TODO: if (yes)
		//ShowWindow(hItems[Item::BUT_SURRENDER], SW_SHOW);
		//ShowWindow(hItems[Item::BUT_CONTINUE], SW_SHOW);
		// return FightStatus::PLAYER_SURRENDERED;
	}

	return FightStatus::CONTINUE;
}

void Fighting::getFightResult(const FightStatus sstatus, const int playerHP, const int opponentHP)
{
	// TODO: Localize
	string result;
	switch (sstatus)
	{
	case FightStatus::OPPONENT_LOST:
		result = "The opponent has lost";
		break;
	case FightStatus::OPPONNENT_SURRENDERED:
		result = "The opponent has surrendered with hit points of " + to_string(opponentHP);
		break;
	case FightStatus::PLAYER_SURRENDERED:
		result = "You have surrendered with hit points of " + to_string(playerHP);
		break;
	case FightStatus::PLAYER_LOST:
		result = "You have lost";
		break;
	case FightStatus::CONTINUE:
		break;
	default:
		// TODO: handle error
		break;
	}
	SendMessage(hItems[Item::STATIC_FIGHT_RESULT], WM_SETTEXT, 0, (LPARAM)(TCHAR*)result.c_str());
}

void Fighting::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/Background/";
	const string FORMAT = ".bmp";
	string path("");

	RECT rect;
	GetClientRect(hWnd, &rect);

	// 1. Background
	// Composing path based on current menu
	switch (currentScreen)
	{
	default:case Screen::SCREEN_ITEM_NUMBER: path = DIRECTORY + "" + FORMAT; break;
	case Screen::FIGHT_ARENA: path = DIRECTORY + "fightArenaBackground768" + FORMAT; break;
	}

	// Loading image
	hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Filling background with selected image
	hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
	FillRect(hdc, &rect, hBackgroundBrush);

	// 2. Text

}

void Fighting::resizeWindow(int cx, int cy)
{
	const int ITEM_HEIGHT = 30, ITEM_WIDTH = 300, DISTANCE = 15;

	int x = cx,
		y = cy;

	// Start
	MoveWindow(hItems[STATIC_START], x - (ITEM_WIDTH + DISTANCE) * 0.5, y - (ITEM_HEIGHT + DISTANCE) * 4, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_START]);

	// HP
	MoveWindow(hItems[STATIC_PLAYER_HP], x - ITEM_WIDTH - DISTANCE, y, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_PLAYER_HP]);

	MoveWindow(hItems[STATIC_OPPONENT_HP], x + DISTANCE + ITEM_WIDTH / 2, y, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_OPPONENT_HP]);

	// Damage
	MoveWindow(hItems[STATIC_PLAYER_DAMAGE], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT + DISTANCE, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_PLAYER_DAMAGE]);

	MoveWindow(hItems[STATIC_OPPONENT_DAMAGE], x + DISTANCE + ITEM_WIDTH / 2, y + ITEM_HEIGHT + DISTANCE, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_OPPONENT_DAMAGE]);

	// Defense
	MoveWindow(hItems[STATIC_PLAYER_DEFENSE], x - ITEM_WIDTH - DISTANCE, y + (ITEM_HEIGHT + DISTANCE) * 2, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_PLAYER_DEFENSE]);

	MoveWindow(hItems[STATIC_OPPONENT_DEFENSE], x + DISTANCE + ITEM_WIDTH / 2, y + (ITEM_HEIGHT + DISTANCE) * 2, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[STATIC_OPPONENT_DEFENSE]);

	// Log
	MoveWindow(hItems[EDIT_LOG_MESSAGES], x - (ITEM_WIDTH + DISTANCE) * 0.5, y, ITEM_WIDTH, ITEM_HEIGHT * 5, TRUE);
	UpdateWindow(hItems[EDIT_LOG_MESSAGES]);

	// Buttons
	MoveWindow(hItems[BUT_SPARE_OPPONENT], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[BUT_SPARE_OPPONENT]);

	MoveWindow(hItems[BUT_EXECUTE_OPPONENT], x + DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[BUT_EXECUTE_OPPONENT]);

	MoveWindow(hItems[BUT_SURRENDER], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[BUT_SURRENDER]);

	MoveWindow(hItems[BUT_CONTINUE], x + DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[BUT_CONTINUE]);

	// End of the fight
	MoveWindow(hItems[STATIC_FIGHT_RESULT], x - (ITEM_WIDTH + DISTANCE) * 0.5, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[BUT_END_FIGHT]);

	MoveWindow(hItems[BUT_END_FIGHT], x - (ITEM_WIDTH + DISTANCE) * 0.5, y + ITEM_HEIGHT * 6 + DISTANCE + 2, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
	UpdateWindow(hItems[BUT_END_FIGHT]);
}

void Fighting::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		switch (currentScreen)
		{
		case Screen::FIGHT_ARENA:
			if ((HWND)lp == hItems[BUT_END_FIGHT])
			{
				game.setDisplayState(DisplayState::MENU);

				for (HWND hItem : hItems)
				{
					if (hItem != NULL)
						DestroyWindow(hItem);
					hItems.clear();
				}

				game.getMenuManager().setMenu(new CityMenu(hWnd));
				updateWindow(hWnd);
			}
			break;

		default:case Screen::SCREEN_ITEM_NUMBER:
			break;
		}
		break;
	}
}