#pragma once
#include "Statistics.h"
#include "NamedNPC.h"

class HarmlessNPC :
	public Statistics,
	public NamedNPC
{
public:
	HarmlessNPC();
	HarmlessNPC(const Statistics&, const NamedNPC&);
	HarmlessNPC(const HarmlessNPC&);
	HarmlessNPC& operator=(const HarmlessNPC&);
	virtual ~HarmlessNPC();
};
