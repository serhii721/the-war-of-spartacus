#pragma once

class Weapon
{
public:
	enum Type
	{
		SWORD,
		SPEAR,
		DAGGER,
		AXE,
		MACE,
		SHIELD,
		NUMBER
	};

	Weapon();
	Weapon(int, Type, int, int, int, int, int, const string&);
	Weapon(const Weapon&);
	Weapon& operator=(const Weapon&);
	~Weapon();

	void update(int, int);
	void setName(const string&);

	int getTotalDamage() const;
	bool isCompatibleWith(Type) const;

	int getDamage() const;
	Type getType() const;
	int getDamageAddition() const;
	int getShieldProbAddition() const;
	int getShieldDefPercentAddition() const;
	const string& getName() const;
private:
	int damage;

	Type type;
	int damageAddition;
	int strAdditionPerc, dexAdditionPerc;

	int shieldProbAddition, shieldDefPercentAddition;

	string name;
};
