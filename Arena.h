#pragma once

class Arena
{
private:
	vector<unique_ptr<NPC>> gladiators;

public:
	Arena();
	Arena(const vector<unique_ptr<NPC>>& gladiators_);
	Arena(const Arena&);
	Arena& operator=(const Arena&);
	~Arena();

	// TODO: void update();
	unique_ptr<NPC>& getGladiator(int);
};
