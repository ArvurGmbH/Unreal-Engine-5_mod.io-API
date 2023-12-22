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
    AccessTokenResponse.AccessToken.AccessToken = AccessToken.Access_Token;
    AccessTokenResponse.AccessToken.DateExpires = FDateTime::FromUnixTimestamp(AccessToken.Date_Expires);

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

    FModioAPI_User User = ConvertUserObjectToUser(UserInfo);

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

    FModioAPI_Game Game = ConvertGameObjectToGame(GameInfo);

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

    FModioAPI_GetMods GetMods = ConvertGetModsSchemaToGetMods(GetModsSchema);

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

    FModioAPI_Mod_Object ModObject;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Mod_Object>(JsonObject.ToSharedRef(), &ModObject))
    {
        Success = false;
        Message = "Error converting JSON Object to Mod!";
        return FModioAPI_Mod();
    }

    FModioAPI_Mod Mod = ConvertModObjectToMod(ModObject);
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

    if (AddMod.Visible == EModioAPI_ModVisibility::ModVisibility_Public)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("visible", 1, BoundaryLabel)));
    }
    else if (AddMod.Visible == EModioAPI_ModVisibility::ModVisibility_Hidden)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("visible", 0, BoundaryLabel)));
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

    // ToDo: Maturity & Community Options
    /*
    if (AddMod.Maturity_Option > 0)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("maturity_option", AddMod.Maturity_Option, BoundaryLabel)));
    }

    if (AddMod.Community_Options != 1)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("community_options", AddMod.Community_Options, BoundaryLabel)));
    }
    */

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

FHttpRequestRef UModioAPIFunctionLibrary::CreateRequestForEditMod(FModioAPI_EditMod EditMod, bool& Success, FString& Message)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    FString BoundaryLabel = FString(TEXT("ModioAPI_UnrealEngine_")) + FString::FromInt(FMath::Rand());

    Request->AppendToHeader("Content-Type", "multipart/form-data; boundary=" + BoundaryLabel);

    // Payload
    TArray<uint8> RequestPayload;

    // Extract the File Name from the Path
    FString FileName;
    FString CutAway;
    FPlatformFileManager::Get().GetPlatformFile().GetFilenameOnDisk(*EditMod.Logo).Split(TEXT("/"), &CutAway, &FileName, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

    // Required Payload
    RequestPayload.Append(CreateBoundaryForImagePNG("logo", EditMod.Logo, BoundaryLabel));
    RequestPayload.Append(StringToByteArray(CreateBoundaryForString("name", EditMod.Name, BoundaryLabel)));
    RequestPayload.Append(StringToByteArray(CreateBoundaryForString("summary", EditMod.Summary, BoundaryLabel)));

    // Optional Payload
    if (!EditMod.Name_ID.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("name_id", EditMod.Name_ID, BoundaryLabel)));
    }

    if (EditMod.Visible == EModioAPI_ModVisibility::ModVisibility_Public)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("visible", 1, BoundaryLabel)));
    }
    else if (EditMod.Visible == EModioAPI_ModVisibility::ModVisibility_Hidden)
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("visible", 0, BoundaryLabel)));
    }

    if (!EditMod.Description.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("description", EditMod.Description, BoundaryLabel)));
    }

    if (!EditMod.Homepage_URL.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("homepage_url", EditMod.Homepage_URL, BoundaryLabel)));
    }

    RequestPayload.Append(StringToByteArray(CreateBoundaryForInteger("stock", EditMod.Stock, BoundaryLabel)));

    if (!EditMod.Metadata_Blob.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("metadata_blob", EditMod.Metadata_Blob, BoundaryLabel)));
    }

    // Mark end of Payload
    FString BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--"));
    RequestPayload.Append(StringToByteArray(BoundaryEnd));

    // Attach Payload to Request
    Request->SetContent(RequestPayload);

    Success = true;
    Message = "Successfully extracted Edit Mod Request Body from Edit Mod Struct!";
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

FString UModioAPIFunctionLibrary::CreateBoundaryForBoolean(FString Key, bool Value, FString BoundaryLabel)
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

