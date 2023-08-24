#pragma once

#include <Spore\BasicIncludes.h>

class SetOfflineName 
	: public ArgScript::ICommand
{
public:
	SetOfflineName();
	~SetOfflineName();

	string16 username;

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	string16 GetNameFromData();
	string16 GetFilepath();
	bool SaveData(string16 name);
};

