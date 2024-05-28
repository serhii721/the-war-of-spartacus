#include "stdafx.h"
#include "City.h"

City::City() : nameIndex(Cities::ROME), arena(), level(0), trader() { }

City::City(int nnameIndex, const Arena& A, int llevel, const HarmlessNPC& ttrader) : nameIndex(nnameIndex), arena(A), level(llevel), trader(ttrader) { }

City::City(const City& C) : nameIndex(C.nameIndex), arena(C.arena), level(C.level), trader(C.trader) { }

City& City::operator=(const City& C)
{
	if (&C == this) return *this;

	nameIndex = C.nameIndex;
	arena = C.arena;
	level = C.level;
	trader = C.trader;

	return *this;
}

City::~City() { }

void City::setName(int s) { nameIndex = s; }

void City::setLevel(int l) { level = l; }

int City::getNameIndex() const { return nameIndex; }

Arena& City::getArena() { return arena; }

int City::getLevel() const { return level; }

HarmlessNPC & City::getTrader() { return trader; }
