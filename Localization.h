#pragma once

class Localization
{
public:
	Language language;
	array<string, Localized::MESSAGE_NUMBER> messages;

	Localization();

	const string& operator[](int) const;

	void setLanguage(Language = ENGLISH);
};
