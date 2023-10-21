#pragma once

class Localization
{
public:
	Language language;
	array<string, Localized::MESSAGE_NUMBER> messages;

	Localization();

	void setLanguage(Language = ENGLISH);
};
