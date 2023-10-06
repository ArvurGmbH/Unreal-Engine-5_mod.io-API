/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ModioAPIStructs.h"
#include "JsonObjectConverter.h"
#include "Http.h"
#include "Containers/UnrealString.h"
#include "ImageUtils.h"
#include "ModioAPIFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MODIOAPI_API UModioAPIFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static FModioAPI_Error_Object ConvertJsonObjectToError(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_Message_Object ConvertJsonObjectToMessage(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_Terms ConvertJsonObjectToTermsOfService(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_AccessToken_Response ConvertJsonObjectToAccessToken(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_User ConvertJsonObjectToUser(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_Game ConvertJsonObjectToGame(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetMods ConvertJsonObjectToGetMods(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_Mod ConvertJsonObjectToMod(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FHttpRequestRef CreateRequestForAddMod(FModioAPI_AddMod AddMod, bool& Success, FString& Message);
	static TArray64<uint8> StringToByteArray(const FString& InString);
	static FString CreateBoundaryForString(FString Key, FString Value, FString BoundaryLabel);
	static FString CreateBoundaryForInteger(FString Key, int32 Value, FString BoundaryLabel);
	static FString CreateBoundaryForStringArray(FString Key, TArray<FString> Value, FString BoundaryLabel);
	static TArray64<uint8> CreateBoundaryForImagePNG(FString Key, FString ImageFilePath, FString BoundaryLabel);
	static FModioAPI_GetUserEvents ConvertJsonObjectToGetUserEvents(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);

	static FModioAPI_ModfilePlatform ConvertModfilePlatformObjectToModfilePlatform(FModioAPI_ModfilePlatform_Object ModfilePlatformObject);
	static FModioAPI_Download ConvertDownloadObjectToDownload(FModioAPI_Download_Object DownloadObject);
	static FModioAPI_Modfile ConvertModfileObjectToModfile(FModioAPI_Modfile_Object ModfileObject);
	static FModioAPI_Mod ConvertModObjectToMod(FModioAPI_Mod_Object ModObject);
	static FModioAPI_GetMods ConvertGetModsSchemaToGetMods(FModioAPI_GetMods_Schema GetModsSchema);
	static FModioAPI_UserEvent ConvertUserEventObjectToUserEvent(FModioAPI_UserEvent_Object UserEventObject);
	static FModioAPI_GetUserEvents ConvertGetUserEventsSchemaToGetUserEvents(FModioAPI_GetUserEvents_Schema GetUserEventsSchema);

	UFUNCTION(BlueprintPure, Category="mod.io API|Filtering")
	static FModioAPI_RequestFilters ApplyFilter(FModioAPI_RequestFilters FiltersIn, FString ColumnForFiltering, TEnumAsByte<EModioAPI_FilterComparison> Comparison, FString Value);
};
