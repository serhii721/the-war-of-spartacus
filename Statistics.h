#pragma once

class Statistics
{
protected:
	int strength, constitution, dexterity, intelligence, wisdom, charisma;
	int age;
	int fame;
public:
	Statistics();
	Statistics(int, int, int, int, int, int, int, int);
	Statistics(const Statistics&);
	Statistics& operator=(const Statistics&);
	virtual ~Statistics();

	void setFame(int);

	bool isValid() const;
	int getStrength() const;
	int getConstitution() const;
	int getDexterity() const;
	int getIntelligence() const;
	int getWisdom() const;
	int getCharisma() const;
	int getAge() const;
	int getFame() const;
};
