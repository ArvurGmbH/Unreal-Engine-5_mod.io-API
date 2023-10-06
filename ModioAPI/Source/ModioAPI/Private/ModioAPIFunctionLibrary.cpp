/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/


#include "ModioAPIFunctionLibrary.h"

FModioAPI_Error_Object UModioAPIFunctionLibrary::ConvertJsonObjectToError(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Error_Object();
    }

    FModioAPI_Error_Object Error;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Error_Object>(JsonObject.ToSharedRef(), &Error))
    {
        Success = false;
        Message = "Error converting JSON Object to Error Response!";
        return FModioAPI_Error_Object();
    }

    Success = true;
    Message = "Successfully converted JSON Object to Error Response!";
    return Error;
}

FModioAPI_Terms UModioAPIFunctionLibrary::ConvertJsonObjectToTermsOfService(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Terms();
    }

    FModioAPI_Terms Terms;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Terms>(JsonObject.ToSharedRef(), &Terms))
    {
        Success = false;
        Message = "Error converting JSON Object to Terms of Service!";
        return FModioAPI_Terms();
    }

    Success = true;
    Message = "Successfully converted JSON Object to Terms of Service!";
    return Terms;
}

FModioAPI_Message_Object UModioAPIFunctionLibrary::ConvertJsonObjectToMessage(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Message_Object();
    }

    FModioAPI_Message_Object ConvertedMessage;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Message_Object>(JsonObject.ToSharedRef(), &ConvertedMessage))
    {
        Success = false;
        Message = "Error converting JSON Object to Message!";
        return FModioAPI_Message_Object();
    }

    Success = true;
    Message = "Successfully converted JSON Object to Message!";
    return ConvertedMessage;
}

FModioAPI_AccessToken_Response UModioAPIFunctionLibrary::ConvertJsonObjectToAccessToken(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_AccessToken_Response();
    }

    FModioAPI_AccessToken_Object AccessToken;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_AccessToken_Object>(JsonObject.ToSharedRef(), &AccessToken))
    {
        Success = false;
        Message = "Error converting JSON Object to Access Token!";
        return FModioAPI_AccessToken_Response();
    }

    FModioAPI_AccessToken_Response AccessTokenResponse;
    AccessTokenResponse.Code = AccessToken.Code;
    AccessTokenResponse.AccessToken.AccessToken = AccessToken.AccessToken;
    AccessTokenResponse.AccessToken.DateExpires = FDateTime::FromUnixTimestamp(AccessToken.DateExpires);

    Success = true;
    Message = "Successfully converted JSON Object to Access Token!";
    return AccessTokenResponse;
}

FModioAPI_User UModioAPIFunctionLibrary::ConvertJsonObjectToUser(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_User();
    }

    FModioAPI_User_Object UserInfo;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_User_Object>(JsonObject.ToSharedRef(), &UserInfo))
    {
        Success = false;
        Message = "Error converting JSON Object to User!";
        return FModioAPI_User();
    }

    FModioAPI_User User;
    User.ID = UserInfo.ID;
    User.Name_ID = UserInfo.Name_ID;
    User.Username = UserInfo.Username;
    User.Display_Name_Portal = UserInfo.Display_Name_Portal;
    User.Date_Online = FDateTime::FromUnixTimestamp(UserInfo.Date_Online);
    User.Date_Joined = FDateTime::FromUnixTimestamp(UserInfo.Date_Joined);
    User.Avatar = UserInfo.Avatar;
    User.Timezone = UserInfo.Timezone;
    User.Language = UserInfo.Language;
    User.Profile_URL = UserInfo.Profile_URL;

    Success = true;
    Message = "Successfully converted JSON Object to User!";
    return User;
}

