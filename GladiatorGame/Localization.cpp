#include "pch.h"
#include "Localization.h"

Localization::Localization() : language(Language::ENGLISH), messages() { }

void Localization::setLanguage(Language llanguage)
{
	// Determinating the language file
	string fileName = "";
	switch (llanguage)
	{
	default: case Language::ENGLISH:
		fileName = "English";
		break;
	case Language::UKRAINIAN:
		fileName = "Ukrainian";
		break;
	case Language::RUSSIAN:
		fileName = "Russian";
		break;
	case Language::LATIN:
		fileName = "Latin";
		break;
	}
	// Opening files
	const string PATH = "Data/Language/", FORMAT = ".lang";
	ifstream fin(PATH + fileName + FORMAT);
	ifstream finEng(PATH + "English" + FORMAT);

	// Reading the file and filling the localization array
	string line;
	int lineIndex;
	for (int i = 0; i < Localized::MESSAGE_NUMBER; i++)
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
}
