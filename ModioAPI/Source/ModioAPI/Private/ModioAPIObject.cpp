/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "ModioAPIObject.h"

FModioAPI_AccessToken UModioAPIObject::GetPersistingCacheAccessToken()
{
	return PersistingCache.CachedAccessToken;
}

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

FString UModioAPIObject::GetModioRootDirectory()
{
	FString ModioRootPath;

	#if PLATFORM_WINDOWS

	FString CutString;
	FString UserDocumentsDirectory;
	FString UserDirectory;
	FString UsersDirectory;
	FPaths::ConvertRelativePathToFull(FPlatformProcess::UserDir()).Split(TEXT("/"), &UserDocumentsDirectory, &CutString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	UserDocumentsDirectory.Split(TEXT("/"), &UserDirectory, &CutString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);
	UserDirectory.Split(TEXT("/"), &UsersDirectory, &CutString, ESearchCase::IgnoreCase, ESearchDir::FromEnd);

	ModioRootPath = UsersDirectory + "/Public/mod.io/";

	if (!IFileManager::Get().DirectoryExists(*ModioRootPath))
	{
		IFileManager::Get().MakeDirectory(*ModioRootPath, true);
	}

	#endif

	#if PLATFORM_ANDROID

	ModioRootPath = FPaths::ConvertRelativePathToFull(FPaths::ProjectPersistentDownloadDir());
	if (ModioRootPath.EndsWith("/"))
	{
		ModioRootPath += "mod.io/";
	}
	else
	{
		ModioRootPath += "/mod.io/";
	}

	#endif

	return ModioRootPath;
}

FString UModioAPIObject::GetModioGameDirectory()
{
	FString ModioGamePath = GetModioRootDirectory();

	ModioGamePath.Append(FString::FromInt(ModioGameID) + "/ModioAPI/");

	if (!IFileManager::Get().DirectoryExists(*ModioGamePath))
	{
		IFileManager::Get().MakeDirectory(*ModioGamePath, true);
	}

	return ModioGamePath;
}

FString UModioAPIObject::GetPersistingCacheFilePath()
{
	FString PersistingCacheFilePath = GetModioGameDirectory();

	PersistingCacheFilePath.Append("PersistingCache.json");

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

	// Delete File if it already exists!
	if (FPaths::FileExists(FilePath))
	{
		IFileManager::Get().Delete(*FilePath, true);
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

FString UModioAPIObject::GetUsersDirectoryPath()
{
	FString ModioUsersPath = GetModioGameDirectory();

	ModioUsersPath.Append("Users/");

	if (!IFileManager::Get().DirectoryExists(*ModioUsersPath))
	{
		IFileManager::Get().MakeDirectory(*ModioUsersPath, true);
	}

	return ModioUsersPath;
}

FString UModioAPIObject::GetAvatarDirectoryPathForUser(int32 UserID)
{
	FString ModioUserDirectoryPath = GetUsersDirectoryPath();

	ModioUserDirectoryPath.Append(FString::FromInt(UserID) + "/");

	if (!IFileManager::Get().DirectoryExists(*ModioUserDirectoryPath))
	{
		IFileManager::Get().MakeDirectory(*ModioUserDirectoryPath, true);
	}

	return ModioUserDirectoryPath;
}

FString UModioAPIObject::GetMediaDirectoryPathForGame()
{
	FString MediaDirectoryForGame = GetModioGameDirectory();

	MediaDirectoryForGame.Append("Media/");

	if (!IFileManager::Get().DirectoryExists(*MediaDirectoryForGame))
	{
		IFileManager::Get().MakeDirectory(*MediaDirectoryForGame, true);
	}

	return MediaDirectoryForGame;
}

FString UModioAPIObject::GetModsDirectoryPath()
{
	FString ModsDirectoryForGame = GetModioGameDirectory();

	ModsDirectoryForGame.Append("Mods/");

	if (!IFileManager::Get().DirectoryExists(*ModsDirectoryForGame))
	{
		IFileManager::Get().MakeDirectory(*ModsDirectoryForGame, true);
	}

	return ModsDirectoryForGame;
}

FString UModioAPIObject::GetDirectoryPathForMod(int32 ModID)
{
	FString ModDirectory = GetModsDirectoryPath();

	ModDirectory.Append(FString::FromInt(ModID) + "/");

	if (!IFileManager::Get().DirectoryExists(*ModDirectory))
	{
		IFileManager::Get().MakeDirectory(*ModDirectory, true);
	}

	return ModDirectory;
}

FString UModioAPIObject::GetMediaDirectoryPathForMod(int32 ModID)
{
	FString ModMediaDirectory = GetDirectoryPathForMod(ModID);

	ModMediaDirectory.Append("Media/");

	if (!IFileManager::Get().DirectoryExists(*ModMediaDirectory))
	{
		IFileManager::Get().MakeDirectory(*ModMediaDirectory, true);
	}

	return ModMediaDirectory;
}

FString UModioAPIObject::GetGuidesDirectoryPath()
{
	FString GuidesDirectoryForGame = GetModioGameDirectory();

	GuidesDirectoryForGame.Append("Guides/");

	if (!IFileManager::Get().DirectoryExists(*GuidesDirectoryForGame))
	{
		IFileManager::Get().MakeDirectory(*GuidesDirectoryForGame, true);
	}

	return GuidesDirectoryForGame;
}

FString UModioAPIObject::GetDirectoryPathForGuide(int32 GuideID)
{
	FString GuideDirectory = GetGuidesDirectoryPath();

	GuideDirectory.Append(FString::FromInt(GuideID) + "/");

	if (!IFileManager::Get().DirectoryExists(*GuideDirectory))
	{
		IFileManager::Get().MakeDirectory(*GuideDirectory, true);
	}

	return GuideDirectory;
}

FString UModioAPIObject::GetMediaDirectoryPathForGuide(int32 GuideID)
{
	FString GuideMediaDirectory = GetDirectoryPathForGuide(GuideID);

	GuideMediaDirectory.Append("Media/");

	if (!IFileManager::Get().DirectoryExists(*GuideMediaDirectory))
	{
		IFileManager::Get().MakeDirectory(*GuideMediaDirectory, true);
	}

	return GuideMediaDirectory;
}

FString UModioAPIObject::GetDirectoryPathForUploadSession(FString MultipartUploadSessionID)
{
	FString UploadSessionDirectory = GetUploadSessionsDirectoryPath();

	UploadSessionDirectory.Append(MultipartUploadSessionID + "/");

	if (!IFileManager::Get().DirectoryExists(*UploadSessionDirectory))
	{
		IFileManager::Get().MakeDirectory(*UploadSessionDirectory, true);
	}

	return UploadSessionDirectory;
}

void UModioAPIObject::PrepareZipArchiveForMultipartUploadSession(FString ZipArchiveFilePath, FString MultipartUploadSessionID, bool& Success, FString& Message, int64& TotalSize)
{
	if (ZipArchiveFilePath.IsEmpty())
	{
		Message = "No Zip Archive File Path provided!";
		Success = false;
		return;
	}

	if (!IFileManager::Get().FileExists(*ZipArchiveFilePath))
	{
		Message = "Zip Archive File Path not pointing to a File!";
		Success = false;
		return;
	}

	if (FPaths::GetExtension(ZipArchiveFilePath, false).ToLower() != "zip")
	{
		Message = "Zip Archive File Path not pointing to a Zip-File!";
		Success = false;
		return;
	}

	if (MultipartUploadSessionID.IsEmpty())
	{
		Message = "Multipart Upload Session ID is empty!";
		Success = false;
		return;
	}

	IFileHandle* ZipArchiveFileHandle = FPlatformFileManager::Get().GetPlatformFile().OpenRead(*ZipArchiveFilePath, true);

	if (!ZipArchiveFileHandle)
	{
		Message = "No Success with Reading from the Zip Archive File!";
		Success = false;
		return;
	}

	TotalSize = ZipArchiveFileHandle->Size();
	int64 BytesPerSplit = 52428800; // 50 Mebibyte = 52.428.800 Bytes
	int32 PacketCount = FMath::CeilToInt((float(TotalSize)) / (float(BytesPerSplit)));
	int64 BytesRead = 0;
	uint8* DataBuffer;
	int32 DataPartIndex = -1;
	TArray<uint8> DataPartData;
	bool WriteSuccess;
	FString WriteMessage;

	int64 NewPosition;

	// All Splits except last one
	if (PacketCount > 1)
	{
		for (int32 Index = 0; Index < PacketCount - 1; ++Index)
		{
			DataBuffer = (uint8*)FMemory::Malloc(BytesPerSplit);

			NewPosition = Index * BytesPerSplit;
			ZipArchiveFileHandle->Seek(NewPosition);

			if (ZipArchiveFileHandle->Read(DataBuffer, BytesPerSplit))
			{
				BytesRead = BytesRead + BytesPerSplit;
				DataPartIndex = Index;
				DataPartData.Empty();
				DataPartData.Append(DataBuffer, BytesPerSplit);
				WriteUploadPartToMultipartUploadSessionDirectory(MultipartUploadSessionID, DataPartIndex, DataPartData, WriteSuccess, WriteMessage);
			}

			FMemory::Free(DataBuffer);
		}
	}

	// Last Split which is not the full size!
	int64 RemainingSize = TotalSize - BytesRead;

	NewPosition = (PacketCount - 1) * BytesPerSplit;
	ZipArchiveFileHandle->Seek(NewPosition);

	DataBuffer = (uint8*)FMemory::Malloc(RemainingSize);

	if (ZipArchiveFileHandle->Read(DataBuffer, RemainingSize))
	{
		BytesRead = BytesRead + RemainingSize;
		DataPartIndex = (PacketCount - 1);
		DataPartData.Empty();
		DataPartData.Append(DataBuffer, RemainingSize);
		WriteUploadPartToMultipartUploadSessionDirectory(MultipartUploadSessionID, DataPartIndex, DataPartData, WriteSuccess, WriteMessage);
	}

	FMemory::Free(DataBuffer);

	Success = true;
	Message = "Successfully prepared Zip Archive for Upload Session!";
	return;
}

void UModioAPIObject::WriteUploadPartToMultipartUploadSessionDirectory(FString MultipartUploadSessionID, int32 UploadPartIndex, TArray<uint8> UploadPartData, bool& Success, FString& Message)
{
	if (MultipartUploadSessionID.IsEmpty())
	{
		Message = "Upload Session ID is empty!";
		Success = false;
		return;
	}

	if (UploadPartData.IsEmpty())
	{
		Message = "Upload Part Data empty!";
		Success = false;
		return;
	}

	FString FileName = GetDirectoryPathForUploadSession(MultipartUploadSessionID) + "Upload.zip." + UModioAPIFunctionLibrary::ConvertIntToZipFilePartNumber(UploadPartIndex + 1);
	Success = FFileHelper::SaveArrayToFile(UploadPartData, *FileName);

	if (Success)
	{
		Message = "Successfully wrote Data to File!";
	}
	else
	{
		Message = "Error while writing Data to File!";
	}
	return;
}

bool UModioAPIObject::CleanupUploadSession(FString MultipartUploadSessionID, FString& Message)
{
	FString CleanupDirectory = GetDirectoryPathForUploadSession(MultipartUploadSessionID);

	if (CleanupDirectory.IsEmpty())
	{
		Message = "Can't find the Directory for the Upload Session in the Cache!";
		return false;
	}

	if (!FPaths::DirectoryExists(CleanupDirectory))
	{
		Message = "The Directory that should be deleted doesn't exist!";
		return false;
	}

	if (IFileManager::Get().DeleteDirectory(*CleanupDirectory, true, true))
	{
		Message = "Successfully cleaned up the Upload Session Cache!";
		return true;
	}
	else
	{
		Message = "Error when cleaning up the Upload Session Cache!";
		return false;
	}
}

bool UModioAPIObject::CleanupAllUploadSessions(FString& Message)
{
	FString UploadSessionsCacheDirectory = GetUploadSessionsDirectoryPath();

	if (IFileManager::Get().DeleteDirectory(*UploadSessionsCacheDirectory, true, true))
	{
		Message = "Successfully cleaned up all the Upload Sessions in Cache!";
		return true;
	}

	Message = "Error when cleaning up all the Upload Sessions in Cache!";
	return false;
}

FString UModioAPIObject::GetModfilesDirectoryPathForMod(int32 ModID)
{
	FString ModModfilesDirectory = GetDirectoryPathForMod(ModID);

	ModModfilesDirectory.Append("Modfiles/");

	if (!IFileManager::Get().DirectoryExists(*ModModfilesDirectory))
	{
		IFileManager::Get().MakeDirectory(*ModModfilesDirectory, true);
	}

	return ModModfilesDirectory;
}

FString UModioAPIObject::GetUploadSessionsDirectoryPath()
{
	FString UploadSessionsDirectoryForGame = GetModioGameDirectory();

	UploadSessionsDirectoryForGame.Append("Uploads/");

	if (!IFileManager::Get().DirectoryExists(*UploadSessionsDirectoryForGame))
	{
		IFileManager::Get().MakeDirectory(*UploadSessionsDirectoryForGame, true);
	}

	return UploadSessionsDirectoryForGame;
}

FString UModioAPIObject::GetModfileDirectoryPathForModfile(int32 ModID, int32 ModfileID)
{
	FString ModfileDirectory = GetModfilesDirectoryPathForMod(ModID);

	ModfileDirectory.Append(FString::FromInt(ModfileID) + "/");

	if (!IFileManager::Get().DirectoryExists(*ModfileDirectory))
	{
		IFileManager::Get().MakeDirectory(*ModfileDirectory, true);
	}

	return ModfileDirectory;
}

/*
Temporary Cache
*/

bool UModioAPIObject::CacheGame(FModioAPI_Game Game, FString& Message)
{
	if (!TempCache.CachedGames.Find(Game.ID))
	{
		Message = "Game was cached!";
		TempCache.CachedGames.Add(Game.ID, Game);
	}
	else
	{
		Message = "Game with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedGames.Emplace(Game.ID, Game);
	}

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
	if (!TempCache.CachedEvents.Find(UserEvent.ID))
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

bool UModioAPIObject::CacheModfile(FModioAPI_Modfile Modfile, FString& Message)
{
	if (!TempCache.CachedModfiles.Find(Modfile.ID))
	{
		Message = "Modfile was cached!";
		TempCache.CachedModfiles.Add(Modfile.ID, Modfile);
	}
	else
	{
		Message = "Modfile with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedModfiles.Emplace(Modfile.ID, Modfile);
	}

	return true;
}

bool UModioAPIObject::UncacheModfile(int32 ModfileID, FString& Message)
{
	if (TempCache.CachedModfiles.Find(ModfileID))
	{
		Message = "Modfile was uncached!";
		TempCache.CachedModfiles.Remove(ModfileID);
		return true;
	}
	else
	{
		Message = "Modfile with that ID didn't exist in Cache!";
		return false;
	}
}

bool UModioAPIObject::CacheSubscribedMod(FModioAPI_Mod SubscribedMod, FString& Message)
{
	if (!TempCache.CachedSubscribedMods.Find(SubscribedMod.ID))
	{
		Message = "Subscribed Mod was cached!";
		TempCache.CachedSubscribedMods.Add(SubscribedMod.ID, SubscribedMod);
	}
	else
	{
		Message = "Subscribed Mod with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedSubscribedMods.Emplace(SubscribedMod.ID, SubscribedMod);
	}

	return true;
}

bool UModioAPIObject::CacheUnsubscriptionFromMod(int32 UnsubscribedModID, FString& Message)
{
	if (TempCache.CachedSubscribedMods.Find(UnsubscribedModID))
	{
		Message = "Unsubscription from Mod was cached!";
		TempCache.CachedSubscribedMods.Remove(UnsubscribedModID);
		return true;
	}

	Message = "Unsubscription from Mod couldn't be cached! Mod wasn't in Subscribed Mods Cache!";
	return false;
}

bool UModioAPIObject::CacheMod(FModioAPI_Mod Mod, FString& Message)
{
	if (!TempCache.CachedMods.Find(Mod.ID))
	{
		Message = "Mod was cached!";
		TempCache.CachedMods.Add(Mod.ID, Mod);
	}
	else
	{
		Message = "Mod with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedMods.Emplace(Mod.ID, Mod);
	}

	return true;
}

bool UModioAPIObject::UncacheMod(int32 ModID, FString& Message)
{
	if (TempCache.CachedMods.Find(ModID))
	{
		Message = "Mod was uncached!";
		TempCache.CachedMods.Remove(ModID);
		return true;
	}
	else
	{
		Message = "Mod with that ID didn't exist in Cache!";
		return false;
	}
}

bool UModioAPIObject::CachePurchasedMod(FModioAPI_Mod PurchasedMod, FString& Message)
{
	if (!TempCache.CachedPurchasedMods.Find(PurchasedMod.ID))
	{
		Message = "Purchased Mod was cached!";
		TempCache.CachedPurchasedMods.Add(PurchasedMod.ID, PurchasedMod);
	}
	else
	{
		Message = "Purchased Mod with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedPurchasedMods.Emplace(PurchasedMod.ID, PurchasedMod);
	}

	return true;
}

bool UModioAPIObject::CacheMutedUser(FModioAPI_User User, FString& Message)
{
	if (!TempCache.CachedMutedUsers.Find(User.ID))
	{
		Message = "Muted User was cached!";
		TempCache.CachedMutedUsers.Add(User.ID, User);
	}
	else
	{
		Message = "Muted User with that ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedMutedUsers.Emplace(User.ID, User);
	}

	FString Temp;
	CacheUser(User, Temp);

	return true;
}

bool UModioAPIObject::CacheRating(FModioAPI_Rating Rating, FString& Message)
{
	if (Rating.Game_ID != ModioGameID)
	{
		Message = "Only Ratings for the Game-ID of the Connection are supported right now!";
		return false;
	}

	if (!TempCache.CachedModRatings.Find(Rating.Mod_ID))
	{
		Message = "Rating was cached!";
		TempCache.CachedModRatings.Add(Rating.Mod_ID, Rating);
	}
	else
	{
		Message = "Rating with that Mod-ID already existed in Cache. Entry was overwritten!";
		TempCache.CachedModRatings.Emplace(Rating.Mod_ID, Rating);
	}

	return true;
}

bool UModioAPIObject::CacheWallet(FModioAPI_Wallet Wallet, FString& Message)
{
	Message = "Wallet was cached!";
	TempCache.CachedWallet = Wallet;
	return true;
}

bool UModioAPIObject::CacheGameTagOptions(FModioAPI_GetGameTagOptions GameTagOptions, FString& Message)
{
	if (TempCache.CachedGames.Find(ModioGameID))
	{
		TempCache.CachedGames.Find(ModioGameID)->Tag_Options = GameTagOptions.Data;
		Message = "Game Tag Options were cached!";
		return true;
	}
	else
	{
		Message = "Game wasn't cached before!";
	}

	return false;
}

bool UModioAPIObject::GetCachedModfilesForMod(int32 ModID, TArray<int32>& Modfiles, FString& Message)
{
	if (ModID < 1)
	{
		return false;
	}

	TArray<FModioAPI_Modfile> CachedModfiles;
	GetCached_Modfiles(FModioAPI_RequestFilters(), FModioAPI_RequestSorting(), FModioAPI_RequestPagination(), CachedModfiles);

	TArray<int32> CachedModfilesForMod;
	CachedModfilesForMod.Empty();

	for (FModioAPI_Modfile Modfile : CachedModfiles)
	{
		if (Modfile.Mod_ID == ModID)
		{
			CachedModfilesForMod.Add(Modfile.ID);
		}
	}

	Modfiles = CachedModfilesForMod;
	return true;
}

bool UModioAPIObject::GetCachedActiveModfileForPlatform(FModioAPI_Mod Mod, TEnumAsByte<EModioAPI_Platforms> Platform, FModioAPI_Modfile& Modfile, FString& Message)
{
	if (Mod.ID < 1)
	{
		return false;
	}

	for (FModioAPI_ModPlatforms ModPlatform : Mod.Platforms)
	{
		if (ModPlatform.Platform == UModioAPIFunctionLibrary::ConvertPlatformToPlatformCode(Platform))
		{
			if (GetCached_Modfile(ModPlatform.Modfile_Live, Modfile))
			{
				return true;
			}
		}
	}

	Modfile = FModioAPI_Modfile();
	return false;
}

UTexture2D* UModioAPIObject::GetGameLogoFromCache(FString& Message)
{
	UTexture2D* GameLogo;

	FImage LoadedImage;
	FString FilePath = GetMediaDirectoryPathForGame() + "Logo.png";

	if (!IFileManager::Get().FileExists(*FilePath))
	{
		Message = "Can't find Logo for Game from Cache!";
		return NULL;
	}

	if (!FImageUtils::LoadImage(*FilePath, LoadedImage))
	{
		Message = "Can't load Logo for Game from Cache!";
		return NULL;
	}

	GameLogo = FImageUtils::CreateTexture2DFromImage(LoadedImage);

	return GameLogo;
}

UTexture2D* UModioAPIObject::GetGameHeaderFromCache(FString& Message)
{
	UTexture2D* GameHeader;

	FImage LoadedImage;
	FString FilePath = GetMediaDirectoryPathForGame() + "Header.png";

	if (!IFileManager::Get().FileExists(*FilePath))
	{
		Message = "Can't find Header for Game from Cache!";
		return NULL;
	}

	if (!FImageUtils::LoadImage(*FilePath, LoadedImage))
	{
		Message = "Can't load Header for Game from Cache!";
		return NULL;
	}

	GameHeader = FImageUtils::CreateTexture2DFromImage(LoadedImage);

	return GameHeader;
}

UTexture2D* UModioAPIObject::GetGameIconFromCache(FString& Message)
{
	UTexture2D* GameIcon;

	FImage LoadedImage;
	FString FilePath = GetMediaDirectoryPathForGame() + "Icon.png";

	if (!IFileManager::Get().FileExists(*FilePath))
	{
		Message = "Can't find Icon for Game from Cache!";
		return NULL;
	}

	if (!FImageUtils::LoadImage(*FilePath, LoadedImage))
	{
		Message = "Can't load Icon for Game from Cache!";
		return NULL;
	}

	GameIcon = FImageUtils::CreateTexture2DFromImage(LoadedImage);

	return GameIcon;
}

UTexture2D* UModioAPIObject::GetModLogoFromCache(int32 ModID, FString& Message)
{
	UTexture2D* ModLogo;

	FImage LoadedImage;
	FString FilePath = GetMediaDirectoryPathForMod(ModID) + "Logo.png";

	if (!IFileManager::Get().FileExists(*FilePath))
	{
		Message = "Can't find Logo for Mod from Cache!";
		return NULL;
	}

	if (!FImageUtils::LoadImage(*FilePath, LoadedImage))
	{
		Message = "Can't load Logo for Mod from Cache!";
		return NULL;
	}

	ModLogo = FImageUtils::CreateTexture2DFromImage(LoadedImage);

	return ModLogo;
}

TArray<UTexture2D*> UModioAPIObject::GetModImagesFromCache(int32 ModID, FString& Message)
{
	// TODO
	Message = "Not implemented yet!";
	return TArray<UTexture2D*>();
}

bool UModioAPIObject::GetModfilePathFromCache(int32 ModID, int32 ModfileID, FString& PathToModfile, FString& Message)
{
	PathToModfile = "";

	if (ModID <= 0)
	{
		Message = "Invalid Mod ID!";
		return false;
	}

	if (ModfileID <= 0)
	{
		Message = "Invalid Modfile ID!";
		return false;
	}

	FString ModfileDirectory = GetModfileDirectoryPathForModfile(ModID, ModfileID);

	if (!IFileManager::Get().DirectoryExists(*ModfileDirectory))
	{
		Message = "Directory for that Modfile doesn't exist in Cache!";
		return false;
	}

	// Get Zip-Archive from Folder
	TArray<FString> Files = TArray<FString>();
	IFileManager::Get().FindFiles(Files, *ModfileDirectory);

	for (FString File : Files)
	{
		if (File.ToLower().EndsWith(".zip"))
		{
			PathToModfile = File;
			Message = "Found the Modfile Zip-Archive!";
			return true;
		}
	}

	Message = "No Zip-Archive found in the Cache Directory for the Modfile!";
	return false;
}

UTexture2D* UModioAPIObject::GetAvatarOfUserFromCache(int32 UserID, FString& Message)
{
	UTexture2D* UserAvatar;

	FImage LoadedImage;

	// TODO: This could potentially be a different type of Image-File / Extension (.jpg, .jpeg, etc.)
	FString FilePath = GetAvatarDirectoryPathForUser(UserID) + "Avatar.png";

	if (!IFileManager::Get().FileExists(*FilePath))
	{
		Message = "Can't find Avatar of User in Cache!";
		return NULL;
	}

	if (!FImageUtils::LoadImage(*FilePath, LoadedImage))
	{
		Message = "Can't load Avatar of User from Cache!";
		return NULL;
	}

	UserAvatar = FImageUtils::CreateTexture2DFromImage(LoadedImage);

	return UserAvatar;
}

bool UModioAPIObject::ClearCachedAvatarForUser(FModioAPI_User User)
{
	if (User.ID <= 0)
	{
		return false;
	}

	// Clear Cached Avatar
	TArray<FString> Files = TArray<FString>();
	FString UserDirectory = GetAvatarDirectoryPathForUser(User.ID);
	IFileManager::Get().FindFiles(Files, *UserDirectory);

	for (FString File : Files)
	{
		if (File.Contains("Avatar"))
		{
			if (IFileManager::Get().Delete(*(UserDirectory + File), true))
			{
				return true;
			}
		}
	}

	return false;
}

bool UModioAPIObject::ClearCachedFileStorageForUser(FModioAPI_User User)
{
	if (User.ID <= 0)
	{
		return false;
	}

	FString UserDirectory = GetAvatarDirectoryPathForUser(User.ID);
	if (IFileManager::Get().DeleteDirectory(*UserDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedFileStorageForAllUsers()
{
	FString UsersDirectory = GetUsersDirectoryPath();
	if (IFileManager::Get().DeleteDirectory(*UsersDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedFileStorageForGuide(FModioAPI_Guide Guide)
{
	if (Guide.ID <= 0)
	{
		return false;
	}

	FString GuideDirectory = GetDirectoryPathForGuide(Guide.ID);
	if (IFileManager::Get().DeleteDirectory(*GuideDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedFileStorageForAllGuides()
{
	FString GuidesDirectory = GetGuidesDirectoryPath();
	if (IFileManager::Get().DeleteDirectory(*GuidesDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedMediaForMod(FModioAPI_Mod Mod)
{
	FString ModsMediaDirectory = GetMediaDirectoryPathForMod(Mod.ID);
	if (IFileManager::Get().DeleteDirectory(*ModsMediaDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedModfilesForMod(FModioAPI_Mod Mod)
{
	FString ModsModfilesDirectory = GetModfilesDirectoryPathForMod(Mod.ID);
	if (IFileManager::Get().DeleteDirectory(*ModsModfilesDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedModfilesForModExcept(FModioAPI_Mod Mod, int32 ExceptedModfileID)
{
	FString ModsModfilesDirectory = GetModfilesDirectoryPathForMod(Mod.ID);
	TArray<FString> Directories;

	IFileManager::Get().FindFilesRecursive(Directories, *ModsModfilesDirectory, TEXT("*"), false, true, true);

	for (FString Directory : Directories)
	{
		if (!Directory.EndsWith(FString::FromInt(ExceptedModfileID)))
		{
			IFileManager::Get().DeleteDirectory(*Directory, true, true);
		}
	}

	return true;
}

bool UModioAPIObject::ClearCachedFileStorageForMod(FModioAPI_Mod Mod)
{
	FString ModFileStorageDirectory = GetDirectoryPathForMod(Mod.ID);
	if (IFileManager::Get().DeleteDirectory(*ModFileStorageDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearCachedFileStorageForAllMods()
{
	FString ModsFileStorageDirectory = GetModsDirectoryPath();
	if (IFileManager::Get().DeleteDirectory(*ModsFileStorageDirectory, true, true))
	{
		return true;
	}

	return false;
}

bool UModioAPIObject::ClearPersistingCache()
{
	FString PersistingCacheFilePath = GetPersistingCacheFilePath();
	if (IFileManager::Get().Delete(*PersistingCacheFilePath, true))
	{
		return true;
	}

	return false;
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
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + GetApiKey());
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
	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + GetApiKey();
	Request->SetURL(TargetURL);
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

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
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

	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::AddMod_ResponseReceived);
	Request->SetVerb("POST");
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
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestEditMod(FString AccessToken, int32 ModID, FModioAPI_EditMod Mod, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
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
	FHttpRequestRef Request = UModioAPIFunctionLibrary::CreateRequestForEditMod(Mod, ExtractionSuccessful, ExtractionMessage);

	if (!ExtractionSuccessful)
	{
		Message = "Error when extracting the HTTP Request Body from Edit Mod Struct!";
		return false;
	}

	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID));
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::EditMod_ResponseReceived);
	Request->SetVerb("POST");
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
		Message = "Successfully processed Request for 'Edit Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Edit Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestDeleteMod(FString AccessToken, int32 ModID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModID <= 0)
	{
		Message = "ModID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::DeleteMod_ResponseReceived);
	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + GetApiKey();
	Request->SetURL(TargetURL);
	Request->SetVerb("DELETE");
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
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
}

// Files

bool UModioAPIObject::RequestGetModfiles(int32 ModID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetModfiles_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Modfiles'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Modfiles'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetModfile(int32 ModID, int32 ModfileID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	if (ModfileID <= 0)
	{
		Message = "Modfile ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetModfile_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + "/" + FString::FromInt(ModfileID) + GetApiKey());
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Modfiles'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Modfiles'!";
		return false;
	}
}

bool UModioAPIObject::RequestAddModfile(FString AccessToken, int32 ModID, FModioAPI_AddModfile Modfile, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FString ExtractionMessage;
	bool ExtractionSuccessful;
	FHttpRequestRef Request = UModioAPIFunctionLibrary::CreateRequestForAddModfile(Modfile, ExtractionSuccessful, ExtractionMessage);

	if (!ExtractionSuccessful)
	{
		Message = "Error when extracting the HTTP Request Body from Add Mod Struct!";
		return false;
	}

	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::AddModfile_ResponseReceived);
	Request->SetVerb("POST");
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
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestEditModfile(FString AccessToken, int32 ModID, int32 ModfileID, FModioAPI_EditModfile ModfileEdit, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModfileID <= 0)
	{
		Message = "Modfile ID needs to be set!";
		return false;
	}

	FString EditModfilePayload;

	// Version
	if (!ModfileEdit.Version.IsEmpty())
	{
		if (!EditModfilePayload.IsEmpty())
		{
			EditModfilePayload += "&";
		}

		EditModfilePayload += "version=" + UModioAPIFunctionLibrary::SanitizeStringForHTTP_Request(ModfileEdit.Version);
	}

	// Changelog
	if (!ModfileEdit.Changelog.IsEmpty())
	{
		if (!EditModfilePayload.IsEmpty())
		{
			EditModfilePayload += "&";
		}

		EditModfilePayload += "changelog=" + UModioAPIFunctionLibrary::SanitizeStringForHTTP_Request(ModfileEdit.Changelog);
	}

	// Metadata Blob
	if (!ModfileEdit.Metadata_Blob.IsEmpty())
	{
		if (!EditModfilePayload.IsEmpty())
		{
			EditModfilePayload += "&";
		}

		EditModfilePayload += "metadata_blob=" + UModioAPIFunctionLibrary::SanitizeStringForHTTP_Request(ModfileEdit.Metadata_Blob);
	}

	// Active
	if (!EditModfilePayload.IsEmpty())
	{
		EditModfilePayload += "&";
	}
	if (ModfileEdit.Active)
	{
		EditModfilePayload += "active=true";
	}
	else
	{
		EditModfilePayload += "active=false";
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + "/" + FString::FromInt(ModfileID));
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::EditModfile_ResponseReceived);
	Request->SetVerb("PUT");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");
	Request->AppendToHeader("Content-Length", FString::FromInt(EditModfilePayload.Len()));

	if (CachesAccessTokenAutomatically() && AccessToken.IsEmpty())
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + PersistingCache.CachedAccessToken.AccessToken);
	}
	else
	{
		Request->AppendToHeader("Authorization", EndpointAuthenticationBearer + AccessToken);
	}

	Request->SetContentAsString(EditModfilePayload);

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Edit Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Edit Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestDeleteModfile(FString AccessToken, int32 ModID, int32 ModfileID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModID <= 0)
	{
		Message = "Mod ID needs to be set!";
		return false;
	}

	if (ModfileID <= 0)
	{
		Message = "Modfile ID needs to be set!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + "/" + FString::FromInt(ModfileID));
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::DeleteModfile_ResponseReceived);
	Request->SetVerb("DELETE");
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
		Message = "Successfully processed Request for 'Delete Modfile'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Delete Modfile'!";
		return false;
	}
}

bool UModioAPIObject::RequestManageModfilePlatformStatus(FString AccessToken, int32 ModID, int32 ModfileID, FModioAPI_ManageModfilePlatformStatus ModfilePlatformStatusManipulations, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModID <= 0)
	{
		Message = "Mod ID needs to be set!";
		return false;
	}

	if (ModfileID <= 0)
	{
		Message = "Modfile ID needs to be set!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + "/" + FString::FromInt(ModfileID) + EndpointPlatforms);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::ManageModfilePlatformStatus_ResponseReceived);
	Request->SetVerb("POST");
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

	FString RequestPayload;

	// Approved Platforms
	TArray<TEnumAsByte<EModioAPI_Platforms>> ApprovedPlatforms;

	for (TEnumAsByte<EModioAPI_Platforms> Platform : ModfilePlatformStatusManipulations.Approved)
	{
		ApprovedPlatforms.AddUnique(Platform);
	}

	// Denied Platforms
	TArray<TEnumAsByte<EModioAPI_Platforms>> DeniedPlatforms;

	for (TEnumAsByte<EModioAPI_Platforms> Platform : ModfilePlatformStatusManipulations.Denied)
	{
		DeniedPlatforms.AddUnique(Platform);
	}

	// Generate String for Approved Platforms
	if (ApprovedPlatforms.Num() > 0)
	{
		for (TEnumAsByte<EModioAPI_Platforms> Platform : ApprovedPlatforms)
		{
			FString PlatformCode = UModioAPIFunctionLibrary::ConvertPlatformToPlatformCode(Platform);

			if (!PlatformCode.IsEmpty())
			{
				RequestPayload.Append("approved[]=" + PlatformCode + "&");
			}
		}
	}

	// Generate String for Denied Platforms
	if (DeniedPlatforms.Num() > 0)
	{
		for (TEnumAsByte<EModioAPI_Platforms> Platform : DeniedPlatforms)
		{
			FString PlatformCode = UModioAPIFunctionLibrary::ConvertPlatformToPlatformCode(Platform);

			if (!PlatformCode.IsEmpty())
			{
				RequestPayload.Append("denied[]=" + PlatformCode + "&");
			}
		}
	}

	// Remove last &-Char
	if (RequestPayload.EndsWith("&"))
	{
		RequestPayload.RemoveAt(RequestPayload.Len() - 1, 1, true);
	}

	Request->SetContentAsString(RequestPayload);

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Delete Modfile'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Delete Modfile'!";
		return false;
	}
}

// Files (Multipart Uploads)

bool UModioAPIObject::RequestGetMultipartUploadParts(FString AccessToken, int32 ModID, FString UploadSessionID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	if (UploadSessionID.IsEmpty())
	{
		Message = "Upload Session ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetMultipartUploadParts_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + EndpointMultipart + GetApiKey() + "&upload_id=" + UploadSessionID);
	Request->SetVerb("GET");
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
		Message = "Successfully processed Request for 'Get Modfiles'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Modfiles'!";
		return false;
	}
}

bool UModioAPIObject::RequestAddMultipartUploadPart(FString AccessToken, int32 ModID, FString UploadID, int64 ContentRangeStart, int64 ContentRangeEnd, int64 ContentTotalSize, TArray<uint8> Data, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	if (UploadID.IsEmpty())
	{
		Message = "Upload Session ID is empty!";
		return false;
	}

	if (ContentRangeStart < 0)
	{
		Message = "Content Range Start is invalid!";
		return false;
	}

	if (ContentRangeEnd <= 0)
	{
		Message = "Content Range End is invalid!";
		return false;
	}

	if (ContentTotalSize <= 0)
	{
		Message = "Content Total Size is invalid!";
		return false;
	}

	if (Data.IsEmpty())
	{
		Message = "Data is empty!";
		return false;
	}

	// Special Case for last Part!
	if (ContentRangeEnd != ContentTotalSize)
	{
		// -1 Because we start at Index 0, but Data starts at 1!
		if (ContentRangeEnd - ContentRangeStart != Data.Num() - 1)
		{
			Message = "Content Range doesn't match Data Size!";
			return false;
		}
	}
	else
	{
		if (ContentRangeEnd - ContentRangeStart != Data.Num())
		{
			Message = "Content Range doesn't match Data Size!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::AddMultipartUploadPart_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + EndpointMultipart + "?upload_id=" + UploadID);
	Request->SetVerb("PUT");
	Request->AppendToHeader("Accept", "application/json");
	FString ContentRangeHeader = "bytes " + (FString::Printf(TEXT("%lld"), ContentRangeStart)) + "-" + (FString::Printf(TEXT("%lld"), ContentRangeEnd)) + "/" + (FString::Printf(TEXT("%lld"), ContentTotalSize));
	Request->AppendToHeader("Content-Range", ContentRangeHeader);
	Request->SetContent(Data);

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
		Message = "Successfully processed Request for 'Get Modfiles'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Modfiles'!";
		return false;
	}
}

bool UModioAPIObject::RequestCreateMultipartUploadSession(FString AccessToken, int32 ModID, FString Filename, FString Nonce, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	if (Filename.IsEmpty())
	{
		Message = "Filename is invalid!";
		return false;
	}

	if (!Filename.ToLower().EndsWith(".zip"))
	{
		Message = "File needs to be a Zip Archive File and have the .zip-Extension!";
		return false;
	}

	if (!Nonce.IsEmpty())
	{
		if (Nonce.Len() > 64)
		{
			Message = "Nonce too long! Max. Characters is 64!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::CreateMultipartUploadSession_ResponseReceived);
	FString URL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + EndpointMultipart + GetApiKey();
	Request->SetURL(URL);
	Request->SetVerb("POST");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	FString RequestPayload = "filename=" + Filename;
	if (!Nonce.IsEmpty())
	{
		RequestPayload.Append("&nonce=" + Nonce);
	}
	Request->SetContentAsString(RequestPayload);

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
		Message = "Successfully processed Request for 'Get Modfiles'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Modfiles'!";
		return false;
	}
}

bool UModioAPIObject::RequestDeleteMultipartUploadSession(FString AccessToken, int32 ModID, FString UploadID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	if (UploadID.IsEmpty())
	{
		Message = "Upload ID is empty!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::DeleteMultipartUploadSession_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + EndpointMultipart + GetApiKey() + "&upload_id=" + UploadID);
	Request->SetVerb("DELETE");
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
		Message = "Successfully processed Request for 'Delete Multipart Upload Sessions'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Delete Multipart Upload Sessions'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetMultipartUploadSessions(FString AccessToken, int32 ModID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetMultipartUploadSessions_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + EndpointMultipart + EndpointSessions + GetApiKey());
	Request->SetVerb("GET");
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
		Message = "Successfully processed Request for 'Get Multipart Upload Sessions'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Multipart Upload Sessions'!";
		return false;
	}
}

bool UModioAPIObject::RequestCompleteMultipartUploadSession(FString AccessToken, int32 ModID, FString UploadID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::CompleteMultipartUploadSession_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointFiles + EndpointMultipart + EndpointComplete + GetApiKey() + "&upload_id=" + UploadID);
	Request->SetVerb("POST");
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
		Message = "Successfully processed Request for 'Get Multipart Upload Sessions'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Multipart Upload Sessions'!";
		return false;
	}
}

// Subscribe

bool UModioAPIObject::RequestSubscribeToMod(FString AccessToken, int32 ModID, bool IncludeDependencies, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointSubscribe;

	Request->SetURL(TargetURL);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::SubscribeToMod_ResponseReceived);
	Request->SetVerb("POST");
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
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
}

bool UModioAPIObject::RequestUnsubscribeFromMod(FString AccessToken, int32 ModID, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointSubscribe;

	Request->SetURL(TargetURL);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::UnsubscribeFromMod_ResponseReceived);
	Request->SetVerb("DELETE");
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
		Message = "Successfully processed Request for 'Get Mod'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Mod'!";
		return false;
	}
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
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointTags + GetApiKey();

	Request->SetURL(TargetURL);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetGameTagOptions_ResponseReceived);
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Game Tag Options'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Game Tag Options'!";
		return false;
	}
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
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointTags + GetApiKey();

	Request->SetURL(TargetURL);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetModTags_ResponseReceived);
	Request->SetVerb("GET");
	Request->AppendToHeader("Content-Type", "application/x-www-form-urlencoded");
	Request->AppendToHeader("Accept", "application/json");

	if (Request->ProcessRequest())
	{
		Message = "Successfully processed Request for 'Get Game Tag Options'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Game Tag Options'!";
		return false;
	}
}

bool UModioAPIObject::RequestAddModTags(FString AccessToken, int32 ModID, TArray<FString> Tags, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (!AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	if (ModID <= 0)
	{
		Message = "Mod ID is invalid!";
		return false;
	}

	if (Tags.IsEmpty())
	{
		Message = "Array of Tags to add is empty!";
		return false;
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	FString TargetURL = GetApiPath() + EndpointGames + "/" + FString::FromInt(ModioGameID) + EndpointMods + "/" + FString::FromInt(ModID) + EndpointTags + GetApiKey();

	Request->SetURL(TargetURL);
	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::AddModTags_ResponseReceived);
	Request->SetVerb("POST");
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
		Message = "Successfully processed Request for 'Get Game Tag Options'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Game Tag Options'!";
		return false;
	}
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

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
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

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
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

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
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
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserGames_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointGames + GetApiKey());
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
		Message = "Successfully processed Request for 'Get User Games'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Games'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserSubscriptions(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserSubscriptions_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointSubscribed + GetApiKey());
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
		Message = "Successfully processed Request for 'Get User Subscriptions'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Subscriptions'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserMods(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserMods_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointMods + GetApiKey());
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
		Message = "Successfully processed Request for 'Get User Mods'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Mods'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserPurchases(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserPurchases_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointPurchased + GetApiKey());
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
		Message = "Successfully processed Request for 'Get User Purchases'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Purchases'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUsersMuted(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUsersMuted_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointUsers + EndpointMuted + GetApiKey());
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
		Message = "Successfully processed Request for 'Get Users Muted'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get Users Muted'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserRatings(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserRatings_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointRatings + GetApiKey());
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
		Message = "Successfully processed Request for 'Get User Ratings'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Ratings'!";
		return false;
	}
}

bool UModioAPIObject::RequestGetUserWallet(FString AccessToken, FString& Message)
{
	if (!IsInitialized())
	{
		Message = "Mod.io not yet initialized!";
		return false;
	}

	if (AccessToken.IsEmpty())
	{
		if (!IsAuthorized())
		{
			Message = "You need to be authorized (logged in) to Request this!";
			return false;
		}
	}

	FHttpRequestRef Request = FHttpModule::Get().CreateRequest();

	Request->OnProcessRequestComplete().BindUObject(this, &UModioAPIObject::GetUserWallet_ResponseReceived);
	Request->SetURL(GetApiPath() + EndpointMe + EndpointWallets + GetApiKey());
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
		Message = "Successfully processed Request for 'Get User Ratings'!";
		return true;
	}
	else
	{
		Message = "Error when Processing Request for 'Get User Ratings'!";
		return false;
	}
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
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMods.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	FModioAPI_GetMods ModsInfo = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMods(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMods.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	for (FModioAPI_Mod Mod : ModsInfo.Data)
	{
		CacheMod(Mod, ConvertMessage);
	}

	OnResponseReceived_GetMods.Broadcast(ModsInfo, FModioAPI_Error_Object());
}

void UModioAPIObject::GetMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	FModioAPI_Mod Mod = UModioAPIFunctionLibrary::ConvertJsonObjectToMod(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	CacheMod(Mod, ConvertMessage);
	OnResponseReceived_GetMod.Broadcast(Mod, FModioAPI_Error_Object());
}

void UModioAPIObject::AddMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 201 for Successful Request
	if (Response.Get()->GetResponseCode() != 201)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_AddMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	FModioAPI_Mod Mod = UModioAPIFunctionLibrary::ConvertJsonObjectToMod(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_AddMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	CacheMod(Mod, ConvertMessage);
	OnResponseReceived_AddMod.Broadcast(Mod, FModioAPI_Error_Object());
}

void UModioAPIObject::EditMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_EditMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	FModioAPI_Mod Mod = UModioAPIFunctionLibrary::ConvertJsonObjectToMod(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_EditMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	CacheMod(Mod, ConvertMessage);
	OnResponseReceived_EditMod.Broadcast(Mod, FModioAPI_Error_Object());
}

void UModioAPIObject::DeleteMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 204 for Successful Request
	if (Response.Get()->GetResponseCode() != 204)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_DeleteMod.Broadcast(ModID, ErrorResponse);
		return;
	}

	UncacheMod(ModID, ConvertMessage);
	OnResponseReceived_DeleteMod.Broadcast(ModID, FModioAPI_Error_Object());
}

// Files

void UModioAPIObject::GetModfiles_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetModfiles.Broadcast(FModioAPI_GetModfiles(), ErrorResponse);
		return;
	}

	FModioAPI_GetModfiles GetModfiles = UModioAPIFunctionLibrary::ConvertJsonObjectToGetModfiles(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetModfiles.Broadcast(FModioAPI_GetModfiles(), ErrorResponse);
		return;
	}

	for (FModioAPI_Modfile Modfile : GetModfiles.Data)
	{
		CacheModfile(Modfile, ConvertMessage);
	}

	OnResponseReceived_GetModfiles.Broadcast(GetModfiles, FModioAPI_Error_Object());
}

void UModioAPIObject::GetModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetModfile.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	FModioAPI_Modfile Modfile = UModioAPIFunctionLibrary::ConvertJsonObjectToModfile(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetModfile.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	CacheModfile(Modfile, ConvertMessage);
	OnResponseReceived_GetModfile.Broadcast(Modfile, FModioAPI_Error_Object());
}

void UModioAPIObject::AddModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 201 for Successful Request
	if (Response.Get()->GetResponseCode() != 201)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_AddModfile.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	FModioAPI_Modfile Modfile = UModioAPIFunctionLibrary::ConvertJsonObjectToModfile(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_AddModfile.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	CacheModfile(Modfile, ConvertMessage);
	OnResponseReceived_AddModfile.Broadcast(Modfile, FModioAPI_Error_Object());
}

void UModioAPIObject::EditModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_EditModfile.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	FModioAPI_Modfile Modfile = UModioAPIFunctionLibrary::ConvertJsonObjectToModfile(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_EditModfile.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	CacheModfile(Modfile, ConvertMessage);
	OnResponseReceived_EditModfile.Broadcast(Modfile, FModioAPI_Error_Object());
}

void UModioAPIObject::DeleteModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);
	int32 ModfileID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointFiles);

	// Expects Response Code 204 for Successful Request
	if (Response.Get()->GetResponseCode() != 204)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_DeleteModfile.Broadcast(ModID, ModfileID, ErrorResponse);
		return;
	}

	UncacheModfile(ModfileID, ConvertMessage);
	OnResponseReceived_DeleteModfile.Broadcast(ModID, ModfileID, FModioAPI_Error_Object());
}

void UModioAPIObject::ManageModfilePlatformStatus_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);
	int32 ModfileID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointFiles);

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_ManageModfilePlatformStatus.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	FModioAPI_Modfile Modfile = UModioAPIFunctionLibrary::ConvertJsonObjectToModfile(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_ManageModfilePlatformStatus.Broadcast(FModioAPI_Modfile(), ErrorResponse);
		return;
	}

	CacheModfile(Modfile, ConvertMessage);
	OnResponseReceived_ManageModfilePlatformStatus.Broadcast(Modfile, FModioAPI_Error_Object());
}

