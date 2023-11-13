#include "stdafx.h"
#include "NamedNPC.h"

NamedNPC::NamedNPC() : firstNameIndex(0), lastNameIndex(0) { }

NamedNPC::NamedNPC(int fNameIndex, int lNameIndex) :
	firstNameIndex(fNameIndex),
	lastNameIndex(lNameIndex)
{ }

NamedNPC::NamedNPC(const NamedNPC& C) :
	firstNameIndex(C.firstNameIndex),
	lastNameIndex(C.lastNameIndex)
{ }

NamedNPC& NamedNPC::operator=(const NamedNPC& C)
{
	if (this == &C) return *this;
	firstNameIndex = C.firstNameIndex;
	lastNameIndex = C.lastNameIndex;
	return *this;
}

NamedNPC::~NamedNPC() { }

void NamedNPC::rename(int fNameIndex, int lNameIndex)
{
}
