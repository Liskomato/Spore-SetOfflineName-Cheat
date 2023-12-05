#pragma once

#include <Spore\BasicIncludes.h>

class SetOfflineName 
	: public ArgScript::ICommand
	, public Sporepedia::IShopperListener
{
public:
	SetOfflineName();
	~SetOfflineName();

	enum class SelectionMode : uint32_t {
		Off = 0,
		Name = 1,
		Author = 2
	};

	string16 username;
	SelectionMode mode;

	// Called when the cheat is invoked
	void ParseLine(const ArgScript::Line& line) override;
	
	// Returns a string containing the description. If mode != DescriptionMode::Basic, return a more elaborated description
	const char* GetDescription(ArgScript::DescriptionMode mode) const override;

	void OnShopperAccept(const ResourceKey& selection) override;

	

	string16 GetNameFromData();
	string16 GetFilepath();
	bool SaveData(string16 name);
};