// Files (Multipart Uploads)

void UModioAPIObject::GetMultipartUploadParts_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMultipartUploadParts.Broadcast(FModioAPI_GetMultipartUploadParts(), ErrorResponse);
		return;
	}

	FModioAPI_GetMultipartUploadParts GetMultipartUploadParts = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMultipartUploadParts(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMultipartUploadParts.Broadcast(FModioAPI_GetMultipartUploadParts(), ErrorResponse);
		return;
	}

	OnResponseReceived_GetMultipartUploadParts.Broadcast(GetMultipartUploadParts, FModioAPI_Error_Object());
}

void UModioAPIObject::AddMultipartUploadPart_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_AddMultipartUploadPart.Broadcast(FModioAPI_MultipartUploadPart(), ErrorResponse);
		return;
	}

	FModioAPI_MultipartUploadPart MultipartUploadPart = UModioAPIFunctionLibrary::ConvertJsonObjectToMultipartUploadPart(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_AddMultipartUploadPart.Broadcast(FModioAPI_MultipartUploadPart(), ErrorResponse);
		return;
	}

	OnResponseReceived_AddMultipartUploadPart.Broadcast(MultipartUploadPart, FModioAPI_Error_Object());
}

void UModioAPIObject::CreateMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_CreateMultipartUploadSession.Broadcast(FModioAPI_MultipartUpload(), ErrorResponse);
		return;
	}

	FModioAPI_MultipartUpload CreatedMultipartUpload = UModioAPIFunctionLibrary::ConvertJsonObjectToMultipartUpload(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_CreateMultipartUploadSession.Broadcast(FModioAPI_MultipartUpload(), ErrorResponse);
		return;
	}

	OnResponseReceived_CreateMultipartUploadSession.Broadcast(CreatedMultipartUpload, FModioAPI_Error_Object());
}

