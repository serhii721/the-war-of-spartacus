#pragma once

class Localization
{
private:
	Language language;
	array<string, Localized::MESSAGE_NUMBER> messages;
	vector<string> npcFirstNames;
	vector<string> npcLastNames;
	vector<string> gladiatorNicknames;

	// TODO: errorMessages
public:
	Localization();
	Localization(const Localization&);
	Localization& operator=(const Localization&);
	~Localization();

	void setLanguage(Language = ENGLISH);

	const string& operator[](int) const;
	Language getLanguage() const;
	// TODO: getErrorMessage()
};
