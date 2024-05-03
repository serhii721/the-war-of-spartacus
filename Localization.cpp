#include "stdafx.h"
#include "Localization.h"

Localization::Localization() :
	language(Language::ENGLISH),
	messages(),
	npcFirstNames(),
	npcLastNames(),
	gladiatorNicknames(),
	weaponTypes(),
	armourTypes(),
	cityNames()
{ }

Localization::Localization(const Localization& L) :
	language(L.language),
	messages(L.messages),
	npcFirstNames(L.npcFirstNames),
	npcLastNames(L.npcLastNames),
	gladiatorNicknames(L.gladiatorNicknames),
	weaponTypes(L.weaponTypes),
	armourTypes(L.armourTypes),
	cityNames(L.cityNames)
{ }

Localization& Localization::operator=(const Localization& L)
{
	if (&L == this) return *this;
	language = L.language;
	messages = L.messages;
	npcFirstNames = L.npcFirstNames;
	npcLastNames = L.npcLastNames;
	gladiatorNicknames = L.gladiatorNicknames;
	weaponTypes = L.weaponTypes;
	armourTypes = L.armourTypes;
	cityNames = L.cityNames;
	return *this;
}

Localization::~Localization() { }

const string& Localization::operator[](int i) const { return messages[i]; }

void Localization::setLanguage(Language llanguage)
{
	// Determinating the language file
	string fileName = "", langPrefix = "";
	switch (llanguage)
	{
	default: case Language::ENGLISH:
		fileName = "English";
		langPrefix = "En_";
		break;
	case Language::UKRAINIAN:
		fileName = "Ukrainian";
		langPrefix = "Uk_";
		break;
	case Language::RUSSIAN:
		fileName = "Russian";
		langPrefix = "Ru_";
		break;
	case Language::LATIN:
		fileName = "Latin";
		langPrefix = "En_";
		break;
	}
	// Opening files
	string path = "Data/Language/";
	const string FORMAT = ".lang";
	ifstream fin(path + fileName + FORMAT);
	ifstream finEng(path + "English" + FORMAT);

	// Reading the file and filling the localization array
	string line, lineEng;
	int lineIndex, i = 0;
	for (; i < Localized::MESSAGE_NUMBER; i++)
	{
		getline(fin, line);
		if (!line.empty())
			messages[i] = line;
		else
		{
			lineIndex = 0;
			while (getline(finEng, line))
			{
				if (lineIndex == i)
				{
					messages[i] = line;
					finEng.clear();
					finEng.seekg(0);
					break;
				}
				lineIndex++;
			}
		}
	}

	fin.close();
	finEng.close();

	// Reading the file of NPCs` first names
	path += "Names/";
	fileName = "FirstNames";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			npcFirstNames.push_back(line);
		else
			npcFirstNames.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of NPCs` last names
	fileName = "LastNames";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			npcLastNames.push_back(line);
		else
			npcLastNames.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of weapon types
	path = "Data/Language/World/";
	fileName = "WeaponTypes";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			weaponTypes.push_back(line);
		else
			weaponTypes.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of armour types
	fileName = "ArmourTypes";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			armourTypes.push_back(line);
		else
			armourTypes.push_back(lineEng);
	}

	fin.close();
	finEng.close();

	// Reading the file of City names
	fileName = "CityNames";
	fin.open(path + langPrefix + fileName + FORMAT);
	finEng.open(path + "En_" + fileName + FORMAT);

	while (getline(fin, line))
	{
		getline(finEng, lineEng);
		if (line != "")
			cityNames.push_back(line);
		else
			cityNames.push_back(lineEng);
	}

	fin.close();
	finEng.close();
}

Language Localization::getLanguage() const { return language; }

string Localization::getNPCName(const NamedNPC& NPC) const
{
	return npcFirstNames[NPC.getFirstNameIndex()] + ' ' + npcLastNames[NPC.getLastNameIndex()];
}

const string& Localization::getNPCFirstName(const NamedNPC& NPC) const
{
	return npcFirstNames[NPC.getFirstNameIndex()];
}

const string& Localization::getNPCLastName(const NamedNPC& NPC) const
{
	return npcLastNames[NPC.getLastNameIndex()];
}

const string& Localization::getNPCNickname(const NamedNPC& NPC) const
{
	return npcLastNames[NPC.getLastNameIndex()]; // TODO: return gladiatorNicknames[NPC.getNicknameIndex()];
}

const string& Localization::getWeaponTypeName(const Weapon& W) const
{
	return weaponTypes[W.getType()];
}

const string& Localization::getArmourTypeName(const Armour& A) const
{
	return armourTypes[A.getType()];
}

const string& Localization::getCityName(const City& C) const
{
	return cityNames[C.getNameIndex()];
}
