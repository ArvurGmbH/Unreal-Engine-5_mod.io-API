/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "ModioAPI.h"
#include "CoreMinimal.h"
#include "ModioAPIEnums.h"
#include "Engine/DataTable.h"
#include "Misc/DateTime.h"
#include "ModioAPIResponseSchemas.h"
#include "ModioAPIStructs.generated.h"

/*
Response Schemas - For Unreal Engine use
e.g.
- int64 Unix Timestamps converted to Unreal Engine FDateTime
- int32 Type/Status converted to easy-to-use Enums
- int32 Bitwise Fields converted to Bool-Arrays/-Structs
- etc.
*/

// Access Token

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Access Token"))
struct FModioAPI_AccessToken
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Access Token")
    FString AccessToken;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Access Token")
    FDateTime DateExpires;
};

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Access Token Response"))
struct FModioAPI_AccessToken_Response
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Access Token")
    int32 Code;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Access Token")
    FModioAPI_AccessToken AccessToken;
};

// Agreement Version

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Agreement Version"))
struct FModioAPI_AgreementVersion
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Agreement Version")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Agreement Version")
    bool IsActive;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Agreement Version")
    TEnumAsByte<EModioAPI_AgreementType> Type;
};

// User

USTRUCT(BlueprintType, Category = "mod.io API|Users", meta = (DisplayName = "User"))
struct FModioAPI_User
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Name_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Username;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Display_Name_Portal;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FDateTime Date_Online;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FDateTime Date_Joined;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FModioAPI_Avatar_Object Avatar;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Timezone;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Language;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Profile_URL;
};

// Comment

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Comment"))
struct FModioAPI_Comment
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    int32 Game_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    int32 Resource_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    FModioAPI_User User;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    int32 Reply_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    FString Thread_Position;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    int32 Karma;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Comment")
    FString Content;
};

// Download

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Download"))
struct FModioAPI_Download
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Download")
    FString Binary_URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Download")
    FDateTime Date_Expires;
};

// Options - Bitwise Fields

USTRUCT(BlueprintType, Category = "mod.io API", meta = (DisplayName = "mod.io Maturity Options"))
struct FModioAPI_MaturityOptions
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API")
    bool None;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API")
    bool Alcohol;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API")
    bool Drugs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API")
    bool Violence;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API")
    bool Explicit;
};

USTRUCT(BlueprintType, Category = "mod.io API", meta = (DisplayName = "mod.io Community Options", Tooltip = "Community Options"))
struct FModioAPI_CommunityOptions
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "All Options are disabled"))
    bool AllOptionsDisabled;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Enable Comments"))
    bool EnableComments;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Enable Guides"))
    bool EnableGuides;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Pin on Homepage"))
    bool PinOnHomepage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Show on Homepage"))
    bool ShowOnHomepage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Show more on Homepage"))
    bool ShowMoreOnHomepage;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Allow change Status"))
    bool AllowChangeStatus;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Enable Previews", Tooltip = "Game must be hidden"))
    bool EnablePreviews;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Allow Preview Share-URL", Tooltip = "Previews must be enabled"))
    bool AllowPreviewShareURL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Allow negative Ratings"))
    bool AllowNegativeRatings;
};

USTRUCT(BlueprintType, Category = "mod.io API", meta = (DisplayName = "mod.io Game Monetization Option", Tooltip = "Community Options"))
struct FModioAPI_GameMonetizationOption
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Disabled"))
    bool Disabled;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Enabled"))
    bool Enabled;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API", meta = (DisplayName = "Enable Marketplace"))
    bool EnableMarketplace;
};


// Game Stats

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game Stats"))
struct FModioAPI_GameStats
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    int32 Game_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    int32 Mods_Count_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    int32 Mods_Downloads_Today;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    int32 Mods_Downloads_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    int32 Mods_Downloads_Daily_Average;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    int32 Mods_Subscribers_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Stats")
    FDateTime Date_Expires;
};

