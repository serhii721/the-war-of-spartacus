#pragma once

class Arena
{
private:
	vector<NPC> gladiators;

public:
	Arena();
	Arena(const vector<NPC>& gladiators_);
	Arena(const Arena&);
	Arena& operator=(const Arena&);
	~Arena();

	// TODO: void update();
};
