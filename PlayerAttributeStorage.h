#pragma once
class PlayerAttributeStorage
{
public:
	int unnassignedAttributes,
		strength, constitution, dexterity, intelligence, wisdom, charisma;

	PlayerAttributeStorage();
	PlayerAttributeStorage(const PlayerAttributeStorage&);
	PlayerAttributeStorage& operator=(const PlayerAttributeStorage&);
};
