#pragma once

class Leveling
{
protected:
	int level;
public:
	Leveling();
	Leveling(int);
	Leveling(const Leveling&);
	Leveling& operator=(const Leveling&);
	virtual ~Leveling();

	int getLevel() const;
};
