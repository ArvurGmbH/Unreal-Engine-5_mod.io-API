/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "ModioAPIObject.h"

bool UModioAPIObject::InitializeModioAPI(int32 GameID, FString ApiKey, bool AutomaticallyCacheAccessToken, bool StoreAccessTokenPersistingly, FString& Message)
{
	if (GameID < 0)
	{
		Message = "GameID is invalid!";
		return false;
	}

	if (ApiKey.IsEmpty())
	{
		Message = "API Key missing!";
		return false;
	}
	
	if (ApiKey.Len() != 32)
	{
		Message = "API Keys need to be 32 characters long!";
		return false;
	}

	ModioGameID = GameID;
	ModioAPI_Key = ApiKey;
	AutoCacheAccessToken = AutomaticallyCacheAccessToken;
	StoreCachedAccessTokenPersistingly = StoreAccessTokenPersistingly;

	// Try loading a cached Access Token if enabled
	if (StoresCachedAccessTokenPersistingly())
	{
		FString LoadPersistingCacheMessage;
		if (LoadPersistingCacheFromFile(LoadPersistingCacheMessage))
		{
			// loaded Persisting Cache!
		}
	}

	Message = "Connection for GameID " + FString::FromInt(GameID) + " initialized!";
	return true;
}

bool UModioAPIObject::IsInitialized()
{
	if (ModioGameID > -1 && !ModioAPI_Key.IsEmpty() && ModioAPI_Key.Len() == 32)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::IsAuthorized()
{
	if (PersistingCache.CachedAccessToken.AccessToken.Len() > 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::HasNetworkConnection()
{
	// ToDo: Properly Implement a check for this
	return true;
}

bool UModioAPIObject::CachesAccessTokenAutomatically()
{
	return AutoCacheAccessToken;
}

bool UModioAPIObject::StoresCachedAccessTokenPersistingly()
{
	return (CachesAccessTokenAutomatically() && StoreCachedAccessTokenPersistingly);
}

FString UModioAPIObject::GetApiPath()
{
	return EndpointBase + FString::FromInt(ModioGameID) + EndpointAPIPath + EndpointAPIVersion;
}

FString UModioAPIObject::GetApiKey()
{
	return EndpointAPIKey + ModioAPI_Key;
}

/*
Persisting Cache
*/

FString UModioAPIObject::GetPersistingCacheFilePath()
{
	FString PersistingCacheFilePath;

	#if PLATFORM_WINDOWS

	FString CutString;
	FString UserDocumentsDirectory;
	FString UserDirectory;
	FString UsersDirectory;
	FPaths::ConvertRelativePathToFull(FPlatformProcess::UserDir()).Split(TEXT("/"), &UserDocumentsDirectory, &CutString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	UserDocumentsDirectory.Split(TEXT("/"), &UserDirectory, &CutString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	UserDirectory.Split(TEXT("/"), &UsersDirectory, &CutString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	FString ModioDirectory = UsersDirectory + "/Public/mod.io/" + FString::FromInt(ModioGameID) + "/ModioAPI/";
	PersistingCacheFilePath = ModioDirectory + "PersistingCache.json";

	#endif

	return PersistingCacheFilePath;
}

bool UModioAPIObject::CacheAccessToken(FModioAPI_AccessToken Token, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	PersistingCache.CachedAccessToken = Token;
	Message = "Cached the Access Token in the Persisting Cache! (Persisting Cache needs to be saved!)";
	return true;
}

bool UModioAPIObject::SavePersistingCacheToFile(FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	// Convert Persisting Cache Struct to JSON Object
	TSharedPtr<FJsonObject> JsonObject = FJsonObjectConverter::UStructToJsonObject(PersistingCache);
	if (!JsonObject)
	{
		Message = "Error converting Persisting Cache to JSON Object!";
		return false;
	}

	// Serialize JSON Object to String
	FString JsonString;
	if (!FJsonSerializer::Serialize(JsonObject.ToSharedRef(), TJsonWriterFactory<>::Create(&JsonString, 0)))
	{
		Message = "Error serializing Persisting Cache JSON Object!";
		return false;
	}

	// Generate File Path for writing Persisting Cache
	FString FilePath = GetPersistingCacheFilePath();
	if (FilePath.Len() <= 0)
	{
		Message = "Error generating Path for writing Persisting Cache File!";
		return false;
	}

	// Write String to File
	if (!FFileHelper::SaveStringToFile(JsonString, *FilePath))
	{
		Message = "Error writing Persisting Cache File!";
		return false;
	}

	Message = "Persisting Cache saved successfully to File!";
	return true;
}

bool UModioAPIObject::LoadPersistingCacheFromFile(FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	// Generate File Path for reading Persisting Cache
	FString FilePath = GetPersistingCacheFilePath();
	if (FilePath.Len() <= 0)
	{
		Message = "Error generating Persisting Cache file path!";
		return false;
	}

	// Check if File exists
	if (!FPlatformFileManager::Get().GetPlatformFile().FileExists(*FilePath))
	{
		Message = "Persisting Cache file doesn't exist!";
		return false;
	}

	// Read File to String
	FString FileLoadedToString;
	if (!FFileHelper::LoadFileToString(FileLoadedToString, *FilePath))
	{
		Message = "Error reading Persisting Cache file to String!";
		return false;
	}

	// Convert String to JSON Object
	TSharedPtr<FJsonObject> JsonObject;
	if (!FJsonSerializer::Deserialize(TJsonReaderFactory<>::Create(FileLoadedToString), JsonObject))
	{
		Message = "Error converting loaded String to JSON Object!";
		return false;
	}

	// Check if Conversion was successful
	if (!JsonObject)
	{
		Message = "Json Object is empty after converting String to JSON Object!";
		return false;
	}

	// Convert JSON Object to Struct
	FModioAPI_PersistingCache LoadedPersistingCache;
	if (!FJsonObjectConverter::JsonObjectToUStruct<FModioAPI_PersistingCache>(JsonObject.ToSharedRef(), &LoadedPersistingCache))
	{
		Message = "Error converting loaded JSON Object to Persisting Cache!";
		return false;
	}

	// Set the Persisting Cache to the loaded one
	PersistingCache = LoadedPersistingCache;

	Message = "Persisting Cache loaded successfully from File!";
	return true;
}

/*
Temporary Cache
*/

bool UModioAPIObject::CacheGame(FModioAPI_Game Game, FString& Message)
{
	TempCache.CachedGame = Game;

	Message = "Game saved to Temporary Cache!";
	return true;
}

bool UModioAPIObject::CacheTermsOfService(FModioAPI_Terms TermsOfService, FString& Message)
{
	TempCache.CachedTerms = TermsOfService;

	Message = "Terms of Service saved to Temporary Cache!";
	return true;
}

bool UModioAPIObject::CacheUser(FModioAPI_User User, FString& Message)
{
	if (!TempCache.CachedUsers.Find(User.ID))
	{
		Message = "User was cached!";
		TempCache.CachedUsers.Add(User.ID, User);
	}
	else
	{
		Message = "User with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedUsers.Emplace(User.ID, User);
	}

	return true;
}

bool UModioAPIObject::CacheUserEvent(FModioAPI_UserEvent UserEvent, FString& Message)
{
	if (!TempCache.CachedUsers.Find(UserEvent.ID))
	{
		Message = "User Event was cached!";
		TempCache.CachedEvents.Add(UserEvent.ID, UserEvent);
	}
	else
	{
		Message = "User Event with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedEvents.Emplace(UserEvent.ID, UserEvent);
	}

	return true;
}

/*
Requests
*/

// Authentication

bool UModioAPIObject::RequestTermsOfService(FString& Message)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::TermsOfService_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointAuthenticate + EndpointTerms + GetApiKey());
	Request->SetVerb("GET");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Terms of Service'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Terms of Service'!";
		return false;
	}
}

bool UModioAPIObject::RequestAuthenticationSteam(FModioAPI_SteamAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationXboxLive(FModioAPI_XboxLiveAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationPlayStationNetwork(FModioAPI_PlaystationNetworkAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationNintendoSwitch(FModioAPI_NintendoSwitchAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationMetaQuest(FModioAPI_MetaQuestAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationEpicGames(FModioAPI_EpicGamesAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationGOGGalaxy(FModioAPI_GOGGalaxyAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationGoogle(FModioAPI_GoogleAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationDiscord(FModioAPI_DiscordAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAuthenticationOpenID(FModioAPI_OpenIDAuth Authentication, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestEmailSecurityCode(FString Email, FString& Message)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::EmailSecurityCode_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointOAuth + EndpointEmailRequest + GetApiKey());
	Request->SetVerb("POST");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");
	Request->SetContentAsString("email=" + Email);

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Email Security Code'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Email Security Code'!";
		return false;
	}
}

bool UModioAPIObject::RequestExchangeSecurityCodeForAccessToken(FString SecurityCode, FString& Message)
{
	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::ExchangeForAccessToken_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointOAuth + EndpointEmailExchange + GetApiKey());
	Request->SetVerb("POST");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");
	Request->SetContentAsString("security_code=" + SecurityCode);

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Exchange Security Code for Access Token'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Exchange Security Code for Access Token'!";
		return false;
	}
}

bool UModioAPIObject::RequestLogout(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Games

bool UModioAPIObject::RequestGetGame(FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetGame_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Game Info'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Game Info'!";
		return false;
	}
}

// Guides

bool UModioAPIObject::RequestGetGuides(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetGuide(int32 GuideID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddGuide(FString AccessToken, FModioAPI_AddGuide GuideToAdd, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestEditGuide(FString AccessToken, FModioAPI_EditGuide GuideEdit, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteGuide(FString AccessToken, int32 GuideID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetGuidesTags(FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Mods

bool UModioAPIObject::RequestGetMods(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetMods_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + FString::FromInt(ModioGameID) + EndpointMods + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Mods'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mods'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetMod(int32 ModID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetMod_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestAddMod(FString AccessToken, FModioAPI_AddMod Mod, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!IsAuthorized())
	{
		Message = "You need to be authorized (logged in) to Request this!";
		return false;
	}

	if (Mod.Logo.IsEmpty() || !FPlatformFileManager::Get().GetPlatformFile().FileExists(*Mod.Logo))
	{
		Message = "Logo needs to be set to an absolute path for a valid Image File!";
		return false;
	}

	if (Mod.Name.IsEmpty())
	{
		Message = "Name can't be empty!";
		return false;
	}

	if (Mod.Summary.IsEmpty())
	{
		Message = "Summary can't be empty!";
		return false;
	}

	FString ExtractionMessage;
	bool ExtractionSuccessful;
	FHttpRequestRef Request = UModioAPIFunctionLibrary::CreateRequestForAddMod(Mod, ExtractionSuccessful, ExtractionMessage);

	if (!ExtractionSuccessful)
	{
		Message = "Error when extracting the HTTP Request Body from Add Mod Struct!";
		return false;
	}

	Request->SetURL(GetApiPath() + EndpointGames + FString::FromInt(ModioGameID) + EndpointMods);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::AddMod_ResponseReceived);
	Request->SetVerb("POST");
	Request->AppendToHeader("Accept", "application/json");
	Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + PersistingCache.CachedAccessToken.AccessToken);

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestEditMod(FString AccessToken, FModioAPI_EditMod Mod, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteMod(FString AccessToken, int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Files

bool UModioAPIObject::RequestGetModfiles(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetModfile(int32 ModID, int32 ModfileID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModfile(FString AccessToken, FModioAPI_AddModfile Mod, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestEditModfile(FString AccessToken, FModioAPI_EditModfile Mod, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteModfile(FString AccessToken, int32 ModID, int32 ModfileID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestManageModfilePlatformStatus(FString AccessToken, int32 ModID, int32 ModfileID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Files (Multipart Uploads)

bool UModioAPIObject::RequestGetMultipartUploadParts(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddMultipartUploadPart(FString AccessToken, int32 ModID, int32 ModfileID, FString UploadID, int64 ContentRange, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestCreateMultipartUploadSession(FString AccessToken, int32 ModID, FString Filename, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteMultipartUploadSession(FString AccessToken, int32 ModID, FString UploadID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetMultipartUploadSessions(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestCompleteMultipartUploadSession(FString AccessToken, int32 ModID, FString UploadID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Subscribe

bool UModioAPIObject::RequestSubscribeToMod(FString AccessToken, int32 ModID, bool IncludeDependencies, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestUnsubscribeFromMod(FString AccessToken, int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Comments - Guides

bool UModioAPIObject::RequestGetGuideComments(int32 GuideID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddGuideComment(FString AccessToken, int32 GuideID, FString Content, int32 Reply_ID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetGuideComment(int32 GuideID, int32 CommentID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestUpdateGuideComment(FString AccessToken, int32 GuideID, FString CommentID, FString Content, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteGuideComment(FString AccessToken, int32 GuideID, FString CommentID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddGuideCommentKarma(FString AccessToken, int32 GuideID, FString CommentID, int32 Karma, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Comments - Mods

bool UModioAPIObject::RequestGetModComments(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModComment(FString AccessToken, int32 ModID, FString Content, int32 Reply_ID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetModComment(int32 ModID, int32 CommentID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestUpdateModComment(FString AccessToken, int32 ModID, FString CommentID, FString Content, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteModComment(FString AccessToken, int32 ModID, FString CommentID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModCommentKarma(FString AccessToken, int32 ModID, FString CommentID, int32 Karma, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Media

bool UModioAPIObject::RequestAddGameMedia(FString AccessToken, FModioAPI_AddGameMedia GameMedia, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModMedia(FString AccessToken, FModioAPI_AddModMedia ModMedia, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteModMedia(FString AccessToken, int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestReorderModMedia(FString AccessToken, int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Events

bool UModioAPIObject::RequestGetModsEvents(FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetModEvents(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Tags

bool UModioAPIObject::RequestGetGameTagOptions(FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddGameTagOptions(FString AccessToken, FModioAPI_AddGameTagOption, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteGameTagOptions(FString AccessToken, FModioAPI_DeleteGameTagOption, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestRenameGameTag(FString AccessToken, FString From, FString To, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetModTags(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModTags(FString AccessToken, int32 ModID, TArray<FString> Tags, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::DeleteModTags(FString AccessToken, int32 ModID, TArray<FString> Tags, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Rating

bool UModioAPIObject::RequestAddModRating(FString AccessToken, int32 ModID, EModioAPI_ModRating Rating, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Stats

bool UModioAPIObject::RequestGetGameStats(FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetModsStats(FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetModStats(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Metadata

bool UModioAPIObject::RequestGetModKVP_Metadata(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModKVP_Metadata(FString AccessToken, int32 ModID, TArray<FString> Metadata, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteModKVP_Metadata(FString AccessToken, int32 ModID, TArray<FString> Metadata, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Dependencies

bool UModioAPIObject::RequestGetModDependencies(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestAddModDependencies(FString AccessToken, int32 ModID, TArray<int32> Dependencies, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestDeleteModDependencies(FString AccessToken, int32 ModID, TArray<int32> Metadata, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Teams

bool UModioAPIObject::RequestGetModTeamMembers(int32 ModID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Reports

bool UModioAPIObject::RequestSubmitReport(FString AccessToken, FModioAPI_SubmitReport Report, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Agreements

bool UModioAPIObject::RequestGetCurrentAgreement(TEnumAsByte<EModioAPI_AgreementType> AgreementType, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetAgreementVersion(TEnumAsByte<EModioAPI_AgreementType> AgreementVersion, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Users

bool UModioAPIObject::RequestMuteUser(FString AccessToken, int32 UserID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestUnmuteUser(FString AccessToken, int32 UserID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Me

bool UModioAPIObject::RequestGetAuthenticatedUser(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!IsAuthorized())
	{
		Message = "You need to be authorized (logged in) to Request this!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetAuthenticatedUser_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (CachesAccessTokenAutomatically() && AccessToken.IsEmpty())
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + PersistingCache.CachedAccessToken.AccessToken);
	}
	else
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + AccessToken);
	}

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Me User Info'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Me User Info'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserEvents(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!IsAuthorized())
	{
		Message = "You need to be authorized (logged in) to Request this!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserEvents_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointEvents + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (CachesAccessTokenAutomatically() && AccessToken.IsEmpty())
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + PersistingCache.CachedAccessToken.AccessToken);
	}
	else
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + AccessToken);
	}

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Me User Info'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Me User Info'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserModfiles(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!IsAuthorized())
	{
		Message = "You need to be authorized (logged in) to Request this!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserModfiles_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointFiles + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (CachesAccessTokenAutomatically() && AccessToken.IsEmpty())
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + PersistingCache.CachedAccessToken.AccessToken);
	}
	else
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + AccessToken);
	}

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get User Modfiles'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Modfiles'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserGames(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetUserSubscriptions(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetUserMods(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetUserPurchases(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetUsersMuted(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetUserRatings(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

bool UModioAPIObject::RequestGetUserWallet(FString AccessToken, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// General

bool UModioAPIObject::RequestGetRessourceOwner(FString AccessToken, FString RessourceType, int32 RessourceID, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

// Checkout

bool UModioAPIObject::RequestPurchaseAnItem(FString AccessToken, int32 DisplayAmount, FString IdempotentKey, FString& Message)
{
	Message = "Not yet implemented!";
	return false;
}

/*
Responses
*/

// Authentication

void UModioAPIObject::TermsOfService_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		OnResponseReceived_TermsOfService.Broadcast(FModioAPI_Terms(), FModioAPI_Error_Object());
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_Terms Terms = UModioAPIFunctionLibrary::ConvertJsonObjectToTermsOfService(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		CacheTermsOfService(Terms, ConvertMessage);
		OnResponseReceived_TermsOfService.Broadcast(Terms, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_TermsOfService.Broadcast(FModioAPI_Terms(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_TermsOfService.Broadcast(FModioAPI_Terms(), FModioAPI_Error_Object());
		}
	}
}

void UModioAPIObject::SteamAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::XboxLiveAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::PlayStationNetworkAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::NintendoSwitchAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::MetaQuestAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::EpicGamesAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GOGGalaxyAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GoogleAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DiscordAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::OpenIDAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::EmailSecurityCode_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_Message_Object Message = UModioAPIFunctionLibrary::ConvertJsonObjectToMessage(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		OnResponseReceived_EmailSecurityCode.Broadcast(Message, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_EmailSecurityCode.Broadcast(FModioAPI_Message_Object(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_EmailSecurityCode.Broadcast(FModioAPI_Message_Object(), FModioAPI_Error_Object());
		}
	}
}

void UModioAPIObject::ExchangeForAccessToken_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_AccessToken_Response AccessToken = UModioAPIFunctionLibrary::ConvertJsonObjectToAccessToken(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		if (AccessToken.Code == 200)
		{
			if (CachesAccessTokenAutomatically())
			{
				bool CachingSuccess = false;
				FString CachingMessage;
				CachingSuccess = CacheAccessToken(AccessToken.AccessToken, CachingMessage);

				if (StoresCachedAccessTokenPersistingly())
				{
					SavePersistingCacheToFile(CachingMessage);
				}
			}
		}

		OnResponseReceived_ExchangeForAccessToken.Broadcast(AccessToken, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_ExchangeForAccessToken.Broadcast(FModioAPI_AccessToken_Response(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_ExchangeForAccessToken.Broadcast(FModioAPI_AccessToken_Response(), FModioAPI_Error_Object());
		}
	}

	
}

void UModioAPIObject::Logout_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Games

void UModioAPIObject::GetGame_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_Game GameInfo = UModioAPIFunctionLibrary::ConvertJsonObjectToGame(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		CacheGame(GameInfo, ConvertMessage);
		OnResponseReceived_GetGame.Broadcast(GameInfo, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_GetGame.Broadcast(FModioAPI_Game(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_GetGame.Broadcast(FModioAPI_Game(), FModioAPI_Error_Object());
		}
	}
}

// Guides

void UModioAPIObject::GetGuides_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::EditGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetGuidesTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Mods

void UModioAPIObject::GetMods_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_GetMods ModsInfo = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMods(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		OnResponseReceived_GetMods.Broadcast(ModsInfo, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_GetMods.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_GetMods.Broadcast(FModioAPI_GetMods(), FModioAPI_Error_Object());
		}
	}
}

void UModioAPIObject::GetMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_Mod Mod = UModioAPIFunctionLibrary::ConvertJsonObjectToMod(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		OnResponseReceived_GetMod.Broadcast(Mod, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_GetMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_GetMod.Broadcast(FModioAPI_Mod(), FModioAPI_Error_Object());
		}
	}
}

void UModioAPIObject::AddMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	FString Received = Response.Get()->GetContentAsString();

	Received += "_ _ _";
}

void UModioAPIObject::EditMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Files

void UModioAPIObject::GetModfiles_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::EditModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::ManageModfilePlatformStatus_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Files (Multipart Uploads)

void UModioAPIObject::GetMultipartUploadParts_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddMultipartUploadPart_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::CreateMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetMultipartUploadSessions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::CompleteMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Subscribe

void UModioAPIObject::SubscribeToMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::UnsubscribeFromMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Comments - Guides

void UModioAPIObject::GetGuideComments_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::UpdateGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddGuideCommentKarma_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Comments - Mods

void UModioAPIObject::GetModComments_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetModComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::UpdateModComments_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteModComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModCommentKarma_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Media

void UModioAPIObject::AddGameMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteModMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::ReorderModMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Events

void UModioAPIObject::GetModsEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetModEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Tags

void UModioAPIObject::GetGameTagOptions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetGameTagOption_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteGameTagOption_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::RenameGameTag_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetModTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteModTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Ratings

void UModioAPIObject::AddModRating_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Stats

void UModioAPIObject::GetGameStats_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetModsStats_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetModStats_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Metadata

void UModioAPIObject::GetModMetadataKVP_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModMetadataKVP_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteModMetadataKVP_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Dependencies

void UModioAPIObject::GetModDependencies_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::AddModDependencies_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::DeleteModDependencies_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Teams

void UModioAPIObject::GetModTeamMembers_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Reports

void UModioAPIObject::SubmitReport_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Agreements

void UModioAPIObject::GetCurrentAgreement_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetAgreementVersion_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Users

void UModioAPIObject::MuteUser_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::UnmuteUser_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

// Me

void UModioAPIObject::GetAuthenticatedUser_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_User MeUserInfo = UModioAPIFunctionLibrary::ConvertJsonObjectToUser(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		CacheUser(MeUserInfo, ConvertMessage);
		TempCache.LocalUsersID = MeUserInfo.ID;
		OnResponseReceived_GetAuthenticatedUser.Broadcast(MeUserInfo, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_GetAuthenticatedUser.Broadcast(FModioAPI_User(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_GetAuthenticatedUser.Broadcast(FModioAPI_User(), FModioAPI_Error_Object());
		}
	}
}

void UModioAPIObject::GetUserEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	if (!Response.Get())
	{
		return;
	}

	TSharedPtr<FJsonObject> ResponseObj;
	TSharedRef<TJsonReader<>> Reader = TJsonReaderFactory<>::Create(Response->GetContentAsString());
	FJsonSerializer::Deserialize(Reader, ResponseObj);

	bool ConvertSuccess = false;
	FString ConvertMessage = "";
	FModioAPI_GetUserEvents UserEvents = UModioAPIFunctionLibrary::ConvertJsonObjectToGetUserEvents(ResponseObj, ConvertSuccess, ConvertMessage);

	if (ConvertSuccess)
	{
		for (FModioAPI_UserEvent UserEvent : UserEvents.Data)
		{
			CacheUserEvent(UserEvent, ConvertMessage);
		}
		
		OnResponseReceived_GetUserEvents.Broadcast(UserEvents, FModioAPI_Error_Object());
	}
	else
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);

		if (ConvertSuccess)
		{
			OnResponseReceived_GetUserEvents.Broadcast(FModioAPI_GetUserEvents(), ErrorResponse);
		}
		else
		{
			OnResponseReceived_GetUserEvents.Broadcast(FModioAPI_GetUserEvents(), FModioAPI_Error_Object());
		}
	}
}

void UModioAPIObject::GetUserModfiles_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUserGames_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUserSubscriptions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUserMods_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUserPurchases_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUsersMuted_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUserRatings_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

void UModioAPIObject::GetUserWallet_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
}

/*
Get Cached
*/

bool UModioAPIObject::GetCached_TermsOfService(FModioAPI_Terms& TermsOfService)
{
	if (!TempCache.CachedTerms.Plaintext.IsEmpty())
	{
		TermsOfService = TempCache.CachedTerms;
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Users(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_User>& Users)
{
	if (TempCache.CachedUsers.Num() > 0)
	{
		TempCache.CachedUsers.GenerateValueArray(Users);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_User(int32 UserID, FModioAPI_User& User)
{
	if (!TempCache.CachedUsers.Find(UserID)->Name_ID.IsEmpty())
	{
		User = *TempCache.CachedUsers.Find(UserID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_UserEvents(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_UserEvent>& UserEvents)
{
	if (TempCache.CachedEvents.Num() > 0)
	{
		TempCache.CachedEvents.GenerateValueArray(UserEvents);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_UserEvent(int32 UserEventID, FModioAPI_UserEvent& UserEvent)
{
	return false;
}