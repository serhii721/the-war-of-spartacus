#pragma once

class Localization
{
private:
	Language language;
	array<string, Localized::MESSAGE_NUMBER> messages;
	vector<string> npcFirstNames;
	vector<string> npcLastNames;
	vector<string> gladiatorNicknames;
	vector<string> weaponTypes;
	vector<string> armourTypes;
	vector<string> cityNames;

	// TODO: errorMessages
public:
	Localization();
	Localization(const Localization&);
	Localization& operator=(const Localization&);
	~Localization();

	void setLanguage(Language = ENGLISH);

	const string& operator[](int) const;
	Language getLanguage() const;
	string getFirstName(int);
	string getLastName(int);
	string getNickname(int);
	string getWeaponType(int);
	string getArmourType(int);
	string getCityName(int);
	// TODO: getErrorMessage()
};
