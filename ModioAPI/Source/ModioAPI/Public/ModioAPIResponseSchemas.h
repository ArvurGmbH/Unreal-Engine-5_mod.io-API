/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "ModioAPIResponseSchemas.generated.h"

/*
mod.io Response Schemas
*/

// Mod Tag

USTRUCT()
struct FModioAPI_ModTag_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    int64 Date_Added;
};

// Filehash

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Filehash"))
struct FModioAPI_Filehash_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Filehash")
    FString MD5;
};

// Avatar

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Avatar"))
struct FModioAPI_Avatar_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Filename;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Original;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Thumb_50x50;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Users")
    FString Thumb_100x100;
};

// Modfile Platform

USTRUCT()
struct FModioAPI_ModfilePlatform_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Platform;

    UPROPERTY()
    int32 Status;
};

// Modfile Platform Supported

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Modfile Platform Supported"))
struct FModioAPI_ModfilePlatformSupported
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform Supported")
    TArray<FString> Targetted;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform Supported")
    TArray<FString> Approved;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform Supported")
    TArray<FString> Denied;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform Supported")
    TArray<FString> Live;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Modfile Platform Supported")
    TArray<FString> Pending;
};

// User

USTRUCT()
struct FModioAPI_User_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    FString Name_ID;

    UPROPERTY()
    FString Username;

    UPROPERTY()
    FString Display_Name_Portal;

    UPROPERTY()
    int64 Date_Online;

    UPROPERTY()
    int64 Date_Joined;

    UPROPERTY()
    FModioAPI_Avatar_Object Avatar;

    UPROPERTY()
    FString Timezone;

    UPROPERTY()
    FString Language;

    UPROPERTY()
    FString Profile_URL;
};

// Mod Platforms

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod Platforms"))
struct FModioAPI_ModPlatforms
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Platforms")
    FString Platform;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Platforms", meta = (Tooltip = "ID of Modfile that is live on Platform"))
    int32 Modfile_Live;
};

// Mod Stats

USTRUCT()
struct FModioAPI_ModStats_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    int32 Popularity_Rank_Position;

    UPROPERTY()
    int32 Popularity_Rank_Total_Mods;

    UPROPERTY()
    int32 Downloads_Today;

    UPROPERTY()
    int32 Downloads_Total;

    UPROPERTY()
    int32 Subscribers_Total;

    UPROPERTY()
    int32 Ratings_Total;

    UPROPERTY()
    int32 Ratings_Positive;

    UPROPERTY()
    int32 Ratings_Negative;

    UPROPERTY()
    int32 Ratings_Percentage_Positive;

    UPROPERTY()
    int32 Ratings_Percentage_Negative;

    UPROPERTY()
    float Ratings_Weighted_Aggregate;

    UPROPERTY()
    FString Ratings_Display_Text;

    UPROPERTY()
    int64 Date_Expires;
};

// Download

USTRUCT()
struct FModioAPI_Download_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Binary_URL;

    UPROPERTY()
    int64 Date_Expires;
};

// Modfile

USTRUCT()
struct FModioAPI_Modfile_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int64 Date_Updated;

    UPROPERTY()
    int64 Date_Scanned;

    UPROPERTY()
    int32 Virus_Status;

    UPROPERTY()
    int32 Virus_Positive;

    UPROPERTY()
    int64 Filesize;

    UPROPERTY()
    int64 Filesize_Uncompressed;

    UPROPERTY()
    FModioAPI_Filehash_Object Filehash;

    UPROPERTY()
    FString Filename;

    UPROPERTY()
    FString Version;

    UPROPERTY()
    FString Changelog;

    UPROPERTY()
    FString Metadata_Blob;

    UPROPERTY()
    FModioAPI_Download_Object Download;

    UPROPERTY()
    TArray<FModioAPI_ModfilePlatform_Object> Platforms;
};

