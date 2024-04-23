#include "stdafx.h"
#include "Fighting.h"

Fighting::Fighting() { }

Fighting::~Fighting() { }

FightStatus Fighting::fight(Player& rPlayer, NPC& rOpponent)
{
	// # 1. Determining whether the opponent attacks first
	AttackResult result;
	int damage;

	FightStatus status;

	if (rOpponent.getWisdom() > rPlayer.getWisdom())
	{
		//output("Opponent has higher wisdom, so he attacks first.\n");

		// Opponent's attack
		rOpponent.attack(rPlayer, result, damage);

		outputOpponentAttackResult(result, damage);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, rOpponent);

		if (status != FightStatus::CONTINUE)
			outputFightResult(status, rPlayer.getHP(), rOpponent.getHP());
	}
	//else
		//output("You have higher wisdom, so you attack first.\n");

	// # 2. Fighting
	while (status == FightStatus::CONTINUE)
	{
		// Checking whether the player is not stunned
		if (result != AttackResult::STUNNED)
		{
			// Player's attack
			rPlayer.attack(rOpponent, result, damage);

			// Output of the result of player's attack
			switch (result)
			{
			case AttackResult::DEALT_DAMAGE:
				//output("You have dealt" + ' ' + to_string(damage) + ' ' + "damage");
				break;
			case AttackResult::DEALT_CRIT_DAMAGE:
				//output("You have dealt" + ' ' + to_string(damage) + ' ' + "critical damage");
				break;
			case AttackResult::STUNNED:
				//output("You have stunned the opponent and dealt" + ' ' + to_string(damage) + ' ' + "damage");
				break;
			case AttackResult::WERE_DODGED:
				//output("The opponent has dodged");
				break;
			case AttackResult::WERE_BLOCKED:
				//output("The opponent has blocked and taken" + ' ' + to_string(damage) + ' ' + "damage");
				break;
			case AttackResult::WERE_COUNTERATTAKED:
				//output("The opponent has counterattacked and dealt" + ' ' + to_string(damage) + ' ' + "damage");
				break;
			default:
				// TODO: handle error
				//outputError("Unknown attack result" + '\n');
				break;
			}
			//output(".\n");

			// Checking the status of the fighting
			status = checkFightStatus(rPlayer, rOpponent);

			if (status != FightStatus::CONTINUE)
			{
				outputFightResult(status, rPlayer.getHP(), rOpponent.getHP());
				break;
			}
		}

		// Checking whether the opponent is stunned
		if (result == AttackResult::STUNNED)
			continue; // Skip opponent's attack

		// Opponent's attack
		rOpponent.attack(rPlayer, result, damage);

		outputOpponentAttackResult(result, damage);

		// Checking the status of the fighting
		status = checkFightStatus(rPlayer, rOpponent);

		if (status != FightStatus::CONTINUE)
		{
			outputFightResult(status, rPlayer.getHP(), rOpponent.getHP());
			break;
		}
	}
	return status;
}

void Fighting::outputOpponentAttackResult(const AttackResult rresult, const int ddamage)
{
	switch (rresult)
	{
	case AttackResult::DEALT_DAMAGE:
		//output("You have been dealt" + ' ' + to_string(ddamage) + ' ' + "damage");
		break;
	case AttackResult::DEALT_CRIT_DAMAGE:
		//output("You have been dealt" + ' ' + to_string(ddamage) + ' ' + "critical damage");
		break;
	case AttackResult::STUNNED:
		//output("You have been stunned and been dealt" + ' ' + to_string(ddamage) + ' ' + "damage");
		break;
	case AttackResult::WERE_DODGED:
		//output("You have dodged");
		break;
	case AttackResult::WERE_BLOCKED:
		//output("You have blocked and taken" + ' ' + to_string(ddamage) + ' ' + "damage");
		break;
	case AttackResult::WERE_COUNTERATTAKED:
		//output("You have counterattacked and dealt" + ' ' + to_string(ddamage) + ' ' + "damage");
		break;
	default:
		//outputError("ERROR_UNKNOWN_ATTACK_RESULT" + '\n');
		return;
	}
	//output(".\n");
}

FightStatus Fighting::checkFightStatus(const Player & rPlayer, const NPC & rOpponent)
{
	if (rOpponent.getHP() < 10)
	{
		if (!rOpponent.isAlive())
			return FightStatus::OPPONENT_LOST;

		// Offer to surrender to the opponent
		// TODO: if (yes)
		// return FightStatus::OPPONNENT_SURRENDERED;
	}

	if (rPlayer.getHP() < 10)
	{
		if (!rPlayer.isAlive())
			return FightStatus::PLAYER_LOST;

		// Offer to surrender to the player
		// TODO: if (yes)
		// return FightStatus::PLAYER_SURRENDERED;
	}

	return FightStatus::CONTINUE;
}

void Fighting::outputFightResult(const FightStatus sstatus, const int playerHP, const int opponentHP)
{
	// TODO: localized
	switch (sstatus)
	{
	case FightStatus::OPPONENT_LOST:
		//output("The opponent has lost" + string(".\n"));
		break;
	case FightStatus::OPPONNENT_SURRENDERED:
		//output("The opponent has surrendered with hit points of" + ' ' + to_string(opponentHP) + ".\n");
		break;
	case FightStatus::PLAYER_SURRENDERED:
		//output("You have lost" + string(".\n"));
		break;
	case FightStatus::PLAYER_LOST:
		//output("You have surrendered with hit points of" + ' ' + to_string(playerHP) + ".\n");
		break;
	case FightStatus::CONTINUE:
		break;
	default:
		//outputError("ERROR_UNKNOWN_FIGHT_STATUS" + '\n');
		break;
	}
}

void Fighting::drawWindow(HWND hWnd, HDC hdc, int cx, int cy)
{
	const string DIRECTORY = "Data/Image/Background/";
	const string FORMAT = ".bmp";
	string path("");

	RECT rect;
	GetClientRect(hWnd, &rect);

	// Composing path based on current menu
	switch (currentScreen)
	{
	default:case ITEM_NUMBER: path = DIRECTORY + "" + FORMAT; break;
	}

	// Loading image
	hBackgroundImage = (HBITMAP)LoadImage(0, path.c_str(), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	// Filling background with selected image
	hBackgroundBrush = CreatePatternBrush(hBackgroundImage);
	FillRect(hdc, &rect, hBackgroundBrush);
}

void Fighting::resizeWindow(int cx, int cy)
{
}

void Fighting::handleInput(HWND hWnd, UINT m, WPARAM wp, LPARAM lp)
{
	switch (m)
	{
	case WM_COMMAND:
		break;
	}
}
