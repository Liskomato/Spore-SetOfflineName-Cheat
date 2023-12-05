// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "SetOfflineName.h"

SetOfflineName* offlineName = nullptr;

void Initialize()
{
	// This method is executed when the game starts, before the user interface is shown
	// Here you can do things such as:
	//  - Add new cheats
	//  - Add new simulator classes
	//  - Add new game modes
	//  - Add new space tools
	//  - Change materials
	offlineName = new SetOfflineName();
	CheatManager.AddCheat("SetOfflineName", offlineName);
}

member_detour(SavePNG_detour, App::Thumbnail_cImportExport, bool(Resource::ResourceObject*, RenderWare::Raster*, Resource::Database*, bool, bool)) 
{
	bool detoured(Resource::ResourceObject * pResource, RenderWare::Raster * pImage, Resource::Database * database,
		bool forceReplace, bool disableSteganography) {
		cAssetMetadataPtr metadata;
		if (Pollinator::GetMetadata(pResource->GetResourceKey().instanceID,pResource->GetResourceKey().groupID,metadata) && metadata->GetAuthorID() == -2) {
			metadata->mAuthorName = offlineName->username;
			metadata->mAuthorID = -3;
			if (!ResourceManager.WriteResource(metadata.get())) {
				App::ConsolePrintF("SetOfflineName: ERROR with saving edited metadata. Check if your creation still exists.");
			}
		}

		bool ret = original_function(this, pResource, pImage, database, forceReplace, disableSteganography);

		return ret;
	}
};

void Dispose()
{
	// This method is called when the game is closing
	offlineName = nullptr;
}

void AttachDetours()
{
	// Call the attach() method on any detours you want to add
	// For example: cViewer_SetRenderType_detour::attach(GetAddress(cViewer, SetRenderType));
	SavePNG_detour::attach(GetAddress(App::Thumbnail_cImportExport,SavePNG));
}


// Generally, you don't need to touch any code here
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
		ModAPI::AddPostInitFunction(Initialize);
		ModAPI::AddDisposeFunction(Dispose);

		PrepareDetours(hModule);
		AttachDetours();
		CommitDetours();
		break;

	case DLL_PROCESS_DETACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	}
	return TRUE;
}