FModioAPI_Game UModioAPIFunctionLibrary::ConvertJsonObjectToGame(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Game();
    }

    FModioAPI_Game_Object GameInfo;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Game_Object>(JsonObject.ToSharedRef(), &GameInfo))
    {
        Success = false;
        Message = "Error converting JSON Object to Game!";
        return FModioAPI_Game();
    }

    FModioAPI_Game Game;
    Game.ID = GameInfo.ID;

    // Game Status
    if (GameInfo.Status == 0) { Game.Status = EModioAPI_GameGuideModStatus::GameStatus_NotAccepted; }
    else if (GameInfo.Status == 1) { Game.Status = EModioAPI_GameGuideModStatus::GameStatus_Accepted; }
    else if (GameInfo.Status == 3) { Game.Status = EModioAPI_GameGuideModStatus::GameStatus_Deleted; }

    Game.Date_Added = FDateTime::FromUnixTimestamp(GameInfo.Date_Added);
    Game.Date_Updated = FDateTime::FromUnixTimestamp(GameInfo.Date_Updated);
    Game.Date_Live = FDateTime::FromUnixTimestamp(GameInfo.Date_Live);

    // Game Presentation Option
    if (GameInfo.Presentation_Option == 0) { Game.Presentation_Option = EModioAPI_GamePresentation::GamePresentation_GridView; }
    else if (GameInfo.Presentation_Option == 1) { Game.Presentation_Option = EModioAPI_GamePresentation::GamePresentation_TableView; }

    // Game Mods Submission Option
    if (GameInfo.Submission_Option == 0) { Game.Submission_Option = EModioAPI_GameModsSubmission::GameModsSubmission_ToolsAPIOnly; }
    else if (GameInfo.Submission_Option == 1) { Game.Submission_Option = EModioAPI_GameModsSubmission::GameModsSubmission_Anywhere; }

    // Game Mods Curation
    if (GameInfo.Curation_Option == 0) { Game.Curation_Option = EModioAPI_GameModsCuration::GameModsCuration_NoCuration; }
    else if (GameInfo.Curation_Option == 1) { Game.Curation_Option = EModioAPI_GameModsCuration::GameModsCuration_PriceChanges; }
    else if (GameInfo.Curation_Option == 2) { Game.Curation_Option = EModioAPI_GameModsCuration::GameModsCuration_FullCuration; }

    // ToDo: Extract Community Options...

    // Options here!

    Game.UGC_Name = GameInfo.UGC_Name;
    Game.Token_Name = GameInfo.Token_Name;
    Game.Icon = GameInfo.Icon;
    Game.Logo = GameInfo.Logo;
    Game.Header = GameInfo.Header;
    Game.Name = GameInfo.Name;
    Game.Name_ID = GameInfo.Name_ID;
    Game.Summary = GameInfo.Summary;
    Game.Instructions = GameInfo.Instructions;
    Game.Instructions_URL = GameInfo.Instructions_URL;
    Game.Profile_URL = GameInfo.Profile_URL;

    // Game Stats
    Game.Stats.Game_ID = GameInfo.Stats.Game_ID;
    Game.Stats.Mods_Count_Total = GameInfo.Stats.Mods_Count_Total;
    Game.Stats.Mods_Downloads_Today = GameInfo.Stats.Mods_Downloads_Today;
    Game.Stats.Mods_Downloads_Total = GameInfo.Stats.Mods_Downloads_Total;
    Game.Stats.Mods_Downloads_Daily_Average = GameInfo.Stats.Mods_Downloads_Daily_Average;
    Game.Stats.Mods_Subscribers_Total = GameInfo.Stats.Mods_Subscribers_Total;
    Game.Stats.Date_Expires = FDateTime::FromUnixTimestamp(GameInfo.Stats.Date_Expires);

    Game.Theme = GameInfo.Theme;
    Game.Other_URLs = GameInfo.Other_URLs;
    Game.Tag_Options = GameInfo.Tag_Options;
    Game.Platforms = GameInfo.Platforms;

    Success = true;
    Message = "Successfully converted JSON Object to Game!";
    return Game;
}

FModioAPI_GetMods UModioAPIFunctionLibrary::ConvertJsonObjectToGetMods(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetMods();
    }

    FModioAPI_GetMods_Schema GetModsSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetMods_Schema>(JsonObject.ToSharedRef(), &GetModsSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Mods!";
        return FModioAPI_GetMods();
    }

    FModioAPI_GetMods GetMods;

    Success = true;
    Message = "Successfully converted JSON Object to Mods!";
    return GetMods;
}

FModioAPI_Mod UModioAPIFunctionLibrary::ConvertJsonObjectToMod(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Mod();
    }

    FModioAPI_Mod Mod;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Mod>(JsonObject.ToSharedRef(), &Mod))
    {
        Success = false;
        Message = "Error converting JSON Object to Mod!";
        return FModioAPI_Mod();
    }

    Success = true;
    Message = "Successfully converted JSON Object to Mod!";
    return Mod;
}