// Game User Preview

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game User Preview"))
struct FModioAPI_GameUserPreview
{
    GENERATED_BODY()

    UPROPERTY()
    FModioAPI_User User;

    UPROPERTY()
    FString Resource_URL;

    UPROPERTY()
    FDateTime Date_Added;
};

// Game

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game"))
struct FModioAPI_Game
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TEnumAsByte<EModioAPI_GameGuideModStatus> Status;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FDateTime Date_Updated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FDateTime Date_Live;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TEnumAsByte<EModioAPI_GamePresentation> Presentation_Option;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TEnumAsByte<EModioAPI_GameModsSubmission> Submission_Option;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TEnumAsByte<EModioAPI_GameModsCuration> Curation_Option;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_CommunityOptions Community_Options;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_GameMonetizationOption Monetization_Option;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_GameMonetizationTeam_Object Monetization_Team;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TEnumAsByte<EModioAPI_API_AccessOption> API_Access_Option;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TEnumAsByte<EModioAPI_MaturityOption> Maturity_Option;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString UGC_Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Token_Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_Icon_Object Icon;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_Logo_Object Logo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_HeaderImage_Object Header;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Name_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Summary;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Instructions;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Instructions_URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Profile_URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_GameStats Stats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FModioAPI_Theme Theme;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TArray<FModioAPI_GameOtherURLs_Object> Other_URLs;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TArray<FModioAPI_GameTagOption_Object> Tag_Options;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    TArray<FModioAPI_GamePlatforms_Object> Platforms;
};

// Get Games

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Games"))
struct FModioAPI_GetGames
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Games")
    TArray<FModioAPI_Game> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Games")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Games")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Games")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Games")
    int32 Result_Total;
};

// Get Guide Comments

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Guide Comments"))
struct FModioAPI_GetGuideComments
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_Guide_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Guide Tag

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Guide Tag"))
struct FModioAPI_GuideTag
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide Tag")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide Tag")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide Tag")
    int32 Count;
};

// Get Guide Tags

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Guides Tags"))
struct FModioAPI_GetGuidesTags
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guide Tags")
    TArray<FModioAPI_GuideTag> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guide Tags")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guide Tags")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guide Tags")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guide Tags")
    int32 Result_Total;
};

// Get Guides

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Guides"))
struct FModioAPI_GetGuides
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guides")
    TArray<FModioAPI_GuideTag> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guides")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guides")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guides")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Guides")
    int32 Result_Total;
};

// Get Mod Comments

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mod Comments"))
struct FModioAPI_GetModComments
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Comments")
    TArray<FModioAPI_Comment> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Comments")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Comments")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Comments")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Comments")
    int32 Result_Total;
};

// Modfile Platform

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Modfile Platform"))
struct FModioAPI_ModfilePlatform
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform")
    FString Platform;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform")
    TEnumAsByte<EModioAPI_ModfilePlatformStatus> Status;
};

// Modfile

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Modfile"))
struct FModioAPI_Modfile
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FDateTime Date_Updated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FDateTime Date_Scanned;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    TEnumAsByte<EModioAPI_ModfileVirusStatus> Virus_Status;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    TEnumAsByte<EModioAPI_ModfileVirusPositivity> Virus_Positive;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files", meta = (Tooltip = "Filesize in Bytes (Compressed as Zip)"))
    int64 Filesize;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files", meta = (Tooltip = "Filesize in Bytes (Uncompressed after extracting Zip)"))
    int64 Filesize_Uncompressed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FModioAPI_Filehash_Object Filehash;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FString Filename;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FString Version;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FString Changelog;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FString Metadata_Blob;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    FModioAPI_Download Download;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Files")
    TArray<FModioAPI_ModfilePlatform> Platforms;
};

// Mod Dependencies

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod Dependencies"))
struct FModioAPI_ModDependencies
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    FString Name_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    int32 Dependency_Depth;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    FModioAPI_Logo_Object Logo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Dependencies")
    FModioAPI_Modfile Modfile;
};