void UModioAPIObject::DeleteMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 204 for Successful Request
	if (Response.Get()->GetResponseCode() != 204)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_DeleteMultipartUploadSession.Broadcast(ModID, ErrorResponse);
		return;
	}

	OnResponseReceived_DeleteMultipartUploadSession.Broadcast(ModID, FModioAPI_Error_Object());
}

void UModioAPIObject::GetMultipartUploadSessions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMultipartUploadSessions.Broadcast(FModioAPI_GetMultipartUploadSessions(), ErrorResponse);
		return;
	}

	FModioAPI_GetMultipartUploadSessions GetMultipartUploadSessions = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMultipartUploadSessions(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetMultipartUploadSessions.Broadcast(FModioAPI_GetMultipartUploadSessions(), ErrorResponse);
		return;
	}

	OnResponseReceived_GetMultipartUploadSessions.Broadcast(GetMultipartUploadSessions, FModioAPI_Error_Object());
}

void UModioAPIObject::CompleteMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_CompleteMultipartUploadSession.Broadcast(FModioAPI_MultipartUpload(), ErrorResponse);
		return;
	}

	FModioAPI_MultipartUpload CompletedMultipartUpload = UModioAPIFunctionLibrary::ConvertJsonObjectToMultipartUpload(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_CompleteMultipartUploadSession.Broadcast(FModioAPI_MultipartUpload(), ErrorResponse);
		return;
	}

	OnResponseReceived_CompleteMultipartUploadSession.Broadcast(CompletedMultipartUpload, FModioAPI_Error_Object());
}