FHttpRequestRef UModioAPIFunctionLibrary::CreateRequestForAddMod(FModioAPI_AddMod AddMod, bool& Success, FString& Message)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    FString BoundaryLabel = FString(TEXT("ModioAPI_UnrealEngine_")) + FString::FromInt(FMath::Rand());

    Request->AppendToHeader("Content-Type", "multipart/form-data; boundary=" + BoundaryLabel);

    // Payload
    TArray<uint8> RequestPayload;

    // Extract the File Name from the Path
    FString FileName;
    FString CutAway;
    FPlatformFileManager::Get().GetPlatformFile().GetFilenameOnDisk(*AddMod.Logo).Split(TEXT("/"), &CutAway, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

    // Required Payload
    RequestPayload.Append(CreateBoundaryForImagePNG("logo", AddMod.Logo, BoundaryLabel));
    RequestPayload.Append(StringToByteArray(CreateBoundaryForString("name", AddMod.Name, BoundaryLabel)));
    RequestPayload.Append(StringToByteArray(CreateBoundaryForString("summary", AddMod.Summary, BoundaryLabel)));

    // Optional Payload
    if (!AddMod.Name_ID.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("name_id", AddMod.Name_ID, BoundaryLabel)));
    }

    int32 ModVisible = AddMod.Visible;
    if (ModVisible != 1)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("name_id", ModVisible, BoundaryLabel)));
    }

    if (!AddMod.Description.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("description", AddMod.Description, BoundaryLabel)));
    }

    if (!AddMod.Homepage_URL.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("homepage_url", AddMod.Homepage_URL, BoundaryLabel)));
    }

    if (AddMod.Stock > 0)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("stock", AddMod.Stock, BoundaryLabel)));
    }

    if (AddMod.Maturity_Option > 0)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("maturity_option", AddMod.Maturity_Option, BoundaryLabel)));
    }

    if (AddMod.Community_Options != 1)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("community_options", AddMod.Community_Options, BoundaryLabel)));
    }

    if (!AddMod.Metadata_Blob.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("metadata_blob", AddMod.Metadata_Blob, BoundaryLabel)));
    }

    /* Not working yet!
    if (!AddMod.Tags.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("tags", AddMod.Tags, BoundaryLabel)));
    }
    */

    // Mark end of Payload
    FString BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--"));
    RequestPayload.Append(StringToByteArray(BoundaryEnd));

    // Attach Payload to Request
    Request->SetContent(RequestPayload);

    Success = true;
    Message = "Successfully extracted Add Mod Request Body from Add Mod Struct!";;
    return Request;
}

TArray64<uint8> UModioAPIFunctionLibrary::StringToByteArray(const FString& InString)
{
    TArray64<uint8> OutBytes;

    // Handle empty strings
    if (InString.Len() > 0)
    {
        FTCHARToUTF8 Converted(*InString); // Convert to UTF8
        OutBytes.Append(reinterpret_cast<const uint8*>(Converted.Get()), Converted.Length());
    }

    return OutBytes;
}

FString UModioAPIFunctionLibrary::CreateBoundaryForString(FString Key, FString Value, FString BoundaryLabel)
{
    // --BoundaryLabel
    FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));

    FString OutString =
        (TEXT("\r\n"))
        + BoundaryBegin
        + FString(TEXT("Content-Disposition: form-data; name=\""))
        + Key
        + FString(TEXT("\"\r\n\r\n"))
        + Value;

    /* OutString is:
    
    --BoundaryLabel
    Content-Disposition: form-data; name="<Key>"

    <Value>*/
    return OutString;
}

FString UModioAPIFunctionLibrary::CreateBoundaryForInteger(FString Key, int32 Value, FString BoundaryLabel)
{
    // --BoundaryLabel
    FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));

    FString OutString =
        (TEXT("\r\n"))
        + BoundaryBegin
        + FString(TEXT("Content-Disposition: form-data; name=\""))
        + Key
        + FString(TEXT("\"\r\n\r\n"))
        + FString::FromInt(Value);

    /* OutString is:

    --BoundaryLabel
    Content-Disposition: form-data; name="<Key>"

    <Value>*/
    return OutString;
}