// Get Mod Dependencies

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mod Dependencies"))
struct FModioAPI_GetModDependencies
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Dependencies")
    TArray<FModioAPI_ModDependencies> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Dependencies")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Dependencies")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Dependencies")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Dependencies")
    int32 Result_Total;
};

// Mod Event

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod Event"))
struct FModioAPI_ModEvent
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Event")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Event")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Event")
    int32 User_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Event")
    FDateTime DateAdded;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Event")
    TEnumAsByte<EModioAPI_ModEventType> Event_Type;
};

// Get Mod Events

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mod Events"))
struct FModioAPI_GetModEvents
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    TArray<FModioAPI_ModEvent> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Total;
};


// Mod Stats

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod Stats"))
struct FModioAPI_ModStats
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Popularity_Rank_Position;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Popularity_Rank_Total_Mods;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Downloads_Today;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Downloads_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Subscribers_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Ratings_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Ratings_Positive;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Ratings_Negative;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Ratings_Percentage_Positive;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    int32 Ratings_Percentage_Negative;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    float Ratings_Weighted_Aggregate;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    FString Ratings_Display_Text;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Stats")
    FDateTime Date_Expires;
};

// Get Mod Stats

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mod Stats"))
struct FModioAPI_GetModStats
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Stats")
    TArray<FModioAPI_ModStats> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Stats")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Stats")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Stats")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Stats")
    int32 Result_Total;
};

// Mod Tag

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod Tag"))
struct FModioAPI_ModTag
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Tag")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Tag")
    FDateTime Date_Added;
};

// Mod

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod"))
struct FModioAPI_Mod
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    int32 Game_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    TEnumAsByte<EModioAPI_GameGuideModStatus> Status;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    TEnumAsByte<EModioAPI_ModVisibility> Visible;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_User Submitted_By;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FDateTime Date_Updated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FDateTime Date_Live;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_MaturityOptions Maturity_Options;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_CommunityOptions Community_Options;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_GameMonetizationOption Monetization_Options;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    int32 Price;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    int32 Tax;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_Logo_Object Logo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Homepage_URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Name_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Summary;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Description;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Description_Plaintext;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Metadata_Blob;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FString Profile_URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_ModMedia_Object Media;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_Modfile Modfile;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    bool Dependencies;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    FModioAPI_ModStats Stats;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    TArray<FModioAPI_ModPlatforms> Platforms;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    TArray<FModioAPI_MetadataKVP_Object> Metadata_KVP;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod")
    TArray<FModioAPI_ModTag> Tags;
};

// Get Mod Tags

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mod Tags"))
struct FModioAPI_GetModTags
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Tags")
    TArray<FModioAPI_ModTag> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Tags")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Tags")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Tags")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Tags")
    int32 Result_Total;
};

// Get Modfiles

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Modfiles"))
struct FModioAPI_GetModfiles
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Modfiles")
    TArray<FModioAPI_Modfile> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Modfiles")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Modfiles")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Modfiles")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Modfiles")
    int32 Result_Total;
};

// Get Mods

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mods"))
struct FModioAPI_GetMods
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mods")
    TArray<FModioAPI_Mod> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mods")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mods")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mods")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mods")
    int32 Result_Total;
};


// Multipart Upload

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Multipart Upload"))
struct FModioAPI_MultipartUpload
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Multipart Upload")
    FString Upload_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Multipart Upload")
    TEnumAsByte<EModioAPI_MultipartUploadStatus> Status;
};

// Multipart Upload Part

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Multipart Upload Part"))
struct FModioAPI_MultipartUploadPart
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Multipart Upload Part")
    FString Upload_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Multipart Upload Part")
    int32 Part_Number;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Multipart Upload Part")
    int64 Part_Size;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Multipart Upload Part")
    FDateTime Date_Added;
};