// Subscribe

void UModioAPIObject::SubscribeToMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 201 for Successful Subscription
	if (Response.Get()->GetResponseCode() != 201)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_SubscribeToMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	FModioAPI_Mod Mod = UModioAPIFunctionLibrary::ConvertJsonObjectToMod(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_SubscribeToMod.Broadcast(FModioAPI_Mod(), ErrorResponse);
		return;
	}

	CacheSubscribedMod(Mod, ConvertMessage);
	OnResponseReceived_SubscribeToMod.Broadcast(Mod, FModioAPI_Error_Object());
}

void UModioAPIObject::UnsubscribeFromMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	int32 ModID = UModioAPIFunctionLibrary::ExtractModIDFromRequestURL(Request.Get()->GetURL(), EndpointMods);

	// Expects Response Code 204 for Successful Unsubscription
	if (Response.Get()->GetResponseCode() != 204)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_UnsubscribeFromMod.Broadcast(ModID, ErrorResponse);
		return;
	}

	CacheUnsubscriptionFromMod(ModID, ConvertMessage);
	OnResponseReceived_UnsubscribeFromMod.Broadcast(ModID, FModioAPI_Error_Object());
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
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetGameTagOptions.Broadcast(FModioAPI_GetGameTagOptions(), ErrorResponse);
		return;
	}

	FModioAPI_GetGameTagOptions GetGameTagOptions = UModioAPIFunctionLibrary::ConvertJsonObjectToGetGameTagOptions(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetGameTagOptions.Broadcast(FModioAPI_GetGameTagOptions(), ErrorResponse);
		return;
	}

	CacheGameTagOptions(GetGameTagOptions, ConvertMessage);
	OnResponseReceived_GetGameTagOptions.Broadcast(GetGameTagOptions, FModioAPI_Error_Object());
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
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetModTags.Broadcast(FModioAPI_GetModTags(), ErrorResponse);
		return;
	}

	FModioAPI_GetModTags GetModTags = UModioAPIFunctionLibrary::ConvertJsonObjectToGetModTags(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetModTags.Broadcast(FModioAPI_GetModTags(), ErrorResponse);
		return;
	}

	//CacheGameTagOptions(GetGameTagOptions, ConvertMessage);
	OnResponseReceived_GetModTags.Broadcast(GetModTags, FModioAPI_Error_Object());
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
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetAuthenticatedUser.Broadcast(FModioAPI_User(), ErrorResponse);
		return;
	}

	FModioAPI_User AuthenticatedUser = UModioAPIFunctionLibrary::ConvertJsonObjectToUser(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetAuthenticatedUser.Broadcast(AuthenticatedUser, ErrorResponse);
		return;
	}

	CacheUser(AuthenticatedUser, ConvertMessage);
	TempCache.AuthenticatedUserID = AuthenticatedUser.ID;
	OnResponseReceived_GetAuthenticatedUser.Broadcast(AuthenticatedUser, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserEvents.Broadcast(FModioAPI_GetUserEvents(), ErrorResponse);
		return;
	}
	
	FModioAPI_GetUserEvents GetUserEvents = UModioAPIFunctionLibrary::ConvertJsonObjectToGetUserEvents(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserEvents.Broadcast(FModioAPI_GetUserEvents(), ErrorResponse);
		return;
	}

	for (FModioAPI_UserEvent UserEvent : GetUserEvents.Data)
	{
		CacheUserEvent(UserEvent, ConvertMessage);
	}

	OnResponseReceived_GetUserEvents.Broadcast(GetUserEvents, FModioAPI_Error_Object());
	return;
}