TArray<uint8> UModioAPIFunctionLibrary::CreateBoundaryForZipArchive(FString Key, FString ArchiveName, TArray<uint8> ZipArchiveData, FString BoundaryLabel)
{
    TArray<uint8> BoundaryData;

    // --BoundaryLabel
    FString BoundaryBegin = FString(TEXT("--")) + BoundaryLabel + FString(TEXT("\r\n"));

    // First, we add the boundary for the logo, which is different from text payload
    FString ZipFileBoundaryString =
        FString(TEXT("\r\n"))
        + BoundaryBegin
        + FString(TEXT("Content-Disposition: form-data; name=\"" + Key + "\"; filename=\"" + ArchiveName + "\""))
        + "\r\n"
        + "Content-Type: application/zip"
        + FString(TEXT("\r\n\r\n"));

    /* ZipFileBoundaryString is:

    --BoundaryLabel
    Content-Disposition: form-data; name="<Key>"; filename="<FileName>"
    Content-Type: application/zip

    */

    // Notice, we convert all strings into uint8 format using FStringToUint8
    BoundaryData.Append(StringToByteArray(ZipFileBoundaryString));
    BoundaryData.Append(ZipArchiveData);
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

FModioAPI_GetModfiles UModioAPIFunctionLibrary::ConvertJsonObjectToGetModfiles(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetModfiles();
    }

    FModioAPI_GetModfiles_Schema GetUserModfilesSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetModfiles_Schema>(JsonObject.ToSharedRef(), &GetUserModfilesSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get User Events!";
        return FModioAPI_GetModfiles();
    }

    FModioAPI_GetModfiles GetUserModfiles = ConvertGetModfilesSchemaToGetModfiles(GetUserModfilesSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Mods!";
    return GetUserModfiles;
}

FModioAPI_GetMultipartUploadParts UModioAPIFunctionLibrary::ConvertJsonObjectToGetMultipartUploadParts(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetMultipartUploadParts();
    }

    FModioAPI_GetMultipartUploadParts_Schema GetMultipartUploadPartsSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetMultipartUploadParts_Schema>(JsonObject.ToSharedRef(), &GetMultipartUploadPartsSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get Multipart Upload Parts!";
        return FModioAPI_GetMultipartUploadParts();
    }

    FModioAPI_GetMultipartUploadParts GetMultipartUploadParts = ConvertGetMultipartUploadPartsSchemaToGetMultipartUploadParts(GetMultipartUploadPartsSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Get Multipart Upload Parts!";
    return GetMultipartUploadParts;
}

FModioAPI_GetMultipartUploadSessions UModioAPIFunctionLibrary::ConvertJsonObjectToGetMultipartUploadSessions(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetMultipartUploadSessions();
    }

    FModioAPI_GetMultipartUploadSessions_Schema GetMultipartUploadSessionsSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetMultipartUploadSessions_Schema>(JsonObject.ToSharedRef(), &GetMultipartUploadSessionsSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get Multipart Upload Sessions!";
        return FModioAPI_GetMultipartUploadSessions();
    }

    FModioAPI_GetMultipartUploadSessions GetMultipartUploadSessions = ConvertGetMultipartUploadSessionsSchemaToGetMultipartUploadSessions(GetMultipartUploadSessionsSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Get Multipart Upload Sessions!";
    return GetMultipartUploadSessions;
}

FModioAPI_GetMutedUsers UModioAPIFunctionLibrary::ConvertJsonObjectToGetMutedUsers(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetMutedUsers();
    }

    FModioAPI_GetMutedUsers_Schema GetMutedUsersSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetMutedUsers_Schema>(JsonObject.ToSharedRef(), &GetMutedUsersSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get Muted Users!";
        return FModioAPI_GetMutedUsers();
    }

    FModioAPI_GetMutedUsers GetMutedUsers = ConvertGetMutedUsersSchemaToGetMutedUsers(GetMutedUsersSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Get Muted Users!";
    return GetMutedUsers;
}

FModioAPI_GetUserRatings UModioAPIFunctionLibrary::ConvertJsonObjectToGetUserRatings(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetUserRatings();
    }

    FModioAPI_GetUserRatings_Schema GetUserRatingsSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetUserRatings_Schema>(JsonObject.ToSharedRef(), &GetUserRatingsSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get User Ratings!";
        return FModioAPI_GetUserRatings();
    }

    FModioAPI_GetUserRatings GetUserRatings = ConvertGetUserRatingsSchemaToGetUserRatings(GetUserRatingsSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Get User Ratings!";
    return GetUserRatings;
}

FModioAPI_Wallet UModioAPIFunctionLibrary::ConvertJsonObjectToWallet(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Wallet();
    }

    FModioAPI_Wallet_Object WalletObject;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Wallet_Object>(JsonObject.ToSharedRef(), &WalletObject))
    {
        Success = false;
        Message = "Error converting JSON Object to Get User Ratings!";
        return FModioAPI_Wallet();
    }

    FModioAPI_Wallet Wallet = ConvertWalletObjectToWallet(WalletObject);

    Success = true;
    Message = "Successfully converted JSON Object to Get User Ratings!";
    return Wallet;
}

FModioAPI_Modfile UModioAPIFunctionLibrary::ConvertJsonObjectToModfile(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_Modfile();
    }

    FModioAPI_Modfile_Object Modfile_Object;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_Modfile_Object>(JsonObject.ToSharedRef(), &Modfile_Object))
    {
        Success = false;
        Message = "Error converting JSON Object to Modfile Object!";
        return FModioAPI_Modfile();
    }

    FModioAPI_Modfile Modfile = ConvertModfileObjectToModfile(Modfile_Object);

    Success = true;
    Message = "Successfully converted JSON Object to Modfile!";
    return Modfile;
}

