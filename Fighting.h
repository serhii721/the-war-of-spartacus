#pragma once

class Fighting
{
private:

public:
	Fighting();
	~Fighting();

	FightStatus fight(Player&, NPC&);
	void outputOpponentAttackResult(const AttackResult, const int damage_);
	FightStatus checkFightStatus(const Player&, const NPC&);
	void outputFightResult(const FightStatus, const int playerHP_, const int opponentHP_);

	void drawWindow(HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
};
