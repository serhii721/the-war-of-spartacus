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
	// TODO: move Attribute and Limit over here?

	Weapon();
	Weapon(
		int damage_,
		Type type_,
		int damageAddition_,
		int strAdditionPerc_,
		int dexAdditionPerc_,
		int shieldProbAddition_,
		int shieldDefPercentAddition_,
		const string& name_
	);
	Weapon(const Weapon&);
	Weapon& operator=(const Weapon&);
	virtual ~Weapon();

	void update(int, int);
	void setName(const string&);

	int getTotalDamage() const;
	bool isCompatibleWith(Type) const;

	int getDamage() const;
	Type getType() const;
	int getDamageAddition() const;
	int getStrengthAdditionPercentage() const;
	int getDexterityAdditionPercentage() const;
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
