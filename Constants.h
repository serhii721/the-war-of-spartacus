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
const int MIN_WEAPON_DAMAGE = 20;
const int WEAPON_RAND_DMG_ADDTN = 6;
const WeaponType BASIC_WEAPON_TYPE = WeaponType::SWORD;
const int MIN_SHIELD_PROB_ADDTN = 10;
const int SHIELD_RAND_PROB_ADDTN = 6;
const int MIN_SHIELD_DEF_PERCENT_ADDTN = 20;
const int SHIELD_RAND_DEF_PERCENT_ADDTN = 6;

// Armour's statistics
const ArmourType BASIC_ARMOUR_TYPE = ArmourType::LIGHT;
/*
 * Maximum values of the parameters are assumed.
 *
 * Health - (Damage - Defense).
 * Health == Basic health + Constitution + Strength:
 * 500 == 100 + 300 + 100.
 *
 * Weapon damage == Basic weapon damage + Strength scale (50~10) + Dexterity scale (50~10).
 * Damage == Strength + Dexterity + Weapon damage:
 * 450 == 100 + 100 + (25 + 50 + 50) * 2.
 *
 * Armour defense compensates weapon damage.
 * Armour defense == Basic armour defense + Strength scale (25~5) + Dexterity scale (75~15).
 * 250 == 50 + 50 + 150.
 *
 * ~Number of hits for one == 4 == 500 / (200 * (1000 - 375) / 1000).
 * [* (1000 - 375) / 1000] is 37.5% damage reduction to prolong a fight.
 * Fight == ~(4 * 2) hits.
 *
 * Gladitor values.
 * Damage: 72 == 10 + 10 + (20 + 3 + 3) * 2.
 * Armour: 40 == 35 + 1 + 4.
 * ~Number of hits for one == 7 == 140 / (32 * (1000 - 375) / 1000).
 *
 * Minimum values.
 * Health: 140 == 100 + 30 + 10.
 * Damage: 64 == 10 + 10 + (20 + 1 + 1) * 2.
 * Armour: 31 == 30 + 0 + 1.
 * ~Number of hits for one == 7 == 140 / (33 * (1000 - 375) / 1000).
 * Fight == ~(7 * 2) hits.
 */
const int MIN_ARMOUR_DEFENSE = 30, MAX_ARMOUR_DEFENSE = 50;
const int ARMOUR_RAND_DEF_ADDITION = 6;

// Fight's statistics
const int PERCENT_DAMAGE_REDUCTION = 375; // 37.5%.
