/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "ModioAPI.h"

#define LOCTEXT_NAMESPACE "FModioAPIModule"

void FModioAPIModule::StartupModule()
{
	// This code will execute after your module is loaded into memory; the exact timing is specified in the .uplugin file per-module
}

void FModioAPIModule::ShutdownModule()
{
	// This function may be called during shutdown to clean up your module.  For modules that support dynamic reloading,
	// we call this function before unloading the module.
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FModioAPIModule, ModioAPI)