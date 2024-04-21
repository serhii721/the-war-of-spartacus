#pragma once

struct NewMenuStorage
{
public:
	int age,
		unnassignedAttributes,
		strength, constitution, dexterity, intelligence, wisdom, charisma;

	NewMenuStorage();
	NewMenuStorage(const NewMenuStorage&);
	NewMenuStorage& operator=(const NewMenuStorage&);
};