// Get Multipart Upload Parts

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Multipart Upload Parts"))
struct FModioAPI_GetMultipartUploadParts
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Parts")
    TArray<FModioAPI_MultipartUploadPart> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Parts")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Parts")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Parts")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Parts")
    int32 Result_Total;
};

// Get Multipart Upload Sessions

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Multipart Upload Sessions"))
struct FModioAPI_GetMultipartUploadSessions
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Sessions")
    TArray<FModioAPI_MultipartUpload> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Sessions")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Sessions")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Sessions")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Multipart Upload Sessions")
    int32 Result_Total;
};

// Get Muted Users

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Muted Users"))
struct FModioAPI_GetMutedUsers
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Muted Users")
    TArray<FModioAPI_User> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Muted Users")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Muted Users")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Muted Users")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Muted Users")
    int32 Result_Total;
};


// Team Member

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Team Member"))
struct FModioAPI_TeamMember
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Team Member")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Team Member")
    FModioAPI_User User;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Team Member")
    int32 Level;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Team Member")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Team Member")
    FString Position;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Team Member")
    bool Invite_Pending;
};

// Get Team Members

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Team Members"))
struct FModioAPI_GetTeamMembers
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Team Members")
    TArray<FModioAPI_TeamMember> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Team Members")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Team Members")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Team Members")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Team Members")
    int32 Result_Total;
};

// User Event

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io User Event"))
struct FModioAPI_UserEvent
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Event")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Event")
    int32 Game_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Event")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Event")
    int32 User_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Event")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Event")
    TEnumAsByte<EModioAPI_UserEventType> Event_Type;
};

// Get User Events

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get User Events"))
struct FModioAPI_GetUserEvents
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Events")
    TArray<FModioAPI_UserEvent> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Events")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Events")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Events")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Events")
    int32 Result_Total;
};

// Rating

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Rating"))
struct FModioAPI_Rating
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Rating")
    int32 Game_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Rating")
    int32 Mod_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Rating")
    TEnumAsByte<EModioAPI_RatingType> Rating;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Rating")
    FDateTime Date_Added;
};

// Get User Ratings

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get User Ratings"))
struct FModioAPI_GetUserRatings
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Ratings")
    TArray<FModioAPI_Rating> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Ratings")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Ratings")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Ratings")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get User Ratings")
    int32 Result_Total;
};

// Guide

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Guide"))
struct FModioAPI_Guide
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    int32 ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    int32 Game_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FString Game_Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FModioAPI_Logo_Object Logo;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FModioAPI_User User;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FDateTime Date_Updated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FDateTime Date_Live;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    TEnumAsByte<EModioAPI_GameGuideModStatus> Status;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FString URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FString Name_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FString Summary;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    FString Description;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    TArray<FModioAPI_GuideTag> Tags;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide")
    TArray<FModioAPI_GuideStats_Object> Stats;
};

// Mod User Preview

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod User Preview"))
struct FModioAPI_ModUserPreview
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod User Preview")
    FModioAPI_User User;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod User Preview")
    FString Resource_URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod User Preview")
    bool Subscribed;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod User Preview")
    FDateTime Date_Added;
};

// Status Audit Log

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Status Audit Log"))
struct FModioAPI_StatusAuditLog
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Status Audit Log")
    TEnumAsByte<EModioAPI_GameGuideModStatus> Status_New;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Status Audit Log")
    TEnumAsByte<EModioAPI_GameGuideModStatus> Status_Old;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Status Audit Log")
    FModioAPI_User User;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Status Audit Log")
    FDateTime Date_Added;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Status Audit Log")
    FString Reason;
};

// Pay

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Pay"))
struct FModioAPI_Pay_Object
{
    GENERATED_BODY()
};

