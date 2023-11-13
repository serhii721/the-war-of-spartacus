#include "stdafx.h"
#include "NamedNPC.h"

NamedNPC::NamedNPC() : firstNameIndex(0), lastNameIndex(0) { }

NamedNPC::NamedNPC(int first, int last) : firstNameIndex(first), lastNameIndex(last) { }

NamedNPC::NamedNPC(const NamedNPC& N) :
	firstNameIndex(N.firstNameIndex),
	lastNameIndex(N.lastNameIndex)
{ }

NamedNPC& NamedNPC::operator=(const NamedNPC& N)
{
	firstNameIndex = N.firstNameIndex;
	lastNameIndex = N.lastNameIndex;
	return *this;
}

NamedNPC::~NamedNPC() { }

void NamedNPC::rename(int first, int last)
{
}
