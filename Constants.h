#pragma once

// World's statistics
const int MAX_CITIES = 11;
const int OPPONENTS_NUMBER = 15;
const int BASIC_REGEN = 30;
const int BASIC_ATTRIBUTES = 10;
//const string OUTPUT_DIVIDER = "\n______________________________________________\n\n\n";

// Player's and NPC's statistics
const int MIN_ATTRIBUTE = 1, MAX_ATTRIBUTE = 100;
const int MIN_STRENGTH = 1, MAX_STRENGTH = 100;
const int MIN_CONSTITUTION = 1, MAX_CONSTITUTION = 100;
const int MIN_DEXTERITY = 1, MAX_DEXTERITY = 100;
const int MIN_INTELLIGENCE = 1, MAX_INTELLIGENCE = 100;
const int MIN_WISDOM = 1, MAX_WISDOM = 100;
const int MIN_CHARISMA = 1, MAX_CHARISMA = 100;
const int MIN_AGE = 20, MAX_AGE = 60;
const int MIN_FAME = 0, BASIC_FAME = 100;
// Fighter's statistics
const int BASIC_HP = 100;
// Leveling statistics
const int MIN_LEVEL = 1;
const int MAX_LEVEL = 100;
const int EXPERIENCE_PER_LEVEL = 200;
const int EXPERIENCE_PER_LEVEL_INCREASE_PERC = 5; // 5.0% more experience needed for every next level from 1
const int ATTRIBUTES_PER_LEVEL = 5;
const double MIN_EXPERIENCE_MULTIPLIER = 0.1;
const double MAX_EXPERIENCE_MULTIPLIER = 2.5;

// Weapon's statistics
const int MIN_WEAPON_DAMAGE = 20;
const int WEAPON_RAND_DAM_ADDITION = 6;
const Weapon::Type BASIC_WEAPON_TYPE = Weapon::SWORD;
const int MIN_SHIELD_PROB_ADDITION = 10;
const int SHIELD_RAND_PROB_ADDITION = 6;
const int MIN_SHIELD_DEF_PERC_ADDITION = 40;
const int SHIELD_RAND_DEF_PERC_ADDITION = 45;

// Armour's statistics
const Armour::Type BASIC_ARMOUR_TYPE = Armour::LIGHT;
/*
 * Maximum values of the parameters are assumed.
 *
 * HP - (Damage - Defense).
 * HP == Basic HP + Constitution + Strength:
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
 * HP: 140 == 100 + 30 + 10.
 * Damage: 64 == 10 + 10 + (20 + 1 + 1) * 2.
 * Armour: 31 == 30 + 0 + 1.
 * ~Number of hits for one == 7 == 140 / (33 * (1000 - 375) / 1000).
 * Fight == ~(7 * 2) hits.
 */
const int MIN_ARMOUR_DEFENSE = 30, MAX_ARMOUR_DEFENSE = 50;
const int ARMOUR_RAND_DEF_ADDITION = 6;

// Fight's statistics
const int ONE_HUNDRED_PERCENT = 1000; // 100.0%
const int DAMAGE_REDUCTION_PERCENT = 375; // 37.5%
const int WEAPON_MAX_RAND_DMG_SPREAD_PERCENT = 10; // 10.0%

// Game settings
const int SLEEP_TIME = 1000; // Delay between attacks in fight
const int STARTING_ADDITIONAL_ATTRIBUTES = 30;
const int ATTRIBUTE_MAX_DIFFERENCE = 10; // How much a player can increase attributes relative to average attributes level

const COLORREF COLOR_WHITE = RGB(255, 255, 255),
			   COLOR_ROMAN_RED = RGB(80, 0, 0),
			   COLOR_ROMAN_RED_PUSHED = RGB(50, 0, 0);

const int ROME_CITY_LEVEL = 80,
		  NAPLES_CITY_LEVEL = 70,
		  METAPONTO_CITY_LEVEL = 20,
		  BOJANO_CITY_LEVEL = 30,
		  ANCONA_CITY_LEVEL = 20,
		  PERUGIA_CITY_LEVEL = 10,
		  FLORENCE_CITY_LEVEL = 40,
		  BOLOGNA_CITY_LEVEL = 30,
		  GENOA_CITY_LEVEL = 40,
		  VENICE_CITY_LEVEL = 50,
		  MILAN_CITY_LEVEL = 60;
