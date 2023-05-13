#pragma once

struct Localization
{
	Language language;
	array<string, Localized::MESSAGE_NUMBER> messages;

	Localization();

	void setLanguage(Language = ENGLISH);
};
