#include "stdafx.h"
#include "Fighting.h"

extern TCHAR str[256];
extern string buf;
extern string logStr;

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
			WS_CHILD | WS_VISIBLE | SS_CENTER,
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
	currentScreen(F.currentScreen)
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

	if (F.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(F.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (F.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(F.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;
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

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (F.hBackgroundImage != NULL)
	{
		BITMAP bm;
		GetObject(F.hBackgroundImage, sizeof(BITMAP), &bm);
		hBackgroundImage = CreateBitmapIndirect(&bm);
	}
	else
		hBackgroundImage = NULL;

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);

	if (F.hBackgroundBrush != NULL)
	{
		LOGBRUSH lb;
		GetObject(F.hBackgroundBrush, sizeof(LOGBRUSH), &lb);
		hBackgroundBrush = CreateBrushIndirect(&lb);
	}
	else
		hBackgroundBrush = NULL;

	return *this;
}

Fighting::~Fighting() { }

void Fighting::setScreen(Screen s)
{
	currentScreen = s;

	if (hBackgroundImage != NULL)
		DeleteObject(hBackgroundImage);

	if (hBackgroundBrush != NULL)
		DeleteObject(hBackgroundBrush);
}

FightStatus Fighting::fight(HWND hWnd, Player& rPlayer, shared_ptr<NPC> pOpponent)
{
	pOpponentCopy = pOpponent;
	AttackResult result = AttackResult::WERE_DODGED;
	FightStatus status = FightStatus::CONTINUE;

	int damage = 0, defense = 0;

	// Windows appearances
	for (HWND hItem : hItems)
		ShowWindow(hItem, SW_SHOW);

	ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_HIDE);
	ShowWindow(hItems[Item::BUT_SPARE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[Item::BUT_EXECUTE_OPPONENT], SW_HIDE);
	ShowWindow(hItems[Item::BUT_SURRENDER], SW_HIDE);
	ShowWindow(hItems[Item::BUT_CONTINUE], SW_HIDE);
	ShowWindow(hItems[Item::BUT_END_FIGHT], SW_HIDE);

	game.updateBackground();

	UpdateWindow(hWnd);

	// Fight name
	sprintf_s(str, "%s vs %s", rPlayer.getName().c_str(), localization.getNPCName(*pOpponent).c_str());
	SendMessage(hItems[Item::STATIC_START], WM_SETTEXT, 0, (LPARAM)str);

	// Show Player and Opponent health
	int playerHP = game.getPlayer().getHP(),
		opponentHP = pOpponentCopy->getHP();

	string pHP = "Health: " + to_string(playerHP),
		oHP = "Health: " + to_string(opponentHP);

	SendMessage(hItems[Item::STATIC_PLAYER_HP], WM_SETTEXT, 0, (LPARAM)(TCHAR*)pHP.c_str());
	SendMessage(hItems[Item::STATIC_OPPONENT_HP], WM_SETTEXT, 0, (LPARAM)(TCHAR*)oHP.c_str());
	SendMessage(hItems[Item::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"");

	// Show Player and Opponent damage
	if (rPlayer.getRightHand())
		damage += rPlayer.getRightHand()->getTotalDamage();
	if (rPlayer.getLeftHand())
		damage += rPlayer.getLeftHand()->getTotalDamage();
	sprintf_s(str, "Damage: %d", damage);
	SendMessage(hItems[Item::STATIC_PLAYER_DAMAGE], WM_SETTEXT, 0, (LPARAM)str);
	damage = 0;

	if (pOpponent->getRightHand())
		damage += pOpponent->getRightHand()->getTotalDamage();
	if (pOpponent->getLeftHand())
		damage += pOpponent->getLeftHand()->getTotalDamage();
	sprintf_s(str, "Damage: %d", damage);
	SendMessage(hItems[Item::STATIC_OPPONENT_DAMAGE], WM_SETTEXT, 0, (LPARAM)str);
	damage = 0;

	// Show Player and Opponent defense
	if (rPlayer.getArmour())
		defense = rPlayer.getArmour()->getTotalDefense();
	sprintf_s(str, "Defense: %d", defense);
	SendMessage(hItems[Item::STATIC_PLAYER_DEFENSE], WM_SETTEXT, 0, (LPARAM)str);

	defense = 0;
	if (pOpponent->getArmour())
		defense = pOpponent->getArmour()->getTotalDefense();
	sprintf_s(str, "Defense: %d", defense);
	SendMessage(hItems[Item::STATIC_OPPONENT_DEFENSE], WM_SETTEXT, 0, (LPARAM)str);

	// # 1. Determining whether the opponent attacks first
	if (pOpponent->getWisdom() > rPlayer.getWisdom())
	{

		SendMessage(hItems[Item::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"Opponent has higher wisdom, so he attacks first\r\n\r\n");
		SendMessage(hItems[Item::EDIT_LOG_MESSAGES], EM_SCROLL, SB_BOTTOM, 0);
		Sleep(SLEEP_TIME);

		// Opponent's attack
		pOpponent->attack(rPlayer, result, damage);

		getAttackResult(Attacker::OPPONENT, result, damage);
		Sleep(SLEEP_TIME);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, *pOpponent);

		if (status != FightStatus::CONTINUE)
		{
			if (status == FightStatus::PLAYER_LOST)
				rPlayer.setHP(1);
			getFightResult(status, rPlayer.getHP(), pOpponent->getHP());
			ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_SHOW);
			ShowWindow(hItems[Item::BUT_END_FIGHT], SW_SHOW);
		}
	}
	else
	{
		SendMessage(hItems[Item::EDIT_LOG_MESSAGES], WM_SETTEXT, 0, (LPARAM)(TCHAR*)"You have higher wisdom, so you attack first\r\n\r\n");
		SendMessage(hItems[Item::EDIT_LOG_MESSAGES], EM_SCROLL, SB_BOTTOM, 0);
		Sleep(SLEEP_TIME);
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
			Sleep(SLEEP_TIME);

			// Checking the status of the fighting
			status = checkFightStatus(rPlayer, *pOpponent);

			if (status != FightStatus::CONTINUE)
			{
				getFightResult(status, rPlayer.getHP(), pOpponent->getHP());
				ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_SHOW);
				ShowWindow(hItems[Item::BUT_END_FIGHT], SW_SHOW);
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
		Sleep(SLEEP_TIME);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, *pOpponent);

		if (status != FightStatus::CONTINUE)
		{
			if (status == FightStatus::PLAYER_LOST)
				rPlayer.setHP(1);
			getFightResult(status, rPlayer.getHP(), pOpponent->getHP());
			ShowWindow(hItems[Item::STATIC_FIGHT_RESULT], SW_SHOW);
			ShowWindow(hItems[Item::BUT_END_FIGHT], SW_SHOW);
			break;
		}
	}
	// # 3. Fight rewards
	// Gaining experience
	int experience = EXPERIENCE_PER_LEVEL;

	// Calculating experience based on opponent's power compared to player
	double experienceMultiplier = 1.0;
	// Level multiplier
	// If player's level greater that opponent's level - player gains 10% more experience
	// If difference is more than 5 levels - player gains another 40% more experience and vice versa
	if (rPlayer.getLevel() > pOpponent->getLevel())
	{
		experienceMultiplier -= 0.3;
		if (rPlayer.getLevel() - 5 > pOpponent->getLevel())
			experienceMultiplier -= 0.6;
	}
	else
	{
		experienceMultiplier += 0.1;
		if (rPlayer.getLevel() + 5 < pOpponent->getLevel())
			experienceMultiplier += 0.4;

	}
	// Stats multiplier
	int playerLevel = rPlayer.getLevel();
	int playerPhysStats = rPlayer.getStrength() + rPlayer.getConstitution() + rPlayer.getDexterity();
	int opponentPhysStats = pOpponent->getStrength() + pOpponent->getConstitution() + pOpponent->getDexterity();
	int statsDifference;
	// Player gains 10% more or less experience for every 10 physical stats difference from opponent
	if (playerPhysStats > opponentPhysStats)
	{
		statsDifference = playerPhysStats - opponentPhysStats;
		do
		{
			statsDifference -= 10;
			experienceMultiplier -= 0.1;
		} while (statsDifference > 0);
	}
	else
	{
		statsDifference = opponentPhysStats - playerPhysStats;
		do
		{
			statsDifference -= 10;
			experienceMultiplier += 0.1;
		} while (statsDifference > 0);
	}

	if (experienceMultiplier < MIN_EXPERIENCE_MULTIPLIER)
		experienceMultiplier = MIN_EXPERIENCE_MULTIPLIER;

	if (experienceMultiplier > MAX_EXPERIENCE_MULTIPLIER)
		experienceMultiplier = MAX_EXPERIENCE_MULTIPLIER;

	experience *= experienceMultiplier;
	rPlayer.gainExperience(experience);

	// Gaining fame
	// Fame number calculates based on experience multiplied by difference from opponent's fame
	int fame = (experience + pOpponent->getFame()) / 10;
	if (status == FightStatus::OPPONENT_LOST || status == FightStatus::OPPONNENT_SURRENDERED)
	{
		double fameMultiplier = 1.0;
		int playerFame = rPlayer.getFame();
		int opponentFame = pOpponent->getFame();
		int fameDifference;
		if (playerFame > opponentFame)
		{
			fameDifference = playerFame - opponentFame;
			do
			{
				fameMultiplier -= 0.1;
				fameDifference -= 100;
			} while (fameDifference > 0);
		}
		else
		{
			fameDifference = opponentFame - playerFame;
			do
			{
				fameMultiplier += 0.1;
				fameDifference -= 100;
			} while (fameDifference > 0);
		}
		fame *= fameMultiplier;
	}
	else
		fame /= 10;

	rPlayer.setFame(rPlayer.getFame() + fame);

	// Information for messages log
	logStr += "You have fought with " + localization.getNPCName(*pOpponent);
	if (status == FightStatus::OPPONENT_LOST || status == FightStatus::OPPONNENT_SURRENDERED)
		logStr += " and won\r\n\r\n";
	else
		logStr += " and lost\r\n\r\n";
	logStr += "You have gained " + to_string(fame) + " fame\r\n\r\n"
		+ "You have gained " + to_string(experience) + " experience\r\n\r\n";
	if (rPlayer.getLevel() > playerLevel)
		logStr += "You have leveled up to level " + to_string(rPlayer.getLevel()) +
		" (" + to_string(rPlayer.getUnnassignedAttributes()) + " unnassigned attributes)\r\n\r\n";

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
	result += "\r\n\r\n";
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
	if (game.isBackgroundChanged())
	{
		// 1. Background
		const string DIRECTORY = "Data/Image/Background/";
		const string FORMAT = ".bmp";
		string path("");
		RECT rect;

		GetClientRect(hWnd, &rect);

		// Composing path based on current menu
		switch (currentScreen)
		{
		default:case Screen::SCREEN_ITEM_NUMBER: path = DIRECTORY + "" + FORMAT; break;
		case Screen::FIGHT_ARENA: path = DIRECTORY + "fightArenaBackground768" + FORMAT; break;
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
	// TODO
}

void Fighting::resizeWindow(int cx, int cy)
{
	// If background is the same - no change needed
	if (!game.isBackgroundChanged())
		return;

	const int ITEM_HEIGHT = 30, ITEM_WIDTH = 300, DISTANCE = 15;

	int x = cx,
		y = cy;

	// Start
	MoveWindow(hItems[STATIC_START], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y - (ITEM_HEIGHT + DISTANCE) * 4, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

	// HP
	MoveWindow(hItems[STATIC_PLAYER_HP], x - ITEM_WIDTH - DISTANCE, y, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[STATIC_OPPONENT_HP], x + DISTANCE + ITEM_WIDTH / 2, y, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

	// Damage
	MoveWindow(hItems[STATIC_PLAYER_DAMAGE], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT + DISTANCE, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[STATIC_OPPONENT_DAMAGE], x + DISTANCE + ITEM_WIDTH / 2, y + ITEM_HEIGHT + DISTANCE, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

	// Defense
	MoveWindow(hItems[STATIC_PLAYER_DEFENSE], x - ITEM_WIDTH - DISTANCE, y + (ITEM_HEIGHT + DISTANCE) * 2, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[STATIC_OPPONENT_DEFENSE], x + DISTANCE + ITEM_WIDTH / 2, y + (ITEM_HEIGHT + DISTANCE) * 2, ITEM_WIDTH / 2, ITEM_HEIGHT, TRUE);

	// Log
	MoveWindow(hItems[EDIT_LOG_MESSAGES], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y, ITEM_WIDTH, ITEM_HEIGHT * 5, TRUE);

	// Buttons
	MoveWindow(hItems[BUT_SPARE_OPPONENT], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[BUT_EXECUTE_OPPONENT], x + DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[BUT_SURRENDER], x - ITEM_WIDTH - DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[BUT_CONTINUE], x + DISTANCE, y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

	// End of the fight
	MoveWindow(hItems[STATIC_FIGHT_RESULT], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y + ITEM_HEIGHT * 5 + DISTANCE, ITEM_WIDTH, ITEM_HEIGHT, TRUE);

	MoveWindow(hItems[BUT_END_FIGHT], x - (int)((ITEM_WIDTH + DISTANCE) * 0.5), y + ITEM_HEIGHT * 6 + DISTANCE + 2, ITEM_WIDTH, ITEM_HEIGHT, TRUE);
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
				game.setBackground(Game::Background::CITY_MENU);

				for (HWND hItem : hItems)
					ShowWindow(hItem, SW_HIDE);

				updateWindow(hWnd);
			}
			break;

		default:case Screen::SCREEN_ITEM_NUMBER:
			break;
		}
		break;
	}
}
