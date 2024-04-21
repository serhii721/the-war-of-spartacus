#pragma once

class NamedNPC
{
protected:
	int firstNameIndex, lastNameIndex;
public:
	NamedNPC();
	NamedNPC(int firstNameIndex_, int lastNameIndex_);
	NamedNPC(const NamedNPC&);
	NamedNPC& operator=(const NamedNPC&);
	virtual ~NamedNPC();

	void rename(int, int);
	// TODO: const string& getName() const;
};
