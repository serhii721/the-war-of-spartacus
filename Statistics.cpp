#include "stdafx.h"
#include "Statistics.h"

Statistics::Statistics() :
	strength(MIN_STRENGTH),
	constitution(MIN_CONSTITUTION),
	dexterity(MIN_DEXTERITY),
	intelligence(MIN_INTELLIGENCE),
	wisdom(MIN_WISDOM),
	charisma(MIN_CHARISMA),
	age(MIN_AGE),
	fame(BASIC_FAME)
{ }

Statistics::Statistics(
	int sstrength,
	int cconstitution,
	int ddexterity,
	int iintelligence,
	int wwisdom,
	int ccharisma,
	int aage,
	int ffame
) :
	strength(sstrength),
	constitution(cconstitution),
	dexterity(ddexterity),
	intelligence(iintelligence),
	wisdom(wwisdom),
	charisma(ccharisma),
	age(aage),
	fame(ffame)
{ }

Statistics::Statistics(const Statistics& S) :
	strength(S.strength),
	constitution(S.constitution),
	dexterity(S.dexterity),
	intelligence(S.intelligence),
	wisdom(S.wisdom),
	charisma(S.charisma),
	age(S.age),
	fame(S.fame)
{ }

Statistics& Statistics::operator=(const Statistics& S)
{
	if (this == &S) return *this;
	strength = S.strength;
	constitution = S.constitution;
	dexterity = S.dexterity;
	intelligence = S.intelligence;
	wisdom = S.wisdom;
	charisma = S.charisma;
	age = S.age;
	fame = S.fame;
	return *this;
}

Statistics::~Statistics() { }

void Statistics::setFame(int ffame)
{
	if (ffame < 0)
		throw(new exception("Negative fame"));

	fame = ffame;
}

bool Statistics::isValid() const
{
	if (
		strength < MIN_STRENGTH || strength > MAX_STRENGTH ||
		constitution < MIN_CONSTITUTION || constitution > MAX_CONSTITUTION ||
		dexterity < MIN_DEXTERITY || dexterity > MAX_DEXTERITY ||
		intelligence < MIN_INTELLIGENCE || intelligence > MAX_INTELLIGENCE ||
		wisdom < MIN_WISDOM || wisdom > MAX_WISDOM ||
		charisma < MIN_CHARISMA || charisma > MAX_CHARISMA ||
		age < MIN_AGE || age > MAX_AGE ||
		fame < MIN_FAME
		)
		return false;
	return true;
}

int Statistics::getStrength() const { return strength; }

int Statistics::getConstitution() const { return constitution; }

int Statistics::getDexterity() const { return dexterity; }

int Statistics::getIntelligence() const { return intelligence; }

int Statistics::getWisdom() const { return wisdom; }

int Statistics::getCharisma() const { return charisma; }

int Statistics::getAge() const { return age; }

int Statistics::getFame() const { return fame; }
