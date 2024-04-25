#pragma once

class Fighting
{
private:
	vector<HWND> hItems;

	shared_ptr<NPC> pOpponentCopy;

	int currentScreen;

	HBITMAP hBackgroundImage;
	HBRUSH hBackgroundBrush;
public:
	enum Item
	{
		STATIC_START,
		STATIC_PLAYER_HP,
		STATIC_OPPONENT_HP,
		STATIC_PLAYER_DAMAGE,
		STATIC_OPPONENT_DAMAGE,
		STATIC_PLAYER_DEFENSE,
		STATIC_OPPONENT_DEFENSE,
		STATIC_FIGHT_RESULT,
		EDIT_LOG_MESSAGES,
		BUT_SPARE_OPPONENT,
		BUT_EXECUTE_OPPONENT,
		BUT_SURRENDER,
		BUT_CONTINUE,
		BUT_END_FIGHT,
		ITEM_NUMBER
	};
	enum Screen
	{
		FIGHT_ARENA,
		SCREEN_ITEM_NUMBER
	};
	enum Attacker
	{
		PLAYER,
		OPPONENT
	};

	Fighting();
	Fighting(HWND);
	Fighting(const Fighting&);
	Fighting& operator=(const Fighting&);
	~Fighting();

	void setScreen(Screen);

	FightStatus fight(HWND, Player&, shared_ptr<NPC>);
	void getAttackResult(const Attacker, const AttackResult, const int damage_);
	FightStatus checkFightStatus(const Player&, const NPC&);
	void getFightResult(const FightStatus, const int playerHP_, const int opponentHP_);

	void drawWindow(HWND, HDC, int, int);
	void resizeWindow(int, int);
	void handleInput(HWND, UINT, WPARAM, LPARAM);
};
