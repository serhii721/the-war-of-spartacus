#pragma once

class NamedNPC
{
protected:
	int firstNameIndex, lastNameIndex;
public:
	NamedNPC();
	NamedNPC(int, int);
	NamedNPC(const NamedNPC&);
	NamedNPC& operator=(const NamedNPC&);
	virtual ~NamedNPC();

	void rename(int, int);
	// TODO: const string& getName() const;
};