/*
Mod.io API Helper Structs
*/

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Steam", meta = (DisplayName = "mod.io Steam Auth", Tooltip = "Steam Authentication"))
struct FModioAPI_SteamAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Steam", meta = (DisplayName = "Appdata (Required)", Tooltip = "The Steam users Encrypted App Ticket provided by the Steamworks SDK"))
    FString Appdata;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Steam", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Steam", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Steam", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Xbox Live", meta = (DisplayName = "mod.io Xbox Live Auth", Tooltip = "Xbox Live Authentication"))
struct FModioAPI_XboxLiveAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Xbox Live", meta = (DisplayName = "Xbox Token (Required)", Tooltip = "The Xbox Live Token"))
    FString XboxToken;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Xbox Live", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Xbox Live", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Xbox Live", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Playstation Network", meta = (DisplayName = "mod.io Playstation Network Auth", Tooltip = "Playstation Network Authentication"))
struct FModioAPI_PlaystationNetworkAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Playstation Network", meta = (DisplayName = "Auth Code (Required)", Tooltip = "The auth code returned from the PlayStation Network API"))
    FString AuthCode;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Playstation Network", meta = (DisplayName = "Environment (Required)", Tooltip = "The PlayStation Network environment you are targeting"))
    int32 Env;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Playstation Network", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Playstation Network", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Authentication|Playstation Network", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Nintendo Switch", meta = (DisplayName = "mod.io Nintendo Switch Auth", Tooltip = "Nintendo Switch Authentication"))
struct FModioAPI_NintendoSwitchAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Nintendo Switch", meta = (DisplayName = "ID Token (Required)", Tooltip = "The NSA ID supplied by the Nintendo Switch SDK"))
    FString ID_Token;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Nintendo Switch", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Nintendo Switch", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Nintendo Switch", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "mod.io Meta Quest Auth", Tooltip = "Meta Quest Authentication"))
struct FModioAPI_MetaQuestAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "Device (Required)", Tooltip = "The Meta Quest device being used for authentication."))
    TEnumAsByte<EModioAPI_MetaQuest_DeviceType> Device;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "Nonce (Required)"))
    FString Nonce;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "User ID (Required)"))
    int32 User_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "Access Token (Required)"))
    FString Access_Token;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Meta Quest", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Epic Games", meta = (DisplayName = "mod.io Epic Games Auth", Tooltip = "Epic Games Authentication"))
struct FModioAPI_EpicGamesAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Epic Games", meta = (DisplayName = "ID Token (Required)", Tooltip = "The ID Token returned from the EOS SDK when you authenticate a user to use mod.io"))
    FString ID_Token;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Epic Games", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Epic Games", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Epic Games", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|GOG Galaxy", meta = (DisplayName = "mod.io GOG Galaxy Auth", Tooltip = "GOG Galaxy Authentication"))
struct FModioAPI_GOGGalaxyAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|GOG Galaxy", meta = (DisplayName = "Appdata (Required)", Tooltip = "The GOG Galaxy users Encrypted App Ticket provided by the GOG Galaxy SDK"))
    FString Appdata;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|GOG Galaxy", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|GOG Galaxy", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|GOG Galaxy", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Google", meta = (DisplayName = "mod.io Google Auth", Tooltip = "Google Authentication"))
struct FModioAPI_GoogleAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Google", meta = (DisplayName = "ID Token (Required)", Tooltip = "The id_token value returned from Google after you have authenticated a user via the Google OAuth2 flow"))
    FString ID_Token;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Google", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Google", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|Discord", meta = (DisplayName = "mod.io Discord Auth", Tooltip = "Discord Authentication"))
struct FModioAPI_DiscordAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Discord", meta = (DisplayName = "Discord Token (Required)", Tooltip = "The access token of the user provided by Discord."))
    FString Discord_Token;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Discord", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Discord", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|Discord", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Authentication|OpenID", meta = (DisplayName = "mod.io OpenID Auth", Tooltip = "OpenID Authentication"))
