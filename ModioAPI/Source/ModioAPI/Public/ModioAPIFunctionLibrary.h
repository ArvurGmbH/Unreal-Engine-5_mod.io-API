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
	static FHttpRequestRef CreateRequestForEditMod(FModioAPI_EditMod EditMod, bool& Success, FString& Message);
	static TArray64<uint8> StringToByteArray(const FString& InString);
	static FString CreateBoundaryForBoolean(FString Key, bool Value, FString BoundaryLabel);
	static FString CreateBoundaryForString(FString Key, FString Value, FString BoundaryLabel);
	static FString CreateBoundaryForInteger(FString Key, int32 Value, FString BoundaryLabel);
	static FString CreateBoundaryForStringArray(FString Key, TArray<FString> Value, FString BoundaryLabel);
	static TArray64<uint8> CreateBoundaryForImagePNG(FString Key, FString ImageFilePath, FString BoundaryLabel);
	static TArray<uint8> CreateBoundaryForZipArchive(FString Key, FString ArchiveName, TArray<uint8> ZipArchiveData, FString BoundaryLabel);
	static FModioAPI_GetUserEvents ConvertJsonObjectToGetUserEvents(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetModfiles ConvertJsonObjectToGetModfiles(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetMultipartUploadParts ConvertJsonObjectToGetMultipartUploadParts(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetMultipartUploadSessions ConvertJsonObjectToGetMultipartUploadSessions(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetMutedUsers ConvertJsonObjectToGetMutedUsers(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetUserRatings ConvertJsonObjectToGetUserRatings(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_Wallet ConvertJsonObjectToWallet(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_Modfile ConvertJsonObjectToModfile(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FHttpRequestRef CreateRequestForAddModfile(FModioAPI_AddModfile AddModfile, bool& Success, FString& Message);
	static FModioAPI_MultipartUpload ConvertJsonObjectToMultipartUpload(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_MultipartUploadPart ConvertJsonObjectToMultipartUploadPart(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetGameTagOptions ConvertJsonObjectToGetGameTagOptions(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetModTags ConvertJsonObjectToGetModTags(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);

	static FModioAPI_ModfilePlatform ConvertModfilePlatformObjectToModfilePlatform(FModioAPI_ModfilePlatform_Object ModfilePlatformObject);
	static FModioAPI_Download ConvertDownloadObjectToDownload(FModioAPI_Download_Object DownloadObject);
	static FModioAPI_Modfile ConvertModfileObjectToModfile(FModioAPI_Modfile_Object ModfileObject);
	static FModioAPI_Mod ConvertModObjectToMod(FModioAPI_Mod_Object ModObject);
	static FModioAPI_User ConvertUserObjectToUser(FModioAPI_User_Object UserObject);
	static FModioAPI_GetMods ConvertGetModsSchemaToGetMods(FModioAPI_GetMods_Schema GetModsSchema);
	static FModioAPI_UserEvent ConvertUserEventObjectToUserEvent(FModioAPI_UserEvent_Object UserEventObject);
	static FModioAPI_GetUserEvents ConvertGetUserEventsSchemaToGetUserEvents(FModioAPI_GetUserEvents_Schema GetUserEventsSchema);
	static FModioAPI_GetModfiles ConvertGetModfilesSchemaToGetModfiles(FModioAPI_GetModfiles_Schema GetUserModfilesSchema);
	static FModioAPI_MultipartUploadPart ConvertMultipartUploadPartObjectToMultipartUploadPart(FModioAPI_MultipartUploadPart_Object MultipartUploadPartObject);
	static FModioAPI_GetMultipartUploadParts ConvertGetMultipartUploadPartsSchemaToGetMultipartUploadParts(FModioAPI_GetMultipartUploadParts_Schema GetMultipartUploadPartsSchema);
	static FModioAPI_GetMultipartUploadSessions ConvertGetMultipartUploadSessionsSchemaToGetMultipartUploadSessions(FModioAPI_GetMultipartUploadSessions_Schema GetMultipartUploadSessionsSchema);
	static FModioAPI_Game ConvertGameObjectToGame(FModioAPI_Game_Object GameObject);
	static FModioAPI_GetGames ConvertGetGamesSchemaToGetGames(FModioAPI_GetGames_Schema GetGamesSchema);
	static FModioAPI_GetGames ConvertJsonObjectToGetGames(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message);
	static FModioAPI_GetMutedUsers ConvertGetMutedUsersSchemaToGetMutedUsers(FModioAPI_GetMutedUsers_Schema GetMutedUsersSchema);
	static FModioAPI_Rating ConvertRatingObjectToRating(FModioAPI_Rating_Object RatingObject);
	static FModioAPI_GetUserRatings ConvertGetUserRatingsSchemaToGetUserRatings(FModioAPI_GetUserRatings_Schema GetUserRatingsSchema);
	static FModioAPI_Wallet ConvertWalletObjectToWallet(FModioAPI_Wallet_Object WalletObject);
	static FModioAPI_MultipartUpload ConvertMultipartUploadObjectToMultipartUpload(FModioAPI_MultipartUpload_Object MultipartUploadObject);
	static int32 ExtractModIDFromRequestURL(FString RequestURL, FString EndpointMods);
	static FModioAPI_ModStats ConvertModStatsObjectToModStats(FModioAPI_ModStats_Object ModstatsObject);
	static FModioAPI_ModTag ConvertModTagObjectToModTag(FModioAPI_ModTag_Object ModTagObject);
	static TArray<FModioAPI_ModTag> ConvertModTagObjectsToModTags(TArray<FModioAPI_ModTag_Object> ModTagObjects);

	static FString ConvertPlatformToPlatformCode(TEnumAsByte<EModioAPI_Platforms> Platform);
	static TSharedPtr<FJsonObject> ConvertResponseToJsonObject(FHttpResponsePtr Response);

	UFUNCTION(BlueprintPure, Category="mod.io API|Filtering")
	static FModioAPI_RequestFilters ApplyFilter(FModioAPI_RequestFilters FiltersIn, FString ColumnForFiltering, TEnumAsByte<EModioAPI_FilterComparison> Comparison, FString Value);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Utilities")
	static FString SanitizeStringForHTTP_Request(FString String);

	static FString ConvertIntToZipFilePartNumber(int32 PartNumber);

	static FString FindUploadPartFileByPartNumber(TArray<FString> UploadPartFiles, int32 PartNumber);
	static int64 GetContentRangeStartForUploadPartNumber(int32 PartNumber);
	static int64 GetContentRangeEndForUploadSessionOnPartNumber(TArray<FString> UploadPartFiles, int32 PartNumber);

	// Cache Filters

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static bool ModIncludesTag(FModioAPI_Mod Mod, FString Tag);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static bool ModIncludesPlatform(FModioAPI_Mod Mod, TEnumAsByte<EModioAPI_Platforms> Platform);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static bool ModHasMetadataKVP(FModioAPI_Mod Mod, FString Key, FString Value);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByName(TArray<FModioAPI_Mod> Mods, FString Name, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsBySubmitter(TArray<FModioAPI_Mod> Mods, FString SubmittedBy, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByVisibility(TArray<FModioAPI_Mod> Mods, TEnumAsByte<EModioAPI_ModVisibility> Visibility, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByDateAdded(TArray<FModioAPI_Mod> Mods, FDateTime DateTime, TEnumAsByte<EModioAPI_CacheTimeFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByDateLive(TArray<FModioAPI_Mod> Mods, FDateTime DateTime, TEnumAsByte<EModioAPI_CacheTimeFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByDateUpdated(TArray<FModioAPI_Mod> Mods, FDateTime DateTime, TEnumAsByte<EModioAPI_CacheTimeFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByTag(TArray<FModioAPI_Mod> Mods, FString Tag, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByPlatform(TArray<FModioAPI_Mod> Mods, TEnumAsByte<EModioAPI_Platforms> Platform, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode);

	UFUNCTION(BlueprintPure, Category = "mod.io API|Filtering")
	static TArray<FModioAPI_Mod> FilterModsByMetadataKVP(TArray<FModioAPI_Mod> Mods, FString Key, FString Value, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode);

	// Cache Sorting

	UFUNCTION(BlueprintPure, Category = "mod.io API|Sorting")
	static TArray<FModioAPI_Mod> SortModsByID(TArray<FModioAPI_Mod> Mods, TEnumAsByte<EModioAPI_SortingOrder> Order);
};