FString UModioAPIFunctionLibrary::CreateBoundaryForStringArray(FString Key, TArray<FString> Value, FString BoundaryLabel)
{
    // --BoundaryLabel
    FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));

    FString OutString =
        (TEXT("\r\n"))
        + BoundaryBegin
        + FString(TEXT("Content-Disposition: form-data; name=\""))
        + Key
        + FString(TEXT("\"\r\n\r\n"));

    for (FString string : Value)
    {
        OutString.Append("tags[");
        OutString.Append(""); // Tag Category
        OutString.Append("]=");
        OutString.Append(""); // Tag Value
        OutString.Append(",");
    }

    /* OutString is:

    --BoundaryLabel
    Content-Disposition: form-data; name="<Key>"

    <Value>*/
    return OutString;
}

TArray64<uint8> UModioAPIFunctionLibrary::CreateBoundaryForImagePNG(FString Key, FString ImageFilePath, FString BoundaryLabel)
{
    TArray64<uint8> BoundaryData;

    // --BoundaryLabel
    FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));

    // Load Image
    FImage LoadedImage;
    FImageUtils::LoadImage(*ImageFilePath, LoadedImage);

    // Write Loaded Image to Byte-Array
    TArray64<uint8> ImageBuffer;
    FImageUtils::CompressImage(ImageBuffer, TEXT("png"), LoadedImage, 0);

    // Extract the File Name from the File Path
    FString FileName;
    FString CutAway;
    FPlatformFileManager::Get().GetPlatformFile().GetFilenameOnDisk(*ImageFilePath).Split(TEXT("/"), &CutAway, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

    // First, we add the boundary for the logo, which is different from text payload
    FString ImageBoundaryString =
        FString(TEXT("\r\n"))
        + BoundaryBegin
        + FString(TEXT("Content-Disposition: form-data; name=\"" + Key + "\"; filename=\"" + FileName + "\""))
        + "\r\n"
        + "Content-Type: image/png"
        + FString(TEXT("\r\n\r\n"));

    /* ImageBoundaryString is:
    
    --BoundaryLabel
    Content-Disposition: form-data; name="<Key>"; filename="<FileName>"
    Content-Type: image/png

    */

    // Notice, we convert all strings into uint8 format using FStringToUint8
    BoundaryData.Append(StringToByteArray(ImageBoundaryString));
    BoundaryData.Append(ImageBuffer);
    BoundaryData.Append(StringToByteArray("\r\n"));

    return BoundaryData;
}

FModioAPI_GetUserEvents UModioAPIFunctionLibrary::ConvertJsonObjectToGetUserEvents(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetUserEvents();
    }

    FModioAPI_GetUserEvents_Schema GetUserEventsSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetUserEvents_Schema>(JsonObject.ToSharedRef(), &GetUserEventsSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get User Events!";
        return FModioAPI_GetUserEvents();
    }

    FModioAPI_GetUserEvents GetUserEvents = ConvertGetUserEventsSchemaToGetUserEvents(GetUserEventsSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Mods!";
    return GetUserEvents;
}

FModioAPI_ModfilePlatform UModioAPIFunctionLibrary::ConvertModfilePlatformObjectToModfilePlatform(FModioAPI_ModfilePlatform_Object ModfilePlatformObject)
{
    FModioAPI_ModfilePlatform ReturnValue;

    ReturnValue.Platform = ModfilePlatformObject.Platform;

    if (ModfilePlatformObject.Status == 0) { ReturnValue.Status = EModioAPI_ModfilePlatformStatus::ModfilePlatformStatus_Pending; }
    else if (ModfilePlatformObject.Status == 1) { ReturnValue.Status = EModioAPI_ModfilePlatformStatus::ModfilePlatformStatus_Approved; }
    else if (ModfilePlatformObject.Status == 2) { ReturnValue.Status = EModioAPI_ModfilePlatformStatus::ModfilePlatformStatus_Denied; }
    else if (ModfilePlatformObject.Status == 3) { ReturnValue.Status = EModioAPI_ModfilePlatformStatus::ModfilePlatformStatus_Targetted; }

    return ReturnValue;
}

FModioAPI_Download UModioAPIFunctionLibrary::ConvertDownloadObjectToDownload(FModioAPI_Download_Object DownloadObject)
{
    FModioAPI_Download ReturnValue;

    ReturnValue.Binary_URL = DownloadObject.Binary_URL;
    ReturnValue.Date_Expires = FDateTime::FromUnixTimestamp(DownloadObject.Date_Expires);

    return ReturnValue;
}

