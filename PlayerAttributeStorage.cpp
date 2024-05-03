#include "stdafx.h"
#include "PlayerAttributeStorage.h"

PlayerAttributeStorage::PlayerAttributeStorage() :
	unnassignedAttributes(STARTING_ADDITIONAL_ATTRIBUTES),
	strength(BASIC_ATTRIBUTES),
	constitution(BASIC_ATTRIBUTES),
	dexterity(BASIC_ATTRIBUTES),
	intelligence(BASIC_ATTRIBUTES),
	wisdom(BASIC_ATTRIBUTES),
	charisma(BASIC_ATTRIBUTES)
{ }

PlayerAttributeStorage::PlayerAttributeStorage(const PlayerAttributeStorage& PAS) :
	unnassignedAttributes(PAS.unnassignedAttributes),
	strength(PAS.strength),
	constitution(PAS.constitution),
	dexterity(PAS.dexterity),
	intelligence(PAS.intelligence),
	wisdom(PAS.wisdom),
	charisma(PAS.charisma)
{ }

PlayerAttributeStorage& PlayerAttributeStorage::operator=(const PlayerAttributeStorage& PAS)
{
	if (&PAS == this) return *this;

	unnassignedAttributes = PAS.unnassignedAttributes;
	strength = PAS.strength;
	constitution = PAS.constitution;
	dexterity = PAS.dexterity;
	intelligence = PAS.intelligence;
	wisdom = PAS.wisdom;
	charisma = PAS.charisma;
	return *this;
}
