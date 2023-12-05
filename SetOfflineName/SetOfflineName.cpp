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
	mode = SetOfflineName::SelectionMode::Off;
}


SetOfflineName::~SetOfflineName()
{
}


void SetOfflineName::ParseLine(const ArgScript::Line& line)
{
	// This method is called when your cheat is invoked.
	// Put your cheat code here.

	

	auto nameoption = line.GetOption("n", 0);
	auto authoroption = line.GetOption("a",0);

	if (nameoption || authoroption) {
		if (nameoption && authoroption) {
			App::ConsolePrintF("Please specify an argument between name (-n) and author (-a).");
			return;
		}
		else if (nameoption) {
			mode = SetOfflineName::SelectionMode::Name;
		}
		else if (authoroption) {
			mode = SetOfflineName::SelectionMode::Author;
		}
		Sporepedia::ShopperRequest request(this);
		Sporepedia::ShopperRequest::Show(request);

	}
	else {
		auto args = line.GetArguments(1);
		if (args[0] == "") {
			App::ConsolePrintF("Please input a name.");
		}
		else {
			username.assign_convert(args[0]);
			if (!SaveData(username)) {
				MessageBoxW(NULL, L"SetOfflineName::SaveData has failed!\n\nSaving data to property file has failed. If you get this error message, please contact Liskomato on GitHub or Discord and tell them what you were doing at the time.", L"SetOfflineName", MB_ICONERROR | MB_OK);
			}
			else {
				App::ConsolePrintF("Offline name set to %ls", username);
			}
		}
	}
}

void SetOfflineName::OnShopperAccept(const ResourceKey& selection) {
	cAssetMetadataPtr metadata;
	if (Pollinator::GetMetadata(selection.instanceID, selection.groupID, metadata)) {
		if (mode == SelectionMode::Name) {
			username = metadata->GetName();
		 }
		else if (mode == SelectionMode::Author) {
			username = metadata->GetAuthor();
		}
		else {
			App::ConsolePrintF("Error: Mode has not been set correctly. Offline name is not changed.");
			return;
		}
		if (!SaveData(username)) {
			MessageBoxW(NULL, L"SetOfflineName::SaveData has failed!\n\nSaving data to property file has failed. If you get this error message, please contact Liskomato on GitHub or Discord and tell them what you were doing at the time.", L"SetOfflineName", MB_ICONERROR | MB_OK);
		}
		else {
			App::ConsolePrintF("Offline name set to %ls", username);
		}
	}
	else {
		App::ConsolePrintF("Error: Metadata not found. Offline name has not been changed.");
	}

	mode = SelectionMode::Off;
}

const char* SetOfflineName::GetDescription(ArgScript::DescriptionMode mode) const
{
	if (mode == ArgScript::DescriptionMode::Basic) {
		return "Sets author name for new creations.";
	}
	else {
		return "SetOfflineName: Sets your offline username in new Spore creations.\n"
			   "Usage:\n"
			   "SetOfflineName <name>: Sets your offline username to the string typed in.\n"
			   "SetOfflineName -a: Select a Spore creation in the Sporepedia and its author will become your offline username.\n"
			   "SetOfflineName -n: Select a Spore creation in the Sporepedia and its name will become your offline username.";
	}
}

string16 SetOfflineName::GetNameFromData() 
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
		WCHAR username[1024];
		DWORD username_len = 1024;
		GetUserNameW(username,&username_len);
		string16 name;
		name.assign_convert(username);
		return name;
	}
}
string16 SetOfflineName::GetFilepath()
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
bool SetOfflineName::SaveData(string16 name) 
{
	PropertyListPtr propList = new App::PropertyList();
	FileStreamPtr stream = new IO::FileStream(GetFilepath().c_str());
	propList->SetProperty(id("offlineName"), &App::Property().SetValueString16(name));
	stream->Open(IO::AccessFlags::ReadWrite, IO::CD::CreateAlways);
	bool check = propList->Write(stream.get());
	stream->Close();
	return check;
}