// Key-Value Pair

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Key-Value Pair"))
struct FModioAPI_KeyValuePair_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Key-Value Pair")
    FString Key;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Key-Value Pair")
    FString Value;
};

// Logo

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Logo"))
struct FModioAPI_Logo_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Logo")
    FString Filename;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Logo")
    FString Original;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Logo")
    FString Thumb_320x180;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Logo")
    FString Thumb_640x360;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Logo")
    FString Thumb_1280x720;
};

// Message

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Message"))
struct FModioAPI_Message_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Message")
    int32 Code;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Message")
    FString Message;
};

// Metadata KVP

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Metadata KVP", Tooltip = "Metadata Key-Value Pair"))
struct FModioAPI_MetadataKVP_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Metadata KVP")
    FString Metakey;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Metadata KVP")
    FString Metavalue;
};

// Image

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Image"))
struct FModioAPI_Image_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Image")
    FString Filename;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Image")
    FString Original;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Image")
    FString Thumb_320x180;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Image")
    FString Thumb_1280x720;
};

// Access Token

USTRUCT()
struct FModioAPI_AccessToken_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Code;

    UPROPERTY()
    FString Access_Token;

    UPROPERTY()
    int64 Date_Expires;
};

// Agreement Version

USTRUCT()
struct FModioAPI_AgreementVersion_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    bool IsActive;

    UPROPERTY()
    int32 Type;
};

// Mod Dependencies

USTRUCT()
struct FModioAPI_ModDependencies_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString Name_ID;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int32 Dependency_Depth;

    UPROPERTY()
    FModioAPI_Logo_Object Logo;

    UPROPERTY()
    FModioAPI_Modfile_Object Modfile;
};


// Mod Media

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Mod Media"))
struct FModioAPI_ModMedia_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Media")
    TArray<FString> Youtube;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Media")
    TArray<FString> Sketchfab;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Mod Media")
    TArray<FModioAPI_Image_Object> Images;
};

// Mod

USTRUCT()
struct FModioAPI_Mod_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Game_ID;

    UPROPERTY()
    int32 Status;

    UPROPERTY()
    int32 Visible;

    UPROPERTY()
    FModioAPI_User_Object Submitted_By;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int64 Date_Updated;

    UPROPERTY()
    int64 Date_Live;

    UPROPERTY()
    int32 Maturity_Option;

    UPROPERTY()
    int32 Community_Options;

    UPROPERTY()
    int32 Monetization_Options;

    UPROPERTY()
    int32 Price;

    UPROPERTY()
    int32 Tax;

    UPROPERTY()
    FModioAPI_Logo_Object Logo;

    UPROPERTY()
    FString Homepage_URL;

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString Name_ID;

    UPROPERTY()
    FString Summary;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    FString Description_Plaintext;

    UPROPERTY()
    FString Metadata_Blob;

    UPROPERTY()
    FString Profile_URL;

    UPROPERTY()
    FModioAPI_ModMedia_Object Media;

    UPROPERTY()
    FModioAPI_Modfile_Object Modfile;

    UPROPERTY()
    bool Dependencies;

    UPROPERTY()
    FModioAPI_ModStats_Object Stats;

    UPROPERTY()
    TArray<FModioAPI_ModPlatforms> Platforms;

    UPROPERTY()
    TArray<FModioAPI_MetadataKVP_Object> Metadata_KVP;

    UPROPERTY()
    TArray<FModioAPI_ModTag_Object> Tags;
};

// Comment

USTRUCT()
struct FModioAPI_Comment_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Game_ID;

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    int32 Resource_ID;

    UPROPERTY()
    FModioAPI_User_Object User;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int32 Reply_ID;

    UPROPERTY()
    FString Thread_Position;

    UPROPERTY()
    int32 Karma;

    UPROPERTY()
    FString Content;
};

// Error

