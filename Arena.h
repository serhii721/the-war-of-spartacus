#pragma once

class Arena
{
private:
	vector<shared_ptr<NPC>> gladiators;

public:
	Arena();
	Arena(const vector<shared_ptr<NPC>>& gladiators_);
	Arena(const Arena&);
	Arena& operator=(const Arena&);
	~Arena();

	// TODO: void update();
	shared_ptr<NPC> getGladiator(int);
};