void UModioAPIObject::GetUserModfiles_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserModfiles.Broadcast(FModioAPI_GetModfiles(), ErrorResponse);
		return;
	}

	FModioAPI_GetModfiles UserModfiles = UModioAPIFunctionLibrary::ConvertJsonObjectToGetModfiles(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserModfiles.Broadcast(FModioAPI_GetModfiles(), ErrorResponse);
		return;
	}

	for (FModioAPI_Modfile Modfile : UserModfiles.Data)
	{
		CacheModfile(Modfile, ConvertMessage);
	}

	OnResponseReceived_GetUserModfiles.Broadcast(UserModfiles, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserGames_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserGames.Broadcast(FModioAPI_GetGames(), ErrorResponse);
		return;
	}

	FModioAPI_GetGames Games = UModioAPIFunctionLibrary::ConvertJsonObjectToGetGames(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserGames.Broadcast(FModioAPI_GetGames(), ErrorResponse);
		return;
	}

	for (FModioAPI_Game Game : Games.Data)
	{
		CacheGame(Game, ConvertMessage);
	}

	OnResponseReceived_GetUserGames.Broadcast(Games, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserSubscriptions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserSubscriptions.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	FModioAPI_GetMods SubscribedMods = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMods(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserSubscriptions.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	for (FModioAPI_Mod Mod : SubscribedMods.Data)
	{
		CacheSubscribedMod(Mod, ConvertMessage);
	}

	OnResponseReceived_GetUserSubscriptions.Broadcast(SubscribedMods, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserMods_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserMods.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	FModioAPI_GetMods Mods = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMods(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserMods.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	for (FModioAPI_Mod Mod : Mods.Data)
	{
		CacheMod(Mod, ConvertMessage);
	}

	OnResponseReceived_GetUserMods.Broadcast(Mods, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserPurchases_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserPurchases.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	FModioAPI_GetMods Mods = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMods(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserPurchases.Broadcast(FModioAPI_GetMods(), ErrorResponse);
		return;
	}

	for (FModioAPI_Mod Mod : Mods.Data)
	{
		CachePurchasedMod(Mod, ConvertMessage);
	}

	OnResponseReceived_GetUserPurchases.Broadcast(Mods, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUsersMuted_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUsersMuted.Broadcast(FModioAPI_GetMutedUsers(), ErrorResponse);
		return;
	}

	FModioAPI_GetMutedUsers GetMutedUsers = UModioAPIFunctionLibrary::ConvertJsonObjectToGetMutedUsers(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUsersMuted.Broadcast(FModioAPI_GetMutedUsers(), ErrorResponse);
		return;
	}

	for (FModioAPI_User User : GetMutedUsers.Data)
	{
		CacheMutedUser(User, ConvertMessage);
	}

	OnResponseReceived_GetUsersMuted.Broadcast(GetMutedUsers, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserRatings_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserRatings.Broadcast(FModioAPI_GetUserRatings(), ErrorResponse);
		return;
	}

	FModioAPI_GetUserRatings GetUserRatings = UModioAPIFunctionLibrary::ConvertJsonObjectToGetUserRatings(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserRatings.Broadcast(FModioAPI_GetUserRatings(), ErrorResponse);
		return;
	}

	for (FModioAPI_Rating Rating : GetUserRatings.Data)
	{
		CacheRating(Rating, ConvertMessage);
	}

	OnResponseReceived_GetUserRatings.Broadcast(GetUserRatings, FModioAPI_Error_Object());
}

void UModioAPIObject::GetUserWallet_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully)
{
	// Prepare received Response and Variables
	TSharedPtr<FJsonObject> ResponseObj = UModioAPIFunctionLibrary::ConvertResponseToJsonObject(Response);
	bool ConvertSuccess = false;
	FString ConvertMessage = "";

	// Expects Response Code 200 for Successful Request
	if (Response.Get()->GetResponseCode() != 200)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserWallet.Broadcast(FModioAPI_Wallet(), ErrorResponse);
		return;
	}

	FModioAPI_Wallet Wallet = UModioAPIFunctionLibrary::ConvertJsonObjectToWallet(ResponseObj, ConvertSuccess, ConvertMessage);

	// Did we receive what we expected?
	if (!ConvertSuccess)
	{
		FModioAPI_Error_Object ErrorResponse = UModioAPIFunctionLibrary::ConvertJsonObjectToError(ResponseObj, ConvertSuccess, ConvertMessage);
		OnResponseReceived_GetUserWallet.Broadcast(FModioAPI_Wallet(), ErrorResponse);
		return;
	}

	CacheWallet(Wallet, ConvertMessage);
	OnResponseReceived_GetUserWallet.Broadcast(Wallet, FModioAPI_Error_Object());
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
	if (TempCache.CachedEvents.Find(UserEventID)->ID > 0)
	{
		UserEvent = *TempCache.CachedEvents.Find(UserEventID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Modfiles(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_Modfile>& Modfiles)
{
	if (TempCache.CachedModfiles.Num() > 0)
	{
		TempCache.CachedModfiles.GenerateValueArray(Modfiles);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Modfile(int32 ModfileID, FModioAPI_Modfile& Modfile)
{
	if (TempCache.CachedModfiles.IsEmpty())
	{
		return false;
	}

	if (TempCache.CachedModfiles.Find(ModfileID)->ID > 0)
	{
		Modfile = *TempCache.CachedModfiles.Find(ModfileID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Games(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_Game>& Games)
{
	if (TempCache.CachedGames.Num() > 0)
	{
		TempCache.CachedGames.GenerateValueArray(Games);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Game(FModioAPI_Game& Game)
{
	if (TempCache.CachedGames.IsEmpty())
	{
		return false;
	}

	if (TempCache.CachedGames.Find(ModioGameID)->ID > 0)
	{
		Game = *TempCache.CachedGames.Find(ModioGameID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_SubscribedMods(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_Mod>& SubscribedMods)
{
	if (TempCache.CachedSubscribedMods.Num() > 0)
	{
		TempCache.CachedSubscribedMods.GenerateValueArray(SubscribedMods);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_SubscribedMod(int32 ModID, FModioAPI_Mod& SubscribedMod)
{
	if (TempCache.CachedSubscribedMods.Find(ModID)->ID > 0)
	{
		SubscribedMod = *TempCache.CachedSubscribedMods.Find(ModID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Mods(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_Mod>& Mods)
{
	if (TempCache.CachedMods.Num() > 0)
	{
		TempCache.CachedMods.GenerateValueArray(Mods);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_Mod(int32 ModID, FModioAPI_Mod& Mod)
{
	if (TempCache.CachedMods.IsEmpty())
	{
		return false;
	}

	if (TempCache.CachedMods.Find(ModID)->ID > 0)
	{
		Mod = *TempCache.CachedMods.Find(ModID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_PurchasedMods(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_Mod>& PurchasedMods)
{
	if (TempCache.CachedPurchasedMods.Num() > 0)
	{
		TempCache.CachedPurchasedMods.GenerateValueArray(PurchasedMods);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_PurchasedMod(int32 ModID, FModioAPI_Mod& PurchasedMod)
{
	if (TempCache.CachedPurchasedMods.Find(ModID)->ID > 0)
	{
		PurchasedMod = *TempCache.CachedPurchasedMods.Find(ModID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_MutedUsers(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_User>& MutedUsers)
{
	if (TempCache.CachedMutedUsers.Num() > 0)
	{
		TempCache.CachedMutedUsers.GenerateValueArray(MutedUsers);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_MutedUser(int32 UserID, FModioAPI_User& MutedUser)
{
	if (TempCache.CachedMutedUsers.Find(UserID)->ID > 0)
	{
		MutedUser = *TempCache.CachedMutedUsers.Find(UserID);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_ModRatings(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_Rating>& Ratings)
{
	if (TempCache.CachedModRatings.Num() > 0)
	{
		TempCache.CachedModRatings.GenerateValueArray(Ratings);
		return true;
	}
	else
	{
		return false;
	}
}

bool UModioAPIObject::GetCached_ModRating(int32 ModID, FModioAPI_Rating& Rating)
{
	if (TempCache.CachedModRatings.Find(ModID)->Mod_ID > 0)
	{
		Rating = *TempCache.CachedModRatings.Find(ModID);
		return true;
	}
	else
	{
		return false;
	}
}