USTRUCT(BlueprintType, Category = "mod.io API|Error", meta = (DisplayName = "mod.io Error Contents", Tooltip = "Error Contents"))
struct FModioAPI_Error_Contents
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Error")
    int32 Code;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Error")
    int32 Error_Ref;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Error")
    FString Message;

    // ToDo: Errors Array
};

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Error", Tooltip = "Error Object"))
struct FModioAPI_Error_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Error")
    FModioAPI_Error_Contents Error;
};

// Game Monetization Team

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game Monetization Team"))
struct FModioAPI_GameMonetizationTeam_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Monetization Team")
    int32 Team_ID;
};

// Header Image

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Header Image"))
struct FModioAPI_HeaderImage_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Header Image")
    FString Filename;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Header Image")
    FString Original;
};

// Icon

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Icon"))
struct FModioAPI_Icon_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Icon")
    FString Filename;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Icon")
    FString Original;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Icon")
    FString Thumb_64x64;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Icon")
    FString Thumb_128x128;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Icon")
    FString Thumb_256x256;
};

// Guide Stats

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Guide Stats"))
struct FModioAPI_GuideStats_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide Stats")
    int32 Guide_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide Stats")
    int32 Visits_Total;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Guide Stats")
    int32 Comments_Total;
};

// Guide Tag

USTRUCT()
struct FModioAPI_GuideTag_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Name;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int32 Count;
};

// Game Stats

USTRUCT()
struct FModioAPI_GameStats_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Game_ID;

    UPROPERTY()
    int32 Mods_Count_Total;

    UPROPERTY()
    int32 Mods_Downloads_Today;

    UPROPERTY()
    int32 Mods_Downloads_Total;

    UPROPERTY()
    int32 Mods_Downloads_Daily_Average;

    UPROPERTY()
    int32 Mods_Subscribers_Total;

    UPROPERTY()
    int64 Date_Expires;
};

// Theme

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Theme"))
struct FModioAPI_Theme
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Theme")
    FString Primary;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Theme")
    FString Dark;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Theme")
    FString Light;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Theme")
    FString Success;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Theme")
    FString Warning;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Theme")
    FString Danger;
};

// Game OtherURLs

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game Other URLs"))
struct FModioAPI_GameOtherURLs_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString Label;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Games")
    FString URL;
};

// Game Platforms

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game Platforms"))
struct FModioAPI_GamePlatforms_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Platforms")
    FString Platform;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Platforms")
    FString Label;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Platforms")
    bool Moderated;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Platforms")
    bool Locked;
};

// Game Tag Option

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Game Tag Option"))
struct FModioAPI_GameTagOption_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Tag Option")
    FString Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Tag Option")
    FString Type;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Tag Option")
    bool Hidden;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Tag Option")
    bool Locked;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Game Tag Option")
    TArray<FString> Tags;
};

// Guide

USTRUCT()
struct FModioAPI_Guide_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Game_ID;

    UPROPERTY()
    FString Game_Name;

    UPROPERTY()
    FModioAPI_Logo_Object Logo;

    UPROPERTY()
    FModioAPI_User_Object User;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int64 Date_Updated;

    UPROPERTY()
    int64 Date_Live;

    UPROPERTY()
    int32 Status;

    UPROPERTY()
    FString URL;

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString Name_ID;

    UPROPERTY()
    FString Summary;

    UPROPERTY()
    FString Description;

    UPROPERTY()
    TArray<FModioAPI_GuideTag_Object> Tags;

    UPROPERTY()
    TArray<FModioAPI_GuideStats_Object> Stats;
};

// Game