FHttpRequestRef UModioAPIFunctionLibrary::CreateRequestForAddModfile(FModioAPI_AddModfile AddModfile, bool& Success, FString& Message)
{
    FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

    FString BoundaryLabel = FString(TEXT("ModioAPI_UnrealEngine_")) + FString::FromInt(FMath::Rand());
    Request->AppendToHeader("Content-Type", "multipart/form-data; boundary=" + BoundaryLabel);
    Request->AppendToHeader("Accept", "application/json");

    // Check if there's filedata
    if (AddModfile.Filedata.IsEmpty())
    {
        if (AddModfile.UploadID.IsEmpty())
        {
            Message = "Filedata is missing and there's also no Upload Session ID provided!";
            Success = false;
            return Request;
        }
    }
    else
    {
        // Check if filedata is above ca. 500 MB (500.000.000 Bytes)
        int64 FileSize = AddModfile.Filedata.Num();
        if (FileSize > 500000000)
        {
            Message = "Filedata exceeds limit of 500 MB! Use Multipart Upload instead!";
            Success = false;
            return Request;
        }
    }

    // Payload
    TArray<uint8> RequestPayload;

    // Required Payload
    if (AddModfile.Filedata.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("upload_id", AddModfile.UploadID, BoundaryLabel)));
    }
    else
    {
        RequestPayload.Append(CreateBoundaryForZipArchive("filedata", AddModfile.ModfileName.ToString(), AddModfile.Filedata, BoundaryLabel));
    }

    RequestPayload.Append(StringToByteArray(CreateBoundaryForString("version", AddModfile.Version, BoundaryLabel)));
    RequestPayload.Append(StringToByteArray(CreateBoundaryForString("changelog", AddModfile.Changelog, BoundaryLabel)));
    RequestPayload.Append(StringToByteArray(CreateBoundaryForBoolean("active", AddModfile.Active, BoundaryLabel)));

    // Platforms
    TArray<TEnumAsByte<EModioAPI_Platforms>> TargettedPlatforms;

    for (TEnumAsByte<EModioAPI_Platforms> Platform : AddModfile.Platforms)
    {
        TargettedPlatforms.AddUnique(Platform);
    }

    if (TargettedPlatforms.Num() > 0)
    {
        for (TEnumAsByte<EModioAPI_Platforms> Platform : TargettedPlatforms)
        {
            FString PlatformCode = ConvertPlatformToPlatformCode(Platform);

            if (!PlatformCode.IsEmpty())
            {
                RequestPayload.Append(StringToByteArray(CreateBoundaryForString("platforms[]", PlatformCode, BoundaryLabel)));
            }
        }
    }

    // Filehash
    if (!AddModfile.ZipFileFilehash.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("filehash", AddModfile.ZipFileFilehash, BoundaryLabel)));
    }

    // Metadata
    if (!AddModfile.MetadataBlob.IsEmpty())
    {
        RequestPayload.Append(StringToByteArray(CreateBoundaryForString("metadata_blob", AddModfile.MetadataBlob, BoundaryLabel)));
    }

    // Mark end of Payload
    FString BoundaryEnd = FString(TEXT("\r\n--")) + BoundaryLabel + FString(TEXT("--"));
    RequestPayload.Append(StringToByteArray(BoundaryEnd));

    // Attach Payload to Request
    Request->SetContent(RequestPayload);

    Success = true;
    Message = "Successfully extracted Add Modfile Request Body from Add Modfile Struct!";;
    return Request;
}

FModioAPI_MultipartUpload UModioAPIFunctionLibrary::ConvertJsonObjectToMultipartUpload(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_MultipartUpload();
    }

    FModioAPI_MultipartUpload_Object MultipartUpload_Object;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_MultipartUpload_Object>(JsonObject.ToSharedRef(), &MultipartUpload_Object))
    {
        Success = false;
        Message = "Error converting JSON Object to Multipart Upload Object!";
        return FModioAPI_MultipartUpload();
    }

    FModioAPI_MultipartUpload MultipartUpload = ConvertMultipartUploadObjectToMultipartUpload(MultipartUpload_Object);

    Success = true;
    Message = "Successfully converted JSON Object to Multipart Upload!";
    return MultipartUpload;
}

FModioAPI_MultipartUploadPart UModioAPIFunctionLibrary::ConvertJsonObjectToMultipartUploadPart(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_MultipartUploadPart();
    }

    FModioAPI_MultipartUploadPart_Object MultipartUploadPart_Object;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_MultipartUploadPart_Object>(JsonObject.ToSharedRef(), &MultipartUploadPart_Object))
    {
        Success = false;
        Message = "Error converting JSON Object to Multipart Upload Object!";
        return FModioAPI_MultipartUploadPart();
    }

    FModioAPI_MultipartUploadPart MultipartUploadPart = ConvertMultipartUploadPartObjectToMultipartUploadPart(MultipartUploadPart_Object);

    Success = true;
    Message = "Successfully converted JSON Object to Multipart Upload!";
    return MultipartUploadPart;
}

FModioAPI_GetGameTagOptions UModioAPIFunctionLibrary::ConvertJsonObjectToGetGameTagOptions(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetGameTagOptions();
    }

    FModioAPI_GetGameTagOptions GetGameTagsOptions;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetGameTagOptions>(JsonObject.ToSharedRef(), &GetGameTagsOptions))
    {
        Success = false;
        Message = "Error converting JSON Object to Get Game Tag Options!";
        return FModioAPI_GetGameTagOptions();
    }

    Success = true;
    Message = "Successfully converted JSON Object to Get Game Tag Options!";
    return GetGameTagsOptions;
}

FModioAPI_GetModTags UModioAPIFunctionLibrary::ConvertJsonObjectToGetModTags(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetModTags();
    }

    FModioAPI_GetModTags_Schema GetModTagsSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetModTags_Schema>(JsonObject.ToSharedRef(), &GetModTagsSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Get Mod Tags!";
        return FModioAPI_GetModTags();
    }

    FModioAPI_GetModTags GetModTags;
    GetModTags.Result_Count = GetModTagsSchema.Result_Count;
    GetModTags.Result_Limit = GetModTagsSchema.Result_Limit;
    GetModTags.Result_Offset = GetModTagsSchema.Result_Offset;
    GetModTags.Result_Total = GetModTagsSchema.Result_Total;
    
    FModioAPI_ModTag ModTag;
    for (FModioAPI_ModTag_Object Object : GetModTagsSchema.Data)
    {
        ModTag.Name = Object.Name;
        ModTag.Date_Added = FDateTime::FromUnixTimestamp(Object.Date_Added);
        GetModTags.Data.Add(ModTag);
    }

    Success = true;
    Message = "Successfully converted JSON Object to Get Mod Tags!";
    return GetModTags;
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
    ReturnValue.Dependencies = ModObject.Dependencies;
    ReturnValue.Stats = ConvertModStatsObjectToModStats(ModObject.Stats);
    ReturnValue.Platforms = ModObject.Platforms;
    ReturnValue.Metadata_KVP = ModObject.Metadata_KVP;
    ReturnValue.Tags = ConvertModTagObjectsToModTags(ModObject.Tags);

    return ReturnValue;
}