FModioAPI_Modfile UModioAPIFunctionLibrary::ConvertModfileObjectToModfile(FModioAPI_Modfile_Object ModfileObject)
{
    FModioAPI_Modfile ReturnValue;

    ReturnValue.ID = ModfileObject.ID;
    ReturnValue.Mod_ID = ModfileObject.Mod_ID;
    ReturnValue.Date_Added = FDateTime::FromUnixTimestamp(ModfileObject.Date_Added);
    ReturnValue.Date_Updated = FDateTime::FromUnixTimestamp(ModfileObject.Date_Updated);
    ReturnValue.Date_Scanned = FDateTime::FromUnixTimestamp(ModfileObject.Date_Scanned);

    if (ModfileObject.Virus_Status == 0) { ReturnValue.Virus_Status = EModioAPI_ModfileVirusStatus::ModfileVirusStatus_NotScanned; }
    else if (ModfileObject.Virus_Status == 1) { ReturnValue.Virus_Status = EModioAPI_ModfileVirusStatus::ModfileVirusStatus_ScanComplete; }
    else if (ModfileObject.Virus_Status == 2) { ReturnValue.Virus_Status = EModioAPI_ModfileVirusStatus::ModfileVirusStatus_InProgress; }
    else if (ModfileObject.Virus_Status == 3) { ReturnValue.Virus_Status = EModioAPI_ModfileVirusStatus::ModfileVirusStatus_TooLargeToScan; }
    else if (ModfileObject.Virus_Status == 4) { ReturnValue.Virus_Status = EModioAPI_ModfileVirusStatus::ModfileVirusStatus_FileNotFound; }
    else if (ModfileObject.Virus_Status == 5) { ReturnValue.Virus_Status = EModioAPI_ModfileVirusStatus::ModfileVirusStatus_ErrorScanning; }
    
    if (ModfileObject.Virus_Positive == 0) { ReturnValue.Virus_Positive = EModioAPI_ModfileVirusPositivity::ModfileVirusPositive_NoThreats; }
    else if (ModfileObject.Virus_Positive == 1) { ReturnValue.Virus_Positive = EModioAPI_ModfileVirusPositivity::ModfileVirusPositive_Malicious; }
    else if (ModfileObject.Virus_Positive == 2) { ReturnValue.Virus_Positive = EModioAPI_ModfileVirusPositivity::ModfileVirusPositive_ContainingHarmfulFiles; }

    ReturnValue.Filesize = ModfileObject.Filesize;
    ReturnValue.Filesize_Uncompressed = ModfileObject.Filesize_Uncompressed;
    ReturnValue.Filehash = ModfileObject.Filehash;
    ReturnValue.Filename = ModfileObject.Filename;
    ReturnValue.Version = ModfileObject.Version;
    ReturnValue.Changelog = ModfileObject.Changelog;
    ReturnValue.Metadata_Blob = ModfileObject.Metadata_Blob;
    ReturnValue.Download = ConvertDownloadObjectToDownload(ModfileObject.Download);

    for (FModioAPI_ModfilePlatform_Object ModfilePlatformObject : ModfileObject.Platforms)
    {
        FModioAPI_ModfilePlatform ModfilePlatform = ConvertModfilePlatformObjectToModfilePlatform(ModfilePlatformObject);
        ReturnValue.Platforms.Add(ModfilePlatform);
    }

    return ReturnValue;
}

FModioAPI_Mod UModioAPIFunctionLibrary::ConvertModObjectToMod(FModioAPI_Mod_Object ModObject)
{
    FModioAPI_Mod ReturnValue;

    ReturnValue.ID = ModObject.ID;
    ReturnValue.Game_ID = ModObject.Game_ID;

    if (ModObject.Status == 0) { ReturnValue.Status = EModioAPI_GameGuideModStatus::GameStatus_NotAccepted; }
    else if (ModObject.Status == 1) { ReturnValue.Status = EModioAPI_GameGuideModStatus::GameStatus_Accepted; }
    else if (ModObject.Status == 3) { ReturnValue.Status = EModioAPI_GameGuideModStatus::GameStatus_Deleted; }

    if (ModObject.Visible == 0) { ReturnValue.Visible = EModioAPI_ModVisibility::ModVisibility_Hidden; }
    else { ReturnValue.Visible = EModioAPI_ModVisibility::ModVisibility_Public; }

    ReturnValue.Date_Added = FDateTime::FromUnixTimestamp(ModObject.Date_Added);
    ReturnValue.Date_Updated = FDateTime::FromUnixTimestamp(ModObject.Date_Updated);
    ReturnValue.Date_Live = FDateTime::FromUnixTimestamp(ModObject.Date_Live);

    // ToDo: Bitwise Field for Maturity Options
    //ReturnValue.Maturity_Options;

    // ToDo: Bitwise Field for Community Options
    //ReturnValue.Community_Options;

    // ToDo: Bitwise Field for Monetization Options
    //ReturnValue.Monetization_Options;

    ReturnValue.Price = ModObject.Price;
    ReturnValue.Tax = ModObject.Tax;
    ReturnValue.Logo = ModObject.Logo;
    ReturnValue.Homepage_URL = ModObject.Homepage_URL;
    ReturnValue.Name = ModObject.Name;
    ReturnValue.Name_ID = ModObject.Name_ID;
    ReturnValue.Summary = ModObject.Summary;
    ReturnValue.Description = ModObject.Description;
    ReturnValue.Description_Plaintext = ModObject.Description_Plaintext;
    ReturnValue.Metadata_Blob = ModObject.Metadata_Blob;
    ReturnValue.Profile_URL = ModObject.Profile_URL;
    ReturnValue.Media = ModObject.Media;
    ReturnValue.Modfile = ConvertModfileObjectToModfile(ModObject.Modfile);

    return ReturnValue;
}