struct FModioAPI_OpenIDAuth
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|OpenID", meta = (DisplayName = "ID Token (Required)", Tooltip = "The ID token issued by the configured identity provider."))
    FString ID_Token;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|OpenID", meta = (DisplayName = "Email (Optional)"))
    FString Email;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|OpenID", meta = (DisplayName = "Date Expires (Optional)"))
    FDateTime DateExpires;

    UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "mod.io API|Authentication|OpenID", meta = (DisplayName = "Terms Agreed (Optional)"))
    bool TermsAgreed;
};

USTRUCT(BlueprintType, Category = "mod.io API|Temp Cache", meta = (DisplayName = "Temp Cache"))
struct FModioAPI_TemporaryCache
{
    GENERATED_BODY()

    // The User ID of the local, authenticated User
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    int32 AuthenticatedUserID;

    // Terms of Service
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    FModioAPI_Terms CachedTerms;

    // Key = Game ID | Value = Game
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Game> CachedGames;

    // Key = User Event ID | Value = User Event
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_UserEvent> CachedEvents;

    // Key = User ID | Value = User
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_User> CachedUsers;

    // Key = User ID | Value = User
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_User> CachedMutedUsers;

    // Key = Mod ID | Value = Mod
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Mod> CachedMods;

    // Key = Modfile ID | Value = Modfile
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Modfile> CachedModfiles;

    // Key = Guide ID | Value = Guide Info
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Guide> CachedGuides;

    // Key = Mod ID | Value = Mod
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Mod> CachedSubscribedMods;

    // Key = Mod ID | Value = Mod
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Mod> CachedPurchasedMods;

    // Key = Mod ID | Value = Rating
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    TMap<int32, FModioAPI_Rating> CachedModRatings;

    // User Wallet (WiP by mod.io!)
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Temp Cache")
    FModioAPI_Wallet CachedWallet;
};

USTRUCT(BlueprintType, Category = "mod.io API|Persisting Cache", meta = (DisplayName = "Persisting Cache"))
struct FModioAPI_PersistingCache
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Persisting Cache")
    FModioAPI_AccessToken CachedAccessToken;
};

USTRUCT(BlueprintType, Category = "mod.io API|Guides|Add Guide", meta = (DisplayName = "mod.io Add Guide"))
struct FModioAPI_AddGuide
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "mod.io API|Guides|Edit Guide", meta = (DisplayName = "mod.io Edit Guide"))
struct FModioAPI_EditGuide
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "mod.io Add Mod"))
struct FModioAPI_AddMod
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Logo (Required)", Tooltip = "Absolute Path to File"))
    FString Logo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Name (Required)"))
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Name ID (Optional)"))
    FString Name_ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Summary (Required)"))
    FString Summary;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Visible (Optional)"))
    TEnumAsByte<EModioAPI_ModVisibility> Visible = EModioAPI_ModVisibility::ModVisibility_Public;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Description (Optional)"))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Homepage URL (Optional)"))
    FString Homepage_URL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Stock (Optional)"))
    int32 Stock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Maturity Option (Optional)"))
    FModioAPI_MaturityOptions Maturity_Option;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Community Options (Optional)"))
    FModioAPI_CommunityOptions Community_Options;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Metadata Blob (Optional)"))
    FString Metadata_Blob;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Add Mod", meta = (DisplayName = "Tags (Optional)"))
    TArray<FString> Tags;
};

USTRUCT(BlueprintType, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "mod.io Edit Mod"))
struct FModioAPI_EditMod
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Logo (Required)", Tooltip = "Absolute Path to File"))
    FString Logo;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Name (Required)"))
    FString Name;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Name ID (Optional)"))
    FString Name_ID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Summary (Required)"))
    FString Summary;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Visible (Required)"))
    TEnumAsByte<EModioAPI_ModVisibility> Visible = EModioAPI_ModVisibility::ModVisibility_Public;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Description (Optional)"))
    FString Description;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Homepage URL (Optional)"))
    FString Homepage_URL;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Stock (Required)"))
    int32 Stock;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Maturity Option (Required)"))
    FModioAPI_MaturityOptions Maturity_Option;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Community Options (Required)"))
    FModioAPI_CommunityOptions Community_Options;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Metadata Blob (Optional)"))
    FString Metadata_Blob;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Mods|Edit Mod", meta = (DisplayName = "Tags (Optional)"))
    TArray<FString> Tags;
};