FModioAPI_User UModioAPIFunctionLibrary::ConvertUserObjectToUser(FModioAPI_User_Object UserObject)
{
    FModioAPI_User User;

    User.ID = UserObject.ID;
    User.Name_ID = UserObject.Name_ID;
    User.Username = UserObject.Username;
    User.Display_Name_Portal = UserObject.Display_Name_Portal;
    User.Date_Online = FDateTime::FromUnixTimestamp(UserObject.Date_Online);
    User.Date_Joined = FDateTime::FromUnixTimestamp(UserObject.Date_Joined);
    User.Avatar = UserObject.Avatar;
    User.Timezone = UserObject.Timezone;
    User.Language = UserObject.Language;
    User.Profile_URL = UserObject.Profile_URL;

    return User;
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

FModioAPI_GetModfiles UModioAPIFunctionLibrary::ConvertGetModfilesSchemaToGetModfiles(FModioAPI_GetModfiles_Schema GetUserModfilesSchema)
{
    FModioAPI_GetModfiles ReturnValue;

    for (FModioAPI_Modfile_Object UserModfileObject : GetUserModfilesSchema.Data)
    {
        FModioAPI_Modfile UserModfile = ConvertModfileObjectToModfile(UserModfileObject);
        ReturnValue.Data.Add(UserModfile);
    }

    ReturnValue.Result_Count = GetUserModfilesSchema.Result_Count;
    ReturnValue.Result_Limit = GetUserModfilesSchema.Result_Limit;
    ReturnValue.Result_Offset = GetUserModfilesSchema.Result_Offset;
    ReturnValue.Result_Total = GetUserModfilesSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_MultipartUploadPart UModioAPIFunctionLibrary::ConvertMultipartUploadPartObjectToMultipartUploadPart(FModioAPI_MultipartUploadPart_Object MultipartUploadPartObject)
{
    FModioAPI_MultipartUploadPart ReturnValue;

    ReturnValue.Upload_ID = MultipartUploadPartObject.Upload_ID;
    ReturnValue.Part_Number = MultipartUploadPartObject.Part_Number;
    ReturnValue.Part_Size = MultipartUploadPartObject.Part_Size;
    ReturnValue.Date_Added = FDateTime::FromUnixTimestamp(MultipartUploadPartObject.Date_Added);

    return ReturnValue;
}

FModioAPI_GetMultipartUploadParts UModioAPIFunctionLibrary::ConvertGetMultipartUploadPartsSchemaToGetMultipartUploadParts(FModioAPI_GetMultipartUploadParts_Schema GetMultipartUploadPartsSchema)
{
    FModioAPI_GetMultipartUploadParts ReturnValue;

    for (FModioAPI_MultipartUploadPart_Object MultipartUploadPartObject : GetMultipartUploadPartsSchema.Data)
    {
        FModioAPI_MultipartUploadPart MultipartUploadPart = ConvertMultipartUploadPartObjectToMultipartUploadPart(MultipartUploadPartObject);
        ReturnValue.Data.Add(MultipartUploadPart);
    }

    ReturnValue.Result_Count = GetMultipartUploadPartsSchema.Result_Count;
    ReturnValue.Result_Limit = GetMultipartUploadPartsSchema.Result_Limit;
    ReturnValue.Result_Offset = GetMultipartUploadPartsSchema.Result_Offset;
    ReturnValue.Result_Total = GetMultipartUploadPartsSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_GetMultipartUploadSessions UModioAPIFunctionLibrary::ConvertGetMultipartUploadSessionsSchemaToGetMultipartUploadSessions(FModioAPI_GetMultipartUploadSessions_Schema GetMultipartUploadSessionsSchema)
{
    FModioAPI_GetMultipartUploadSessions ReturnValue;

    for (FModioAPI_MultipartUpload_Object MultipartUploadObject : GetMultipartUploadSessionsSchema.Data)
    {
        FModioAPI_MultipartUpload MultipartUpload = ConvertMultipartUploadObjectToMultipartUpload(MultipartUploadObject);
        ReturnValue.Data.Add(MultipartUpload);
    }

    ReturnValue.Result_Count = GetMultipartUploadSessionsSchema.Result_Count;
    ReturnValue.Result_Limit = GetMultipartUploadSessionsSchema.Result_Limit;
    ReturnValue.Result_Offset = GetMultipartUploadSessionsSchema.Result_Offset;
    ReturnValue.Result_Total = GetMultipartUploadSessionsSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_Game UModioAPIFunctionLibrary::ConvertGameObjectToGame(FModioAPI_Game_Object GameObject)
{
    FModioAPI_Game Game;
    Game.ID = GameObject.ID;

    // Game Status
    if (GameObject.Status == 0) { Game.Status = EModioAPI_GameGuideModStatus::GameStatus_NotAccepted; }
    else if (GameObject.Status == 1) { Game.Status = EModioAPI_GameGuideModStatus::GameStatus_Accepted; }
    else if (GameObject.Status == 3) { Game.Status = EModioAPI_GameGuideModStatus::GameStatus_Deleted; }

    Game.Date_Added = FDateTime::FromUnixTimestamp(GameObject.Date_Added);
    Game.Date_Updated = FDateTime::FromUnixTimestamp(GameObject.Date_Updated);
    Game.Date_Live = FDateTime::FromUnixTimestamp(GameObject.Date_Live);

    // Game Presentation Option
    if (GameObject.Presentation_Option == 0) { Game.Presentation_Option = EModioAPI_GamePresentation::GamePresentation_GridView; }
    else if (GameObject.Presentation_Option == 1) { Game.Presentation_Option = EModioAPI_GamePresentation::GamePresentation_TableView; }

    // Game Mods Submission Option
    if (GameObject.Submission_Option == 0) { Game.Submission_Option = EModioAPI_GameModsSubmission::GameModsSubmission_ToolsAPIOnly; }
    else if (GameObject.Submission_Option == 1) { Game.Submission_Option = EModioAPI_GameModsSubmission::GameModsSubmission_Anywhere; }

    // Game Mods Curation
    if (GameObject.Curation_Option == 0) { Game.Curation_Option = EModioAPI_GameModsCuration::GameModsCuration_NoCuration; }
    else if (GameObject.Curation_Option == 1) { Game.Curation_Option = EModioAPI_GameModsCuration::GameModsCuration_PriceChanges; }
    else if (GameObject.Curation_Option == 2) { Game.Curation_Option = EModioAPI_GameModsCuration::GameModsCuration_FullCuration; }

    // ToDo: Extract Community Options...

    // Options here!

    Game.UGC_Name = GameObject.UGC_Name;
    Game.Token_Name = GameObject.Token_Name;
    Game.Icon = GameObject.Icon;
    Game.Logo = GameObject.Logo;
    Game.Header = GameObject.Header;
    Game.Name = GameObject.Name;
    Game.Name_ID = GameObject.Name_ID;
    Game.Summary = GameObject.Summary;
    Game.Instructions = GameObject.Instructions;
    Game.Instructions_URL = GameObject.Instructions_URL;
    Game.Profile_URL = GameObject.Profile_URL;

    // Game Stats
    Game.Stats.Game_ID = GameObject.Stats.Game_ID;
    Game.Stats.Mods_Count_Total = GameObject.Stats.Mods_Count_Total;
    Game.Stats.Mods_Downloads_Today = GameObject.Stats.Mods_Downloads_Today;
    Game.Stats.Mods_Downloads_Total = GameObject.Stats.Mods_Downloads_Total;
    Game.Stats.Mods_Downloads_Daily_Average = GameObject.Stats.Mods_Downloads_Daily_Average;
    Game.Stats.Mods_Subscribers_Total = GameObject.Stats.Mods_Subscribers_Total;
    Game.Stats.Date_Expires = FDateTime::FromUnixTimestamp(GameObject.Stats.Date_Expires);

    Game.Theme = GameObject.Theme;
    Game.Other_URLs = GameObject.Other_URLs;
    Game.Tag_Options = GameObject.Tag_Options;
    Game.Platforms = GameObject.Platforms;

    return Game;
}

FModioAPI_GetGames UModioAPIFunctionLibrary::ConvertGetGamesSchemaToGetGames(FModioAPI_GetGames_Schema GetGamesSchema)
{
    FModioAPI_GetGames ReturnValue;

    for (FModioAPI_Game_Object GameObject : GetGamesSchema.Data)
    {
        FModioAPI_Game Game = ConvertGameObjectToGame(GameObject);
        ReturnValue.Data.Add(Game);
    }

    ReturnValue.Result_Count = GetGamesSchema.Result_Count;
    ReturnValue.Result_Limit = GetGamesSchema.Result_Limit;
    ReturnValue.Result_Offset = GetGamesSchema.Result_Offset;
    ReturnValue.Result_Total = GetGamesSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_GetGames UModioAPIFunctionLibrary::ConvertJsonObjectToGetGames(TSharedPtr<FJsonObject> JsonObject, bool& Success, FString& Message)
{
    if (!JsonObject)
    {
        Success = false;
        Message = "No Json Object for Conversion!";
        return FModioAPI_GetGames();
    }

    FModioAPI_GetGames_Schema GetGamesSchema;
    if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_GetGames_Schema>(JsonObject.ToSharedRef(), &GetGamesSchema))
    {
        Success = false;
        Message = "Error converting JSON Object to Mods!";
        return FModioAPI_GetGames();
    }

    FModioAPI_GetGames GetGames = ConvertGetGamesSchemaToGetGames(GetGamesSchema);

    Success = true;
    Message = "Successfully converted JSON Object to Mods!";
    return GetGames;
}

FModioAPI_GetMutedUsers UModioAPIFunctionLibrary::ConvertGetMutedUsersSchemaToGetMutedUsers(FModioAPI_GetMutedUsers_Schema GetMutedUsersSchema)
{
    FModioAPI_GetMutedUsers ReturnValue;

    for (FModioAPI_User_Object UserObject : GetMutedUsersSchema.Data)
    {
        FModioAPI_User User = ConvertUserObjectToUser(UserObject);
        ReturnValue.Data.Add(User);
    }

    ReturnValue.Result_Count = GetMutedUsersSchema.Result_Count;
    ReturnValue.Result_Limit = GetMutedUsersSchema.Result_Limit;
    ReturnValue.Result_Offset = GetMutedUsersSchema.Result_Offset;
    ReturnValue.Result_Total = GetMutedUsersSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_Rating UModioAPIFunctionLibrary::ConvertRatingObjectToRating(FModioAPI_Rating_Object RatingObject)
{
    FModioAPI_Rating ReturnValue;

    ReturnValue.Game_ID = RatingObject.Game_ID;
    ReturnValue.Mod_ID = RatingObject.Mod_ID;
    
    if (RatingObject.Rating < 0) { ReturnValue.Rating = EModioAPI_RatingType::RatingType_Negative; }
    else if (RatingObject.Rating > 0) { ReturnValue.Rating = EModioAPI_RatingType::RatingType_Positive; }

    ReturnValue.Date_Added = FDateTime::FromUnixTimestamp(RatingObject.Date_Added);

    return ReturnValue;
}

FModioAPI_GetUserRatings UModioAPIFunctionLibrary::ConvertGetUserRatingsSchemaToGetUserRatings(FModioAPI_GetUserRatings_Schema GetUserRatingsSchema)
{
    FModioAPI_GetUserRatings ReturnValue;

    for (FModioAPI_Rating_Object RatingObject : GetUserRatingsSchema.Data)
    {
        FModioAPI_Rating Rating = ConvertRatingObjectToRating(RatingObject);
        ReturnValue.Data.Add(Rating);
    }

    ReturnValue.Result_Count = GetUserRatingsSchema.Result_Count;
    ReturnValue.Result_Limit = GetUserRatingsSchema.Result_Limit;
    ReturnValue.Result_Offset = GetUserRatingsSchema.Result_Offset;
    ReturnValue.Result_Total = GetUserRatingsSchema.Result_Total;

    return ReturnValue;
}

FModioAPI_Wallet UModioAPIFunctionLibrary::ConvertWalletObjectToWallet(FModioAPI_Wallet_Object WalletObject)
{
    FModioAPI_Wallet Wallet;

    Wallet.Type = WalletObject.Type;
    Wallet.Payment_Method_ID = WalletObject.Payment_Method_ID;
    Wallet.Currency = WalletObject.Currency;
    Wallet.Balance = WalletObject.Balance;

    return Wallet;
}

FModioAPI_MultipartUpload UModioAPIFunctionLibrary::ConvertMultipartUploadObjectToMultipartUpload(FModioAPI_MultipartUpload_Object MultipartUploadObject)
{
    FModioAPI_MultipartUpload MultipartUpload;

    MultipartUpload.Upload_ID = MultipartUploadObject.Upload_ID;

    if (MultipartUploadObject.Status == 0) { MultipartUpload.Status = EModioAPI_MultipartUploadStatus::MultipartUploadStatus_Incomplete; }
    else if (MultipartUploadObject.Status == 1) { MultipartUpload.Status = EModioAPI_MultipartUploadStatus::MultipartUploadStatus_Pending; }
    else if (MultipartUploadObject.Status == 2) { MultipartUpload.Status = EModioAPI_MultipartUploadStatus::MultipartUploadStatus_Processing; }
    else if (MultipartUploadObject.Status == 3) { MultipartUpload.Status = EModioAPI_MultipartUploadStatus::MultipartUploadStatus_Complete; }
    else if (MultipartUploadObject.Status == 4) { MultipartUpload.Status = EModioAPI_MultipartUploadStatus::MultipartUploadStatus_Cancelled; }

    return MultipartUpload;
}

int32 UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(FString RequestURL, FString EndpointMods)
{
    FString WorkingOn = RequestURL;
    FString LeftSplit;
    FString RightSplit;

    // Split ModID from rest of the URL
    WorkingOn.Split(EndpointMods, &LeftSplit, &RightSplit, ESearchCase::CaseSensitive, ESearchDir::FromEnd);
    WorkingOn = RightSplit.TrimChar('/', nullptr);

    FString Output = WorkingOn;

    if (WorkingOn.Contains("/"))
    {
        // Find Slash after ModID
        int32 SlashIndex;
        WorkingOn.FindChar('/', SlashIndex);

        Output = WorkingOn.Left(SlashIndex);
    }

    if (WorkingOn.Contains("?"))
    {
        // Find Question Mark after ModID
        int32 QuestionMarkIndex;
        WorkingOn.FindChar('?', QuestionMarkIndex);

        Output = WorkingOn.Left(QuestionMarkIndex);
    }

    return FCString::Atoi(*Output);
}

FModioAPI_ModStats UModioAPIFunctionLibrary::ConvertModStatsObjectToModStats(FModioAPI_ModStats_Object ModstatsObject)
{
    FModioAPI_ModStats Stats = FModioAPI_ModStats();
    Stats.Mod_ID = ModstatsObject.Mod_ID;
    Stats.Popularity_Rank_Position = ModstatsObject.Popularity_Rank_Position;
    Stats.Popularity_Rank_Total_Mods = ModstatsObject.Popularity_Rank_Total_Mods;
    Stats.Downloads_Today = ModstatsObject.Downloads_Today;
    Stats.Downloads_Total = ModstatsObject.Downloads_Total;
    Stats.Subscribers_Total = ModstatsObject.Subscribers_Total;
    Stats.Ratings_Total = ModstatsObject.Ratings_Total;
    Stats.Ratings_Positive = ModstatsObject.Ratings_Positive;
    Stats.Ratings_Negative = ModstatsObject.Ratings_Negative;
    Stats.Ratings_Percentage_Positive = ModstatsObject.Ratings_Percentage_Positive;
    Stats.Ratings_Percentage_Negative = ModstatsObject.Ratings_Percentage_Negative;
    Stats.Ratings_Weighted_Aggregate = ModstatsObject.Ratings_Weighted_Aggregate;
    Stats.Ratings_Display_Text = ModstatsObject.Ratings_Display_Text;
    Stats.Date_Expires = FDateTime::FromUnixTimestamp(ModstatsObject.Date_Expires);

    return Stats;
}

FModioAPI_ModTag UModioAPIFunctionLibrary::ConvertModTagObjectToModTag(FModioAPI_ModTag_Object ModTagObject)
{
    FModioAPI_ModTag Tag = FModioAPI_ModTag();
    Tag.Name = ModTagObject.Name;
    Tag.Date_Added = FDateTime::FromUnixTimestamp(ModTagObject.Date_Added);
    return Tag;
}

TArray<FModioAPI_ModTag> UModioAPIFunctionLibrary::ConvertModTagObjectsToModTags(TArray<FModioAPI_ModTag_Object> ModTagObjects)
{
    TArray<FModioAPI_ModTag> Tags;

    for (FModioAPI_ModTag_Object TagObject : ModTagObjects)
    {
        Tags.Add(ConvertModTagObjectToModTag(TagObject));
    }

    return Tags;
}

FString UModioAPIFunctionLibrary::ConvertPlatformToPlatformCode(TEnumAsByte<EModioAPI_Platforms> Platform)
{
    switch (Platform)
    {
        case EModioAPI_Platforms::Platforms_Source:
            return "source";
        case EModioAPI_Platforms::Platforms_Windows:
            return "windows";
        case EModioAPI_Platforms::Platforms_Mac:
            return "mac";
        case EModioAPI_Platforms::Platforms_Linux:
            return "linux";
        case EModioAPI_Platforms::Platforms_Android:
            return "android";
        case EModioAPI_Platforms::Platforms_iOS:
            return "ios";
        case EModioAPI_Platforms::Platforms_XboxOne:
            return "xboxone";
        case EModioAPI_Platforms::Platforms_XboxSeriesX:
            return "xboxseriesx";
        case EModioAPI_Platforms::Platforms_PlayStation4:
            return "ps4";
        case EModioAPI_Platforms::Platforms_PlayStation5:
            return "ps5";
        case EModioAPI_Platforms::Platforms_Switch:
            return "switch";
        case EModioAPI_Platforms::Platforms_Oculus:
            return "oculus";
        default:
            return "";
    }
}

TSharedPtr<FJsonObject> UModioAPIFunctionLibrary::ConvertResponseToJsonObject(FHttpResponsePtr Response)
{
    TSharedPtr<FJsonObject> ResponseObj;
    TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
    FJsonSerializer::Deserialize(Reader, ResponseObj);

    return ResponseObj;
}

FModioAPI_RequestFilters UModioAPIFunctionLibrary::ApplyFilter(FModioAPI_RequestFilters FiltersIn, FString ColumnForFiltering, TEnumAsByte<EModioAPI_FilterComparison> Comparison, FString Value)
{
    // Apply Filter

    return FiltersIn;
}

FString UModioAPIFunctionLibrary::SanitizeStringForHTTP_Request(FString String)
{
    FString StringWorkingOn = String;

    FString From = TEXT("\r\n");
    FString To = "%0D%0A";
    int32 IndexToReplace;

    while (StringWorkingOn.Contains(From))
    {
        IndexToReplace = String.Find(From);
        StringWorkingOn = StringWorkingOn.Replace(*From, *To, ESearchCase::IgnoreCase);
    }

    return StringWorkingOn;
}

FString UModioAPIFunctionLibrary::ConvertIntToZipFilePartNumber(int32 PartNumber)
{
    if (PartNumber > 99)
    {
        return FString::FromInt(PartNumber);
    }
    else if (PartNumber > 9)
    {
        return "0" + FString::FromInt(PartNumber);
    }
    else
    {
        return "00" + FString::FromInt(PartNumber);
    }
}

FString UModioAPIFunctionLibrary::FindUploadPartFileByPartNumber(TArray<FString> UploadPartFiles, int32 PartNumber)
{
    FString FileNameEnding = ConvertIntToZipFilePartNumber(PartNumber);

    for (int i = 0; i < UploadPartFiles.Num(); i++)
    {
        if (UploadPartFiles[i].EndsWith(FileNameEnding))
        {
            return UploadPartFiles[i];
        }
    }

    return FString();
}

int64 UModioAPIFunctionLibrary::GetContentRangeStartForUploadPartNumber(int32 PartNumber)
{
    if (PartNumber < 1)
    {
        return -1;
    }

    return (PartNumber - 1) * 52428800;
}

int64 UModioAPIFunctionLibrary::GetContentRangeEndForUploadSessionOnPartNumber(TArray<FString> UploadPartFiles, int32 PartNumber)
{
    int64 ContentRangeEnd = -1;

    if (PartNumber < 1)
    {
        return -1;
    }

    // Is this the last Part?
    if (PartNumber == UploadPartFiles.Num())
    {
        FString File = FindUploadPartFileByPartNumber(UploadPartFiles, PartNumber);

        if (!File.IsEmpty())
        {
            // Content Range End = Start of the Part Number + File Size of that last Part
            ContentRangeEnd = GetContentRangeStartForUploadPartNumber(PartNumber) + IFileManager::Get().FileSize(*File);
            ContentRangeEnd--;
        }
    }
    else
    {
        ContentRangeEnd = (PartNumber * 52428800) - 1;
    }

    return ContentRangeEnd;
}

bool UModioAPIFunctionLibrary::ModIncludesTag(FModioAPI_Mod Mod, FString Tag)
{
    for (FModioAPI_ModTag ModTag : Mod.Tags)
    {
        if (ModTag.Name == Tag)
        {
            return true;
        }
    }

    return false;
}

bool UModioAPIFunctionLibrary::ModIncludesPlatform(FModioAPI_Mod Mod, TEnumAsByte<EModioAPI_Platforms> Platform)
{
    for (FModioAPI_ModPlatforms ModPlatform : Mod.Platforms)
    {
        if (ModPlatform.Platform == ConvertPlatformToPlatformCode(Platform))
        {
            return true;
        }
    }

    return false;
}

bool UModioAPIFunctionLibrary::ModHasMetadataKVP(FModioAPI_Mod Mod, FString Key, FString Value)
{
    for (FModioAPI_MetadataKVP_Object KVP : Mod.Metadata_KVP)
    {
        if (KVP.Metakey == Key)
        {
            if (KVP.Metavalue == Value)
            {
                return true;
            }
        }
    }

    return false;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByName(TArray<FModioAPI_Mod> Mods, FString Name, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheFilterMode::CacheFilterMode_Contains:
            if (Mod.Name.Contains(Name))
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheFilterMode::CacheFilterMode_ContainsNot:
            if (!Mod.Name.Contains(Name))
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsBySubmitter(TArray<FModioAPI_Mod> Mods, FString SubmittedBy, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheFilterMode::CacheFilterMode_Contains:
            if (Mod.Submitted_By.Username.Contains(SubmittedBy))
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheFilterMode::CacheFilterMode_ContainsNot:
            if (!Mod.Submitted_By.Username.Contains(SubmittedBy))
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByVisibility(TArray<FModioAPI_Mod> Mods, TEnumAsByte<EModioAPI_ModVisibility> Visibility, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheFilterMode::CacheFilterMode_Contains:
            if (Mod.Visible == Visibility)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheFilterMode::CacheFilterMode_ContainsNot:
            if (Mod.Visible != Visibility)
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByDateAdded(TArray<FModioAPI_Mod> Mods, FDateTime DateTime, TEnumAsByte<EModioAPI_CacheTimeFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
            case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_Same:
            if (Mod.Date_Added == DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_SameOrLater:
            if (Mod.Date_Added >= DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_Later:
            if (Mod.Date_Added > DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_SameOrBefore:
            if (Mod.Date_Added <= DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_BeforeOnly:
            if (Mod.Date_Added < DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByDateLive(TArray<FModioAPI_Mod> Mods, FDateTime DateTime, TEnumAsByte<EModioAPI_CacheTimeFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_Same:
            if (Mod.Date_Live == DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_SameOrLater:
            if (Mod.Date_Live >= DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_Later:
            if (Mod.Date_Live > DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_SameOrBefore:
            if (Mod.Date_Live <= DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_BeforeOnly:
            if (Mod.Date_Live < DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByDateUpdated(TArray<FModioAPI_Mod> Mods, FDateTime DateTime, TEnumAsByte<EModioAPI_CacheTimeFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_Same:
            if (Mod.Date_Updated == DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_SameOrLater:
            if (Mod.Date_Updated >= DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_Later:
            if (Mod.Date_Updated > DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_SameOrBefore:
            if (Mod.Date_Updated <= DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheTimeFilterMode::CacheTimeFilterMode_BeforeOnly:
            if (Mod.Date_Updated < DateTime)
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByTag(TArray<FModioAPI_Mod> Mods, FString Tag, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheFilterMode::CacheFilterMode_Contains:
            if (ModIncludesTag(Mod, Tag))
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheFilterMode::CacheFilterMode_ContainsNot:
            if (!ModIncludesTag(Mod, Tag))
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByPlatform(TArray<FModioAPI_Mod> Mods, TEnumAsByte<EModioAPI_Platforms> Platform, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheFilterMode::CacheFilterMode_Contains:
            if (ModIncludesPlatform(Mod, Platform))
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheFilterMode::CacheFilterMode_ContainsNot:
            if (!ModIncludesPlatform(Mod, Platform))
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::FilterModsByMetadataKVP(TArray<FModioAPI_Mod> Mods, FString Key, FString Value, TEnumAsByte<EModioAPI_CacheFilterMode> FilterMode)
{
    TArray<FModioAPI_Mod> Filtered;

    for (FModioAPI_Mod Mod : Mods)
    {
        switch (FilterMode)
        {
        case EModioAPI_CacheFilterMode::CacheFilterMode_Contains:
            if (ModHasMetadataKVP(Mod, Key, Value))
            {
                Filtered.Add(Mod);
            }
            break;
        case EModioAPI_CacheFilterMode::CacheFilterMode_ContainsNot:
            if (!ModHasMetadataKVP(Mod, Key, Value))
            {
                Filtered.Add(Mod);
            }
            break;
        }
    }

    return Filtered;
}

TArray<FModioAPI_Mod> UModioAPIFunctionLibrary::SortModsByID(TArray<FModioAPI_Mod> Mods, TEnumAsByte<EModioAPI_SortingOrder> Order)
{
    // TODO
    /*
    TArray<FModioAPI_Mod> Sorted;

    for (FModioAPI_Mod Mod : Mods)
    {
        
    }

    return Sorted;
    */

    return Mods;
}
