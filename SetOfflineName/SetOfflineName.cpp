#include "stdafx.h"
#include "stdio.h"
#include "shlobj.h"
#include <string>
#include "SetOfflineName.h"

SetOfflineName::SetOfflineName()
{
	username = GetNameFromData();
	if (!SaveData(username)) {
		MessageBoxW(NULL, L"SetOfflineName::SaveData has failed!\n\nSaving data to property file has failed. If you get this error message, please contact Liskomato on GitHub or Discord and tell them what you were doing at the time.", L"SetOfflineName", MB_ICONERROR | MB_OK);
	}
	else {
		App::ConsolePrintF("Offline name set to %ls", username);
	}
}


SetOfflineName::~SetOfflineName()
{
}


void SetOfflineName::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	const char8_t* name = line.GetArgumentAt(0);
	if (name == "") {
		App::ConsolePrintF("Please input a name.");
	}
	else {
		username = (string16)name;
		if (!SaveData(username)) {
			MessageBoxW(NULL, L"SetOfflineName::SaveData has failed!\n\nSaving data to property file has failed. If you get this error message, please contact Liskomato on GitHub or Discord and tell them what you were doing at the time.", L"SetOfflineName", MB_ICONERROR | MB_OK);
		}
		else {
			App::ConsolePrintF("Offline name set to %ls",username);
		}
	}
}

const char* SetOfflineName::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Sets author name for new creations.";
	}
	else {
		return "SetOfflineName: Set your author name in new Spore creations.";
	}
}

string16 GetNameFromData() 
{
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilepath().c_str());
	if (stream->Open(IO::AccessFlags::Read, IO::CD::OpenExisting)) {
		propList->Read(stream.get());
		stream->Close();
	}
	if (propList->HasProperty(id("offlineName"))) {
		string16 name;
		App::Property::GetString16(propList.get(),id("offlineName"),name);
		return name;
	}
	else {
		char8_t username[1024];
		DWORD username_len = 1024;
		GetUserNameA(username,&username_len);
		return (string16)username;
	}
}
string16 GetFilepath()
{
	// Get %APPDATA%
	PWSTR appdata;
	/*	size_t len;
		errno_t err = getenv_s(&appdata, &len, u"APPDATA");*/

	if (SHGetKnownFolderPath(FOLDERID_RoamingAppData, KF_FLAG_CREATE, NULL, &appdata) != S_OK) {
		// This shouldn't happen.
		return NULL;
	}


	// Add rest of the filepath.

	string16 path = (char16_t*)appdata;

	path += u"\\Spore\\Preferences\\OfflineName.prop";

	return path;
}
bool SaveData(string16 name) 
{
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilepath().c_str());
	propList->SetProperty(id("offlineName"), &App::Property().SetValueString16(name));
	stream->Open(IO::AccessFlags::ReadWrite, IO::CD::CreateAlways);
	bool check = propList->Write(stream.get());
	stream->Close();
	return check;
}