USTRUCT(BlueprintType, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "mod.io Add Modfile"))
struct FModioAPI_AddModfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Name (Required)", Tooltip = "The Name for this Modfile. Will be used on the mod.io Server as a base for naming the package."))
    FName ModfileName;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Filedata (Required if no Upload ID)", Tooltip = "The Data for the Zip-File as Byte Array. Data size cannot exceed 500 MB! For bigger data use Multipart Uploads!"))
    TArray<uint8> Filedata;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Upload ID (Required if no Filedata)", Tooltip = "The Upload Session ID using Multipart Upload! The Session needs to be completed before requesting this!"))
    FString UploadID;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Version (Required)", Tooltip = "Version of the Modfile"))
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Changelog (Required)", Tooltip = "Changelog for the Modfile"))
    FString Changelog;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Active (Required)", Tooltip = "Flag this upload as the current release?"))
    bool Active = true;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Zip-File MD5 Filehash (Optional)", Tooltip = "The MD5 Filehash for the provided file will be used to check validity and completeness of the uploaded File after upload!"))
    FString ZipFileFilehash;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Metadata Blob (Optional)", Tooltip = "Metadata stored by the game developer which may include properties such as what version of the game this file is compatible with!"))
    FString MetadataBlob;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Add Modfile", meta = (DisplayName = "Platforms (Required)", Tooltip = "The Platforms this file is targeting! Namely: windows, mac, linux, android, ios, source, xboxone, xboxseriesx, ps4, ps5, switch, oculus"))
    TArray<TEnumAsByte<EModioAPI_Platforms>> Platforms;
};

USTRUCT(BlueprintType, Category = "mod.io API|Files|Edit Modfile", meta = (DisplayName = "mod.io Edit Modfile"))
struct FModioAPI_EditModfile
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Edit Modfile", meta = (DisplayName = "Version (Optional)", Tooltip = "Version of the Modfile"))
    FString Version;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Edit Modfile", meta = (DisplayName = "Changelog (Optional)", Tooltip = "Changelog for the Modfile"))
    FString Changelog;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Edit Modfile", meta = (DisplayName = "Active (Required)", Tooltip = "Flag this upload as the current release?"))
    bool Active;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Edit Modfile", meta = (DisplayName = "Metadata Blob (Optional)", Tooltip = "Metadata stored by the game developer which may include properties such as what version of the game this file is compatible with!"))
    FString Metadata_Blob;
};

USTRUCT(BlueprintType, Category = "mod.io API|Files|Manage Modfile", meta = (DisplayName = "mod.io Manage Modfile Platform Status"))
struct FModioAPI_ManageModfilePlatformStatus
{
    GENERATED_BODY()

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Manage Modfile", meta = (DisplayName = "Approved Platforms (Required)"))
    TArray<TEnumAsByte<EModioAPI_Platforms>> Approved;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "mod.io API|Files|Manage Modfile", meta = (DisplayName = "Denied Platforms (Required)"))
    TArray<TEnumAsByte<EModioAPI_Platforms>> Denied;
};

USTRUCT(BlueprintType, Category = "mod.io API|Media|Add Game Media", meta = (DisplayName = "mod.io Add Game Media"))
struct FModioAPI_AddGameMedia
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "mod.io API|Media|Add Game Media", meta = (DisplayName = "mod.io Add Game Media"))
struct FModioAPI_AddModMedia
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "mod.io API|Tags|Add Game Tag Option", meta = (DisplayName = "mod.io Add Game Tag Option"))
struct FModioAPI_AddGameTagOption
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "mod.io API|Tags|Add Game Tag Option", meta = (DisplayName = "mod.io Add Game Tag Option"))
struct FModioAPI_DeleteGameTagOption
{
    GENERATED_BODY()
};

