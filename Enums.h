#pragma once

enum AttackResult
{
	DEALT_DAMAGE,
	DEALT_CRIT_DAMAGE,
	STUNNED,
	WERE_DODGED,
	WERE_BLOCKED,
	WERE_COUNTERATTAKED
};

enum FightStatus
{
	CONTINUE,
	OPPONENT_LOST,
	OPPONNENT_SURRENDERED,
	PLAYER_SURRENDERED,
	PLAYER_LOST
};

enum Attribute
{
	STRENGTH,
	CONSTITUTION,
	DEXTERITY,
	WISDOM,
	INTELLIGENCE,
	CHARISMA
};

enum Limit { MIN, MAX };

enum Cities
{
	ROMA,
	CANUSIUM,
	METAPONTUM,
	BOVIANUM,
	ANCONA,
	PERUSINUS,
	FLORENCE,
	BONINA,
	GENUA,
	VENETIAE,
	MEDIOLANUM,
	CITIES_NUMBER
};
