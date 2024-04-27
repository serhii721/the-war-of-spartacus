#pragma once
#include "Fighter.h"
#include "NamedNPC.h"
#include "Leveling.h"

class NPC :
	public Fighter,
	public NamedNPC,
	public Leveling
{
public:
	NPC();
	NPC(const Fighter&, const NamedNPC&, const Leveling&);
	NPC(const NPC&);
	NPC& operator=(const NPC&);
	virtual ~NPC();
};