USTRUCT()
struct FModioAPI_Game_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Status;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    int64 Date_Updated;

    UPROPERTY()
    int64 Date_Live;

    UPROPERTY()
    int32 Presentation_Option;

    UPROPERTY()
    int32 Submission_Option;

    UPROPERTY()
    int32 Curation_Option;

    UPROPERTY()
    int32 Community_Options;

    UPROPERTY()
    int32 Monetization_Options;

    UPROPERTY()
    FModioAPI_GameMonetizationTeam_Object Monetization_Team;

    UPROPERTY()
    int32 API_Access_Options;

    UPROPERTY()
    int32 Maturity_Options;

    UPROPERTY()
    FString UGC_Name;

    UPROPERTY()
    FString Token_Name;

    UPROPERTY()
    FModioAPI_Icon_Object Icon;

    UPROPERTY()
    FModioAPI_Logo_Object Logo;

    UPROPERTY()
    FModioAPI_HeaderImage_Object Header;

    UPROPERTY()
    FString Name;

    UPROPERTY()
    FString Name_ID;

    UPROPERTY()
    FString Summary;

    UPROPERTY()
    FString Instructions;

    UPROPERTY()
    FString Instructions_URL;

    UPROPERTY()
    FString Profile_URL;

    UPROPERTY()
    FModioAPI_GameStats_Object Stats;

    UPROPERTY()
    FModioAPI_Theme Theme;

    UPROPERTY()
    TArray<FModioAPI_GameOtherURLs_Object> Other_URLs;

    UPROPERTY()
    TArray<FModioAPI_GameTagOption_Object> Tag_Options;

    UPROPERTY()
    TArray<FModioAPI_GamePlatforms_Object> Platforms;
};

// Game User Preview

USTRUCT()
struct FModioAPI_GameUserPreview_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FModioAPI_User_Object User;

    UPROPERTY()
    FString Resource_URL;

    UPROPERTY()
    int64 Date_Added;
};

// Get Game Tag Options

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Game Tag Options"))
struct FModioAPI_GetGameTagOptions
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Game Tag Options")
    TArray<FModioAPI_GameTagOption_Object> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Game Tag Options")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Game Tag Options")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Game Tag Options")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Game Tag Options")
    int32 Result_Total;
};

// Get Games

USTRUCT()
struct FModioAPI_GetGames_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_Game_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Guide Comments

USTRUCT()
struct FModioAPI_GetGuideComments_Schema
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

// Get Guide Tags

USTRUCT()
struct FModioAPI_GetGuideTags_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_GuideTag_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Guides

USTRUCT()
struct FModioAPI_GetGuides_Schema
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

// Get Mod Comments

USTRUCT()
struct FModioAPI_GetModComments_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_Comment_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Mod Dependencies

USTRUCT()
struct FModioAPI_GetModDependencies_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_ModDependencies_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Mod Event

USTRUCT()
struct FModioAPI_ModEvent_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    int32 User_ID;

    UPROPERTY()
    int64 DateAdded;

    UPROPERTY()
    FString Event_Type;
};

// Get Mod Events

USTRUCT()
struct FModioAPI_GetModEvents_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_ModEvent_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Mod KVP Metadata

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Get Mod KVP Metadata"))
struct FModioAPI_GetModMetadataKVP
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    TArray<FModioAPI_MetadataKVP_Object> Data;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Count;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Offset;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Limit;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Get Mod Events")
    int32 Result_Total;
};

// Get Mod Stats

USTRUCT()
struct FModioAPI_GetModStats_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_ModStats_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Mod Stats

USTRUCT()
struct FModioAPI_GetModTags_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_ModTag_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Modfiles

USTRUCT()
struct FModioAPI_GetModfiles_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_Modfile_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Mods

USTRUCT()
struct FModioAPI_GetMods_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_Mod_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};


// Multipart Upload

USTRUCT()
struct FModioAPI_MultipartUpload_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Upload_ID;

    UPROPERTY()
    int32 Status;
};

// Multipart Upload Part

USTRUCT()
struct FModioAPI_MultipartUploadPart_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Upload_ID;

    UPROPERTY()
    int32 Part_Number;

    UPROPERTY()
    int64 Part_Size;

    UPROPERTY()
    int64 Date_Added;
};

// Get Multipart Upload Parts

USTRUCT()
struct FModioAPI_GetMultipartUploadParts_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_MultipartUploadPart_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Multipart Upload Sessions