USTRUCT(BlueprintType, Category = "mod.io API|Tags|Add Game Tag Option", meta = (DisplayName = "mod.io Add Game Tag Option"))
struct FModioAPI_SubmitReport
{
    GENERATED_BODY()
};

/*
Filtering
*/

USTRUCT()
struct FModioAPI_RequestFilter
{
    GENERATED_BODY()

    UPROPERTY()
    FString ColumnForFiltering;

    UPROPERTY()
    TEnumAsByte<EModioAPI_FilterComparison> Comparison;

    UPROPERTY()
    FString Value;
};

USTRUCT(BlueprintType, Category = "mod.io API|Filtering", meta = (DisplayName = "mod.io Request Filters"))
struct FModioAPI_RequestFilters
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_RequestFilter> Filters;
};

/*
Sorting
*/

USTRUCT(BlueprintType, Category = "mod.io API|Sorting", meta = (DisplayName = "mod.io Request Sorting"))
struct FModioAPI_RequestSorting
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    FString ColumnForSorting;

    UPROPERTY(BlueprintReadWrite)
    TEnumAsByte<EModioAPI_SortingOrder> Order;
};

/*
Pagination
*/

USTRUCT(BlueprintType, Category = "mod.io API|Pagination", meta = (DisplayName = "mod.io Request Pagination"))
struct FModioAPI_RequestPagination
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite)
    int64 Limit = -1;

    UPROPERTY(BlueprintReadWrite)
    int64 Offset = -1;
};

/*
Async Actions
*/

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Progress"))
struct FModioAPI_UploadProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int32 PartsUploaded;

    UPROPERTY(BlueprintReadOnly)
    int32 PartsTotal;

    UPROPERTY(BlueprintReadOnly)
    int64 BytesUploaded;

    UPROPERTY(BlueprintReadOnly)
    int64 BytesTotal;

    UPROPERTY(BlueprintReadOnly)
    float Progress;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Multipart Modfile Upload"))
struct FModioAPI_UploadModfileMultipartMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FString UploadSessionID;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_UploadProgress UploadProgress;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_Modfile AddedModfile;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "File Download Progress"))
struct FModioAPI_DownloadFileProgress
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    int64 BytesReceived;

    UPROPERTY(BlueprintReadOnly)
    int64 BytesTotal;

    UPROPERTY(BlueprintReadOnly)
    float Progress;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Modfile Download"))
struct FModioAPI_DownloadModfileMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_Modfile Modfile;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_DownloadFileProgress ProgressInfo;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EModioAPI_DownloadResult> Result;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Icon Download"))
struct FModioAPI_DownloadIconMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_Icon_Object Icon;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_DownloadFileProgress ProgressInfo;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EModioAPI_DownloadResult> Result;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Header Download"))
struct FModioAPI_DownloadHeaderMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_HeaderImage_Object Header;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_DownloadFileProgress ProgressInfo;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EModioAPI_DownloadResult> Result;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Logo Download"))
struct FModioAPI_DownloadLogoMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_Logo_Object Logo;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_DownloadFileProgress ProgressInfo;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EModioAPI_DownloadResult> Result;
};

USTRUCT(BlueprintType, Category = "mod.io API|Async Actions", meta = (DisplayName = "Avatar Download"))
struct FModioAPI_DownloadAvatarMessage
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly)
    FString ErrorMessage;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_Avatar_Object Avatar;

    UPROPERTY(BlueprintReadOnly)
    FModioAPI_DownloadFileProgress ProgressInfo;

    UPROPERTY(BlueprintReadOnly)
    TEnumAsByte<EModioAPI_DownloadResult> Result;
};