FModioAPI_GetMods UModioAPIFunctionLibrary::ConvertGetModsSchemaToGetMods(FModioAPI_GetMods_Schema GetModsSchema)
{
    FModioAPI_GetMods ReturnValue;

    for (FModioAPI_Mod_Object UserEventObject : GetModsSchema.Data)
    {
        FModioAPI_Mod Mod = ConvertModObjectToMod(UserEventObject);
        ReturnValue.Data.Add(Mod);
    }

    ReturnValue.Result_Count = GetModsSchema.Result_Count;
    ReturnValue.Result_Limit = GetModsSchema.Result_Limit;
    ReturnValue.Result_Offset = GetModsSchema.Result_Offset;
    ReturnValue.Result_Total = GetModsSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_UserEvent UModioAPIFunctionLibrary::ConvertUserEventObjectToUserEvent(FModioAPI_UserEvent_Object UserEventObject)
{
    FModioAPI_UserEvent ReturnValue;

    ReturnValue.ID = UserEventObject.ID;
    ReturnValue.Game_ID = UserEventObject.Game_ID;
    ReturnValue.Mod_ID = UserEventObject.Mod_ID;
    ReturnValue.User_ID = UserEventObject.User_ID;
    ReturnValue.Date_Added = FDateTime::FromUnixTimestamp(UserEventObject.Date_Added);

    if (UserEventObject.Event_Type.Contains("JOIN")) { ReturnValue.Event_Type = EModioAPI_UserEventType::UserEventType_TeamJoin; }
    else if (UserEventObject.Event_Type.Contains("LEAVE")) { ReturnValue.Event_Type = EModioAPI_UserEventType::UserEventType_TeamLeave; }
    else if (UserEventObject.Event_Type.Contains("UNSUB")) { ReturnValue.Event_Type = EModioAPI_UserEventType::UserEventType_Unsubscribe; }
    else if (UserEventObject.Event_Type.Contains("SUB")) { ReturnValue.Event_Type = EModioAPI_UserEventType::UserEventType_Subscribe; }

    return ReturnValue;
}

FModioAPI_GetUserEvents UModioAPIFunctionLibrary::ConvertGetUserEventsSchemaToGetUserEvents(FModioAPI_GetUserEvents_Schema GetUserEventsSchema)
{
    FModioAPI_GetUserEvents ReturnValue;

    for (FModioAPI_UserEvent_Object UserEventObject : GetUserEventsSchema.Data)
    {
        FModioAPI_UserEvent UserEvent = ConvertUserEventObjectToUserEvent(UserEventObject);
        ReturnValue.Data.Add(UserEvent);
    }

    ReturnValue.Result_Count = GetUserEventsSchema.Result_Count;
    ReturnValue.Result_Limit = GetUserEventsSchema.Result_Limit;
    ReturnValue.Result_Offset = GetUserEventsSchema.Result_Offset;
    ReturnValue.Result_Total = GetUserEventsSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_RequestFilters UModioAPIFunctionLibrary::ApplyFilter(FModioAPI_RequestFilters FiltersIn, FString ColumnForFiltering, TEnumAsByte<EModioAPI_FilterComparison> Comparison, FString Value)
{
    // Apply Filter

    return FiltersIn;
}