USTRUCT()
struct FModioAPI_GetMultipartUploadSessions_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_MultipartUpload_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Get Muted Users

USTRUCT()
struct FModioAPI_GetMutedUsers_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_User_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};


// Team Member

USTRUCT()
struct FModioAPI_TeamMember_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    FModioAPI_User_Object User;

    UPROPERTY()
    int32 Level;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    FString Position;

    UPROPERTY()
    int32 Invite_Pending;
};

// Get Muted Users

USTRUCT()
struct FModioAPI_GetTeamMembers_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_TeamMember_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};


// User Event

USTRUCT()
struct FModioAPI_UserEvent_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 ID;

    UPROPERTY()
    int32 Game_ID;

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    int32 User_ID;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    FString Event_Type;
};

// Get User Events

USTRUCT()
struct FModioAPI_GetUserEvents_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_UserEvent_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Rating

USTRUCT()
struct FModioAPI_Rating_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Game_ID;

    UPROPERTY()
    int32 Mod_ID;

    UPROPERTY()
    int32 Rating;

    UPROPERTY()
    int64 Date_Added;
};

// Get User Ratings

USTRUCT()
struct FModioAPI_GetUserRatings_Schema
{
    GENERATED_BODY()

    UPROPERTY()
    TArray<FModioAPI_Rating_Object> Data;

    UPROPERTY()
    int32 Result_Count;

    UPROPERTY()
    int32 Result_Offset;

    UPROPERTY()
    int32 Result_Limit;

    UPROPERTY()
    int32 Result_Total;
};

// Mod User Preview

USTRUCT()
struct FModioAPI_ModUserPreview_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FModioAPI_User_Object User;

    UPROPERTY()
    FString Resource_URL;

    UPROPERTY()
    bool Subscribed;

    UPROPERTY()
    int64 Date_Added;
};

// Pay

// Preview

// Proxy Transformer

// Status Audit Log

USTRUCT()
struct FModioAPI_StatusAuditLog_Object
{
    GENERATED_BODY()

    UPROPERTY()
    int32 Status_New;

    UPROPERTY()
    int32 Status_Old;

    UPROPERTY()
    FModioAPI_User_Object User;

    UPROPERTY()
    int64 Date_Added;

    UPROPERTY()
    FString Reason;
};

// Button Info

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Button"))
struct FModioAPI_ButtonInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Button")
    FString Text;
};

// Link Info

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Link"))
struct FModioAPI_LinkInfo
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Link")
    FString Text;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Link")
    FString URL;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Link")
    bool Required;
};

// Terms

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Terms of Service"))
struct FModioAPI_Terms
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Terms of Service")
    FString Plaintext;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Terms of Service")
    FString HTML;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Terms of Service")
    TMap<FString, FModioAPI_ButtonInfo> Buttons;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Terms of Service")
    TMap<FString, FModioAPI_LinkInfo> Links;
};

// User Access

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io User Access"))
struct FModioAPI_UserAccess_Object
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Access")
    FString Resource_Type;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Access")
    int32 Resource_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Access")
    int32 Resource_Name;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Access")
    FString Resource_Name_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|User Access")
    FString Resource_URL;
};

// Wallet

USTRUCT()
struct FModioAPI_Wallet_Object
{
    GENERATED_BODY()

    UPROPERTY()
    FString Type;

    UPROPERTY()
    FString Payment_Method_ID;

    UPROPERTY()
    FString Currency;

    UPROPERTY()
    int32 Balance;
};

USTRUCT(BlueprintType, Category = "mod.io API|Responses", meta = (DisplayName = "mod.io Wallet"))
struct FModioAPI_Wallet
{
    GENERATED_BODY()

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Wallet")
    FString Type;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Wallet")
    FString Payment_Method_ID;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Wallet")
    FString Currency;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "mod.io API|Wallet")
    int32 Balance;
};