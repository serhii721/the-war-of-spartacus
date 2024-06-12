#include "stdafx.h"
#include "City.h"

City::City() : nameIndex(Cities::ROME), arena(), level(0), trader(), promotionAchieved(false) { }

City::City(int nnameIndex, const Arena& A, int llevel, const HarmlessNPC& ttrader) :
	nameIndex(nnameIndex),
	arena(A),
	level(llevel),
	trader(ttrader),
	promotionAchieved(false)
{ }

City::City(const City& C) :
	nameIndex(C.nameIndex),
	arena(C.arena),
	level(C.level),
	trader(C.trader),
	promotionAchieved(false)
{ }

City& City::operator=(const City& C)
{
	if (&C == this) return *this;

	nameIndex = C.nameIndex;
	arena = C.arena;
	level = C.level;
	trader = C.trader;
	promotionAchieved = C.promotionAchieved;

	return *this;
}

City::~City() { }

void City::setName(int s) { nameIndex = s; }

void City::setLevel(int l) { level = l; }

void City::setPromotion(bool b) { promotionAchieved = b; }

int City::getNameIndex() const { return nameIndex; }

Arena& City::getArena() { return arena; }

int City::getLevel() const { return level; }

HarmlessNPC & City::getTrader() { return trader; }

bool City::getPromotionStatus() const { return promotionAchieved; }
