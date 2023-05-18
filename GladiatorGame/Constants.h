#pragma once

const int OPPONENTS_NUMBER = 3;
const int BASIC_REGEN = 5;
const int BASIC_PLAYER_ATTRIBUTES = 10;
const string OUTPUT_DIVIDER = "\n______________________________________________\n\n\n";

// Gladiator's statistics
const unsigned MIN_AGE = 20, MAX_AGE = 30;
const int BASIC_HEALTH = 100;
const int BASIC_FAME = 0;
const int BASIC_FATIGUE = 0;
const int MIN_STRENGTH = 1, MAX_STRENGTH = 100;
const int MIN_CONSTITUTION = 1, MAX_CONSTITUTION = 100;
const int MIN_DEXTERITY = 1, MAX_DEXTERITY = 100;
const int MIN_INTELLIGENCE = 1, MAX_INTELLIGENCE = 100;
const int MIN_WISDOM = 1, MAX_WISDOM = 100;
const int MIN_CHARISMA = 1, MAX_CHARISMA = 100;

// Weapon's statistics
const int BASIC_WEAPON_DAMAGE = 20;
const int WEAPON_RAND_DMG_ADDTN = 6;
const WeaponType BASIC_WEAPON_TYPE = WeaponType::SWORD;

// Armour's statistics
const ArmourType BASIC_ARMOUR_TYPE = ArmourType::LIGHT;
