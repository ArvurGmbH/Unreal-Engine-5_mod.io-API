/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "ModioAPIStructs.h"
#include "ModioAPIResponseSchemas.h"
#include "Blueprint/UserWidget.h"
#include "Http.h"
#include "JsonObjectConverter.h"
#include "ModioAPIFunctionLibrary.h"
#include "ModioAPIObject.generated.h"

// Authentication
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetTermsOfServiceDelegate, FModioAPI_Terms, TermsOfService, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_SteamAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_XboxLiveAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_PlayStationNetworkAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_NintendoSwitchAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_MetaQuestAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_EpicGamesAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GOGGalaxyAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GoogleAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_DiscordAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_OpenIDAuthenticationDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetEmailSecurityCodeDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_ExchangeForAccessTokenDelegate, FModioAPI_AccessToken_Response, AccessToken, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_LogoutDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);

// Games
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGameDelegate, FModioAPI_Game, Game, FModioAPI_Error_Object, ErrorResponse);

// Guides
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGuidesDelegate, FModioAPI_GetGuides, Guides, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGuideDelegate, FModioAPI_Guide, Guide, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddGuideDelegate, FModioAPI_Guide, Guide, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_EditGuideDelegate, FModioAPI_Guide, Guide, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteGuideDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGuidesTagsDelegate, FModioAPI_GetGuidesTags, GuidesTags, FModioAPI_Error_Object, ErrorResponse);

// Mods
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModsDelegate, FModioAPI_GetMods, Mods, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModDelegate, FModioAPI_Mod, Mod, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModDelegate, FModioAPI_Mod, Mod, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_EditModDelegate, FModioAPI_Mod, Mod, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModDelegate, FModioAPI_Error_Object, ErrorResponse);

// Files
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModfilesDelegate, FModioAPI_GetModfiles, Modfiles, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModfileDelegate, FModioAPI_Modfile, Modfile, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModfileDelegate, FModioAPI_Modfile, Modfile, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_EditModfileDelegate, FModioAPI_Modfile, Modfile, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModfileDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_ManageModfilePlatformStatusDelegate, FModioAPI_Modfile, Modfile, FModioAPI_Error_Object, ErrorResponse);

// Files (Multipart Uploads)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetMultipartUploadPartsDelegate, FModioAPI_GetMultipartUploadParts, MultipartUploadParts, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddMultipartUploadPartDelegate, FModioAPI_MultipartUploadPart, MultipartUploadPart, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_CreateMultipartUploadSessionDelegate, FModioAPI_MultipartUpload_Object, MultipartUpload, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteMultipartUploadSessionDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetMultipartUploadSessionsDelegate, FModioAPI_GetMultipartUploadSessions, MultipartUploadSessions, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_CompleteMultipartUploadSessionDelegate, FModioAPI_MultipartUpload_Object, MultipartUpload, FModioAPI_Error_Object, ErrorResponse);

// Subscribe
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_SubscribeToModDelegate, FModioAPI_Mod, Mod, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_UnsubscribeFromModDelegate, FModioAPI_Error_Object, ErrorResponse);

// Comments - Guides
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGuideCommentsDelegate, FModioAPI_GetGuideComments, GuideComments, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddGuideCommentDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGuideCommentDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_UpdateGuideCommentDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteGuideCommentDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddGuideCommentKarmaDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);

// Comments - Mods
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModCommentsDelegate, FModioAPI_GetModComments, ModComments, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModCommentDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModCommentDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_UpdateModCommentDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModCommentDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModCommentKarmaDelegate, FModioAPI_Comment, Comment, FModioAPI_Error_Object, ErrorResponse);

// Media
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddGameMediaDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModMediaDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModMediaDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_ReorderModMediaDelegate, FModioAPI_Error_Object, ErrorResponse);

// Events
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModsEventsDelegate, FModioAPI_GetModEvents, ModEvents, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModEventsDelegate, FModioAPI_GetModEvents, ModEvents, FModioAPI_Error_Object, ErrorResponse);

// Tags
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGameTagOptionsDelegate, FModioAPI_GetGameTagOptions, GameTagOptions, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGameTagOptionDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteGameTagOptionDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_RenameGameTagDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModTagsDelegate, FModioAPI_GetModTags, ModTags, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModTagsDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModTagsDelegate, FModioAPI_Error_Object, ErrorResponse);

// Ratings
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModRatingDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);

// Stats
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetGameStatsDelegate, FModioAPI_GameStats, Stats, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModsStatsDelegate, FModioAPI_GetModStats, Stats, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModStatsDelegate, FModioAPI_ModStats, Stats, FModioAPI_Error_Object, ErrorResponse);

// Metadata
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModMetadataKVPDelegate, FModioAPI_GetModMetadataKVP, MetadataKVP, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModMetadataKVPDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModMetadataKVPDelegate, FModioAPI_Error_Object, ErrorResponse);

// Dependencies
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModDependenciesDelegate, FModioAPI_GetModDependencies, ModDependencies, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_AddModDependenciesDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_DeleteModDependenciesDelegate, FModioAPI_Error_Object, ErrorResponse);

// Teams
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetModTeamMembersDelegate, FModioAPI_GetTeamMembers, TeamMembers, FModioAPI_Error_Object, ErrorResponse);

// Reports
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_SubmitReportDelegate, FModioAPI_Message_Object, Message, FModioAPI_Error_Object, ErrorResponse);

// Agreements
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetCurrentAgreementDelegate, FModioAPI_AgreementVersion, AgreementVersion, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetAgreementVersionDelegate, FModioAPI_AgreementVersion, AgreementVersion, FModioAPI_Error_Object, ErrorResponse);

// Users
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_MuteUserDelegate, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_UnmuteUserDelegate, FModioAPI_Error_Object, ErrorResponse);

// Me
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetAuthenticatedUserDelegate, FModioAPI_User, User, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserEventsDelegate, FModioAPI_GetUserEvents, UserEvents, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserModfilesDelegate, FModioAPI_GetModfiles, UserModfiles, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserGamesDelegate, FModioAPI_GetGames, UserGames, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserSubscriptionsDelegate, FModioAPI_GetMods, UserSubscribedMods, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserModsDelegate, FModioAPI_GetMods, UserMods, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserPurchasesDelegate, FModioAPI_GetMods, UserPurchases, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUsersMutedDelegate, FModioAPI_GetMutedUsers, UserMutedUsers, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserRatingsDelegate, FModioAPI_GetUserRatings, UserRatings, FModioAPI_Error_Object, ErrorResponse);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetUserWalletDelegate, FModioAPI_Wallet_Object, Wallet, FModioAPI_Error_Object, ErrorResponse);

// General
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_GetResourceOwnerDelegate, FModioAPI_User, Owner, FModioAPI_Error_Object, ErrorResponse);

// Checkout
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FModioAPI_PurchaseAnItemDelegate, FModioAPI_Pay_Object, Payment, FModioAPI_Error_Object, ErrorResponse);

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType, Category="mod.io API", meta = (DisplayName = "mod.io API Object"))
class MODIOAPI_API UModioAPIObject : public UObject
{
	GENERATED_BODY()
	
	protected:
		const FString EndpointBase = "https://g-";
		const FString EndpointAPIPath = ".modapi.io";
		const FString EndpointAPIVersion = "/v1";
		const FString EndpointAPIKey = "?api_key=";
		const FString EndpointAuthenticationBearer = "Bearer ";

		const FString EndpointAuthenticate = "/authenticate";
		const FString EndpointTerms = "/terms";
		const FString EndpointOAuth = "/oauth";
		const FString EndpointEmailRequest = "/emailrequest";
		const FString EndpointEmailExchange = "/emailexchange";
		const FString EndpointLogout = "/logout";
		const FString EndpointGames = "/games";
		const FString EndpointGuides = "/guides";
		const FString EndpointMods = "/mods";
		const FString EndpointFiles = "/files";
		const FString EndpointPlatforms = "/platforms";
		const FString EndpointMultipart = "/multipart";
		const FString EndpointSessions = "/sessions";
		const FString EndpointComplete = "/complete";
		const FString EndpointSubscribe = "/subscribe";
		const FString EndpointComments = "/comments";
		const FString EndpointKarma = "/karma";
		const FString EndpointMedia = "/media";
		const FString EndpointReorder = "/reorder";
		const FString EndpointEvents = "/events";
		const FString EndpointTags = "/tags";
		const FString EndpointRename = "/rename";
		const FString EndpointRatings = "/ratings";
		const FString EndpointStats = "/stats";
		const FString EndpointMetadataKVP = "/metadatakvp";
		const FString EndpointDependencies = "/dependencies";
		const FString EndpointTeam = "/team";
		const FString EndpointReport = "/report";
		const FString EndpointAgreements = "/agreements";
		const FString EndpointTypes = "/types";
		const FString EndpointCurrent = "/current";
		const FString EndpointVersions = "/versions";
		const FString EndpointUsers = "/users";
		const FString EndpointMute = "/mute";
		const FString EndpointMe = "/me";
		const FString EndpointSubscribed = "/subscribed";
		const FString EndpointPurchased = "/purchased";
		const FString EndpointMuted = "/muted";
		const FString EndpointWallets = "/wallets";
		const FString EndpointGeneral = "/general";
		const FString EndpointOwnership = "/ownership";
		const FString EndpointCheckout = "/checkout";

		UPROPERTY()
		int32 ModioGameID = -1;

		UPROPERTY()
		FString ModioAPI_Key;

		UPROPERTY()
		bool AutoCacheAccessToken;

		UPROPERTY()
		bool StoreCachedAccessTokenPersistingly;

		UPROPERTY()
		FModioAPI_TemporaryCache TempCache;

		UPROPERTY()
		FModioAPI_PersistingCache PersistingCache;

	public:
		// Authentication

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_GetTermsOfServiceDelegate OnResponseReceived_TermsOfService;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_SteamAuthenticationDelegate OnResponseReceived_SteamAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_XboxLiveAuthenticationDelegate OnResponseReceived_XboxLiveAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_PlayStationNetworkAuthenticationDelegate OnResponseReceived_PlayStationNetworkAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_NintendoSwitchAuthenticationDelegate OnResponseReceived_NintendoSwitchAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_MetaQuestAuthenticationDelegate OnResponseReceived_MetaQuestAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_EpicGamesAuthenticationDelegate OnResponseReceived_EpicGamesAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication", meta = (DisplayName = "On Response Received GOG Galaxy Authentication"))
		FModioAPI_GOGGalaxyAuthenticationDelegate OnResponseReceived_GOGGalaxyAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_GoogleAuthenticationDelegate OnResponseReceived_GoogleAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_DiscordAuthenticationDelegate OnResponseReceived_DiscordAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication", meta = (DisplayName = "On Response Received OpenID Authentication"))
		FModioAPI_OpenIDAuthenticationDelegate OnResponseReceived_OpenIDAuthentication;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_GetEmailSecurityCodeDelegate OnResponseReceived_EmailSecurityCode;

		UPROPERTY(BlueprintAssignable, Category = "mod.io API|Events|Authentication")
		FModioAPI_ExchangeForAccessTokenDelegate OnResponseReceived_ExchangeForAccessToken;

		// Games

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Games")
		FModioAPI_GetGameDelegate OnResponseReceived_GetGame;

		// Guides

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Guides")
		FModioAPI_GetGuidesDelegate OnResponseReceived_GetGuides;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Guides")
		FModioAPI_GetGuideDelegate OnResponseReceived_GetGuide;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Guides")
		FModioAPI_AddGuideDelegate OnResponseReceived_AddGuide;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Guides")
		FModioAPI_EditGuideDelegate OnResponseReceived_EditGuide;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Guides")
		FModioAPI_DeleteGuideDelegate OnResponseReceived_DeleteGuide;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Guides")
		FModioAPI_GetGuidesTagsDelegate OnResponseReceived_GetGuidesTags;

		// Mods

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Mods")
		FModioAPI_GetModsDelegate OnResponseReceived_GetMods;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Mods")
		FModioAPI_GetModDelegate OnResponseReceived_GetMod;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Mods")
		FModioAPI_AddModDelegate OnResponseReceived_AddMod;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Mods")
		FModioAPI_EditModDelegate OnResponseReceived_EditMod;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Mods")
		FModioAPI_DeleteModDelegate OnResponseReceived_DeleteMod;

		// Files

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files")
		FModioAPI_GetModfilesDelegate OnResponseReceived_GetModfiles;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files")
		FModioAPI_GetModfileDelegate OnResponseReceived_GetModfile;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files")
		FModioAPI_AddModfileDelegate OnResponseReceived_AddModfile;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files")
		FModioAPI_EditModfileDelegate OnResponseReceived_EditModfile;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files")
		FModioAPI_DeleteModfileDelegate OnResponseReceived_DeleteModfile;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files")
		FModioAPI_ManageModfilePlatformStatusDelegate OnResponseReceived_ManageModfilePlatformStatus;

		// Files (Multipart Uploads)

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files (Multipart Uploads)")
		FModioAPI_GetMultipartUploadPartsDelegate OnResponseReceived_GetMultipartUploadParts;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files (Multipart Uploads)")
		FModioAPI_AddMultipartUploadPartDelegate OnResponseReceived_AddMultipartUploadPart;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files (Multipart Uploads)")
		FModioAPI_CreateMultipartUploadSessionDelegate OnResponseReceived_CreateMultipartUploadSession;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files (Multipart Uploads)")
		FModioAPI_DeleteMultipartUploadSessionDelegate OnResponseReceived_DeleteMultipartUploadSession;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files (Multipart Uploads)")
		FModioAPI_GetMultipartUploadSessionsDelegate OnResponseReceived_GetMultipartUploadSessions;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Files (Multipart Uploads)")
		FModioAPI_CompleteMultipartUploadSessionDelegate OnResponseReceived_CompleteMultipartUploadSession;

		// Subscribe

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Subscribe")
		FModioAPI_SubscribeToModDelegate OnResponseReceived_SubscribeToMod;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Subscribe")
		FModioAPI_UnsubscribeFromModDelegate OnResponseReceived_UnsubscribeFromMod;

		// Comments - Guides

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Guides")
		FModioAPI_GetGuideCommentsDelegate OnResponseReceived_GetGuideComments;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Guides")
		FModioAPI_AddGuideCommentDelegate OnResponseReceived_AddGuideComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Guides")
		FModioAPI_GetGuideCommentDelegate OnResponseReceived_GetGuideComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Guides")
		FModioAPI_UpdateGuideCommentDelegate OnResponseReceived_UpdateGuideComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Guides")
		FModioAPI_DeleteGuideCommentDelegate OnResponseReceived_DeleteGuideComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Guides")
		FModioAPI_AddGuideCommentKarmaDelegate OnResponseReceived_AddGuideCommentKarma;

		// Comments - Mods

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Mods")
		FModioAPI_GetModCommentsDelegate OnResponseReceived_GetModComments;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Mods")
		FModioAPI_AddModCommentDelegate OnResponseReceived_AddModComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Mods")
		FModioAPI_GetModCommentDelegate OnResponseReceived_GetModComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Mods")
		FModioAPI_UpdateModCommentDelegate OnResponseReceived_UpdateModComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Mods")
		FModioAPI_DeleteModCommentDelegate OnResponseReceived_DeleteModComment;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Comments|Mods")
		FModioAPI_AddModCommentKarmaDelegate OnResponseReceived_AddModCommentKarma;

		// Media

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Media")
		FModioAPI_AddGameMediaDelegate OnResponseReceived_AddGameMedia;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Media")
		FModioAPI_AddModMediaDelegate OnResponseReceived_AddModMedia;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Media")
		FModioAPI_DeleteModMediaDelegate OnResponseReceived_DeleteModMedia;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Media")
		FModioAPI_ReorderModMediaDelegate OnResponseReceived_ReorderModMedia;

		// Events

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Events")
		FModioAPI_GetModsEventsDelegate OnResponseReceived_GetModsEvents;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Events")
		FModioAPI_GetModEventsDelegate OnResponseReceived_GetModEvents;

		// Tags

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Tags")
		FModioAPI_GetGameTagOptionsDelegate OnResponseReceived_GetGameTagOptions;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Tags")
		FModioAPI_GetGameTagOptionDelegate OnResponseReceived_GetGameTagOption;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Tags")
		FModioAPI_DeleteGameTagOptionDelegate OnResponseReceived_DeleteGameTagOption;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Tags")
		FModioAPI_RenameGameTagDelegate OnResponseReceived_RenameGameTag;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Tags")
		FModioAPI_AddModTagsDelegate OnResponseReceived_AddModTags;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Tags")
		FModioAPI_DeleteModTagsDelegate OnResponseReceived_DeleteModTags;

		// Ratings

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Ratings")
		FModioAPI_AddModRatingDelegate OnResponseReceived_AddModRating;

		// Stats

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Stats")
		FModioAPI_GetGameStatsDelegate OnResponseReceived_GetGameStats;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Stats")
		FModioAPI_GetModsStatsDelegate OnResponseReceived_GetModsStats;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Stats")
		FModioAPI_GetModStatsDelegate OnResponseReceived_GetModStats;

		// Metadata

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Metadata")
		FModioAPI_GetModMetadataKVPDelegate OnResponseReceived_GetModMetadataKVP;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Metadata")
		FModioAPI_AddModMetadataKVPDelegate OnResponseReceived_AddModMetadataKVP;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Metadata")
		FModioAPI_DeleteModMetadataKVPDelegate OnResponseReceived_DeleteModMetadataKVP;

		// Dependencies

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Dependencies")
		FModioAPI_GetModDependenciesDelegate OnResponseReceived_GetModDependencies;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Dependencies")
		FModioAPI_AddModDependenciesDelegate OnResponseReceived_AddModDependencies;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Dependencies")
		FModioAPI_DeleteModDependenciesDelegate OnResponseReceived_DeleteModDependencies;

		// Teams

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Teams")
		FModioAPI_GetModTeamMembersDelegate OnResponseReceived_GetModTeamMembers;

		// Reports

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Reports")
		FModioAPI_SubmitReportDelegate OnResponseReceived_SubmitReport;

		// Agreements

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Agreements")
		FModioAPI_GetCurrentAgreementDelegate OnResponseReceived_GetCurrentAgreement;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Agreements")
		FModioAPI_GetAgreementVersionDelegate OnResponseReceived_GetAgreementVersion;

		// Users

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Users")
		FModioAPI_MuteUserDelegate OnResponseReceived_MuteUser;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Users")
		FModioAPI_UnmuteUserDelegate OnResponseReceived_UnmuteUser;

		// Me

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetAuthenticatedUserDelegate OnResponseReceived_GetAuthenticatedUser;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserEventsDelegate OnResponseReceived_GetUserEvents;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserModfilesDelegate OnResponseReceived_GetUserModfiles;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserGamesDelegate OnResponseReceived_GetUserGames;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserSubscriptionsDelegate OnResponseReceived_GetUserSubscriptions;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserModsDelegate OnResponseReceived_GetUserMods;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserPurchasesDelegate OnResponseReceived_GetUserPurchases;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUsersMutedDelegate OnResponseReceived_GetUsersMuted;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserRatingsDelegate OnResponseReceived_GetUserRatings;

		UPROPERTY(BlueprintAssignable, BlueprintCallable, Category = "mod.io API|Events|Me")
		FModioAPI_GetUserWalletDelegate OnResponseReceived_GetUserWallet;

	public:
		UFUNCTION()
		bool InitializeModioAPI(int32 GameID, FString ApiKey, bool AutomaticallyCacheAccessToken, bool StoreAccessTokenPersistingly, FString& Message);

		UFUNCTION(BlueprintPure)
		bool IsInitialized();

		UFUNCTION(BlueprintPure)
		bool IsAuthorized();

		UFUNCTION(BlueprintPure)
		bool HasNetworkConnection();

		UFUNCTION(BlueprintPure)
		bool CachesAccessTokenAutomatically();

		UFUNCTION(BlueprintPure)
		bool StoresCachedAccessTokenPersistingly();

	protected:
		UFUNCTION()
		FString GetApiPath();

		UFUNCTION()
		FString GetApiKey();

		/*
		Persisting Cache
		*/

		UFUNCTION()
		FString GetPersistingCacheFilePath();

		UFUNCTION()
		bool CacheAccessToken(FModioAPI_AccessToken Token, FString& Message);

		UFUNCTION()
		bool SavePersistingCacheToFile(FString& Message);

		UFUNCTION()
		bool LoadPersistingCacheFromFile(FString& Message);

		/*
		Temporary Cache
		*/

		UFUNCTION()
		bool CacheGame(FModioAPI_Game Game, FString& Message);

		UFUNCTION()
		bool CacheTermsOfService(FModioAPI_Terms TermsOfService, FString& Message);

		UFUNCTION()
		bool CacheUser(FModioAPI_User User, FString& Message);

		UFUNCTION()
		bool CacheUserEvent(FModioAPI_UserEvent UserEvent, FString& Message);

		/*
		Requests
		*/
	public:
		// Authentication

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Terms'"))
		bool RequestTermsOfService(FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Steam Authentication'"))
		bool RequestAuthenticationSteam(FModioAPI_SteamAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Xbox Live Authentication'"))
		bool RequestAuthenticationXboxLive(FModioAPI_XboxLiveAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'PlayStation Network Authentication'"))
		bool RequestAuthenticationPlayStationNetwork(FModioAPI_PlaystationNetworkAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Nintendo Switch Authentication'"))
		bool RequestAuthenticationNintendoSwitch(FModioAPI_NintendoSwitchAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Meta Quest Authentication'"))
		bool RequestAuthenticationMetaQuest(FModioAPI_MetaQuestAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Epic Games Authentication'"))
		bool RequestAuthenticationEpicGames(FModioAPI_EpicGamesAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'GOG Galaxy Authentication'"))
		bool RequestAuthenticationGOGGalaxy(FModioAPI_GOGGalaxyAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Google Authentication'"))
		bool RequestAuthenticationGoogle(FModioAPI_GoogleAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Discord Authentication'"))
		bool RequestAuthenticationDiscord(FModioAPI_DiscordAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'OpenID Authentication'"))
		bool RequestAuthenticationOpenID(FModioAPI_OpenIDAuth Authentication, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Email Authentication'", Tooltip = "Requests an Email Login by sending a Security Code to an Email Address.\n(Step 1 of 2 for Email Authentication! Go on with 'Email Exchange')"))
		bool RequestEmailSecurityCode(FString Email, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Email Exchange'", Tooltip = "Requests an Email Login by exchanging the Security Code received via Email to get the Access Token in return.\n(Step 2 of 2 for Email Authentication! Before this you need to do 'Email Authentication')"))
		bool RequestExchangeSecurityCodeForAccessToken(FString SecurityCode, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Authentication", meta = (DisplayName = "Request 'Logout'", Tooltip = "Invalidates the active Access Token", AdvancedDisplay = "AccessToken"))
		bool RequestLogout(FString AccessToken, FString& Message);

		// Games

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Games", meta = (DisplayName = "Request 'Get Game'"))
		bool RequestGetGame(FString& Message);

		// Guides

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Guides", meta = (DisplayName = "Request 'Get Guides'", AdvancedDisplay = "Filters, Sorting, Pagination"))
		bool RequestGetGuides(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Guides", meta = (DisplayName = "Request 'Get Guide'"))
		bool RequestGetGuide(int32 GuideID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Guides", meta = (DisplayName = "Request 'Add Guide'", AdvancedDisplay = "AccessToken"))
		bool RequestAddGuide(FString AccessToken, FModioAPI_AddGuide GuideToAdd, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Guides", meta = (DisplayName = "Request 'Edit Guide'", AdvancedDisplay = "AccessToken"))
		bool RequestEditGuide(FString AccessToken, FModioAPI_EditGuide GuideEdit, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Guides", meta = (DisplayName = "Request 'Delete Guide'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteGuide(FString AccessToken, int32 GuideID, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Guides", meta = (DisplayName = "Request 'Get Guides Tags'"))
		bool RequestGetGuidesTags(FString& Message);

		// Mods

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Mods", meta = (DisplayName = "Request 'Get Mods'", AdvancedDisplay = "Filters, Sorting, Pagination"))
		bool RequestGetMods(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Mods", meta = (DisplayName = "Request 'Get Mod'"))
		bool RequestGetMod(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Mods", meta = (DisplayName = "Request 'Add Mod'", AdvancedDisplay = "AccessToken"))
		bool RequestAddMod(FString AccessToken, FModioAPI_AddMod Mod, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Mods", meta = (DisplayName = "Request 'Edit Mod'", AdvancedDisplay = "AccessToken"))
		bool RequestEditMod(FString AccessToken, FModioAPI_EditMod Edit, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Mods", meta = (DisplayName = "Request 'Delete Mod'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteMod(FString AccessToken, int32 ModID, FString& Message);

		// Files

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files", meta = (DisplayName = "Request 'Get Modfiles'"))
		bool RequestGetModfiles(int32 ModID, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files", meta = (DisplayName = "Request 'Get Modfile'"))
		bool RequestGetModfile(int32 ModID, int32 ModfileID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files", meta = (DisplayName = "Request 'Add Modfile'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModfile(FString AccessToken, FModioAPI_AddModfile Modfile, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files", meta = (DisplayName = "Request 'Edit Modfile'", AdvancedDisplay = "AccessToken"))
		bool RequestEditModfile(FString AccessToken, FModioAPI_EditModfile Edit, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files", meta = (DisplayName = "Request 'Delete Modfile'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteModfile(FString AccessToken, int32 ModID, int32 ModfileID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files", meta = (DisplayName = "Request 'Manage Modfile Platform Status'", AdvancedDisplay = "AccessToken"))
		bool RequestManageModfilePlatformStatus(FString AccessToken, int32 ModID, int32 ModfileID, FString& Message);

		// Files (Multipart Uploads)

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files (Multipart Uploads)", meta = (DisplayName = "Request 'Get Multipart Upload Parts'"))
		bool RequestGetMultipartUploadParts(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files (Multipart Uploads)", meta = (DisplayName = "Request 'Add Multipart Upload Part'", AdvancedDisplay = "AccessToken"))
		bool RequestAddMultipartUploadPart(FString AccessToken, int32 ModID, int32 ModfileID, FString UploadID, int64 ContentRange, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files (Multipart Uploads)", meta = (DisplayName = "Request 'Create Multipart Upload Session'", AdvancedDisplay = "AccessToken"))
		bool RequestCreateMultipartUploadSession(FString AccessToken, int32 ModID, FString Filename, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files (Multipart Uploads)", meta = (DisplayName = "Request 'Delete Multipart Upload Session'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteMultipartUploadSession(FString AccessToken, int32 ModID, FString UploadID, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files (Multipart Uploads)", meta = (DisplayName = "Request 'Get Multipart Upload Sessions'"))
		bool RequestGetMultipartUploadSessions(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Files (Multipart Uploads)", meta = (DisplayName = "Request 'Complete Multipart Upload Session'", AdvancedDisplay = "AccessToken"))
		bool RequestCompleteMultipartUploadSession(FString AccessToken, int32 ModID, FString UploadID, FString& Message);

		// Subscribe

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Subscribe", meta = (DisplayName = "Request 'Subscribe to Mod'", AdvancedDisplay = "AccessToken"))
		bool RequestSubscribeToMod(FString AccessToken, int32 ModID, bool IncludeDependencies, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Subscribe", meta = (DisplayName = "Request 'Unsubscribe from Mod'", AdvancedDisplay = "AccessToken"))
		bool RequestUnsubscribeFromMod(FString AccessToken, int32 ModID, FString& Message);

		// Comments - Guides

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Guides", meta = (DisplayName = "Request 'Get Guide Comments'"))
		bool RequestGetGuideComments(int32 GuideID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Guides", meta = (DisplayName = "Request 'Add Guide Comment'", AdvancedDisplay = "AccessToken"))
		bool RequestAddGuideComment(FString AccessToken, int32 GuideID, FString Content, int32 Reply_ID, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Guides", meta = (DisplayName = "Request 'Get Guide Comment'"))
		bool RequestGetGuideComment(int32 GuideID, int32 CommentID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Guides", meta = (DisplayName = "Request 'Update Guide Comment'", AdvancedDisplay = "AccessToken"))
		bool RequestUpdateGuideComment(FString AccessToken, int32 GuideID, FString CommentID, FString Content, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Guides", meta = (DisplayName = "Request 'Delete Guide Comment'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteGuideComment(FString AccessToken, int32 GuideID, FString CommentID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Guides", meta = (DisplayName = "Request 'Add Guide Comment Karma'", AdvancedDisplay = "AccessToken"))
		bool RequestAddGuideCommentKarma(FString AccessToken, int32 GuideID, FString CommentID, int32 Karma, FString& Message);

		// Comments - Mods

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Mods", meta = (DisplayName = "Request 'Get Mod Comments'"))
		bool RequestGetModComments(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Mods", meta = (DisplayName = "Request 'Add Mod Comment'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModComment(FString AccessToken, int32 ModID, FString Content, int32 Reply_ID, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Mods", meta = (DisplayName = "Request 'Get Mod Comment'"))
		bool RequestGetModComment(int32 ModID, int32 CommentID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Mods", meta = (DisplayName = "Request 'Update Mod Comment'", AdvancedDisplay = "AccessToken"))
		bool RequestUpdateModComment(FString AccessToken, int32 ModID, FString CommentID, FString Content, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Mods", meta = (DisplayName = "Request 'Delete Mod Comment'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteModComment(FString AccessToken, int32 ModID, FString CommentID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Comments|Mods", meta = (DisplayName = "Request 'Add Mod Comment Karma'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModCommentKarma(FString AccessToken, int32 ModID, FString CommentID, int32 Karma, FString& Message);

		// Media

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Media", meta = (DisplayName = "Request 'Add Game Media'", AdvancedDisplay = "AccessToken"))
		bool RequestAddGameMedia(FString AccessToken, FModioAPI_AddGameMedia GameMedia, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Media", meta = (DisplayName = "Request 'Add Mod Media'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModMedia(FString AccessToken, FModioAPI_AddModMedia ModMedia, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Media", meta = (DisplayName = "Request 'Delete Mod Media'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteModMedia(FString AccessToken, int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Media", meta = (DisplayName = "Request 'Reorder Mod Media'", AdvancedDisplay = "AccessToken"))
		bool RequestReorderModMedia(FString AccessToken, int32 ModID, FString& Message);

		// Events

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Events", meta = (DisplayName = "Request 'Get Mods Events'"))
		bool RequestGetModsEvents(FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Events", meta = (DisplayName = "Request 'Get Mod Events'"))
		bool RequestGetModEvents(int32 ModID, FString& Message);

		// Tags

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Get Game Tag Options'"))
		bool RequestGetGameTagOptions(FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Add Game Tag Options'", AdvancedDisplay = "AccessToken"))
		bool RequestAddGameTagOptions(FString AccessToken, FModioAPI_AddGameTagOption Option, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Delete Game Tag Options'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteGameTagOptions(FString AccessToken, FModioAPI_DeleteGameTagOption Option, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Rename Game Tag'", AdvancedDisplay = "AccessToken"))
		bool RequestRenameGameTag(FString AccessToken, FString From, FString To, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Get Mod Tags'"))
		bool RequestGetModTags(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Add Mod Tags'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModTags(FString AccessToken, int32 ModID, TArray<FString> Tags, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Tags", meta = (DisplayName = "Request 'Delete Mod Tags'", AdvancedDisplay = "AccessToken"))
		bool DeleteModTags(FString AccessToken, int32 ModID, TArray<FString> Tags, FString& Message);

		// Ratings

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Ratings", meta = (DisplayName = "Request 'Add Mod Rating'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModRating(FString AccessToken, int32 ModID, EModioAPI_ModRating Rating, FString& Message);

		// Stats

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Stats", meta = (DisplayName = "Request 'Get Game Stats'"))
		bool RequestGetGameStats(FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Stats", meta = (DisplayName = "Request 'Get Mods Stats'"))
		bool RequestGetModsStats(FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Stats", meta = (DisplayName = "Request 'Get Mod Stats'"))
		bool RequestGetModStats(int32 ModID, FString& Message);

		// Metadata

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Metadata", meta = (DisplayName = "Request 'Get Mod KVP Metadata'"))
		bool RequestGetModKVP_Metadata(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Metadata", meta = (DisplayName = "Request 'Add Mod KVP Metadata'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModKVP_Metadata(FString AccessToken, int32 ModID, TArray<FString> Metadata, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Metadata", meta = (DisplayName = "Request 'Delete Mod KVP Metadata'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteModKVP_Metadata(FString AccessToken, int32 ModID, TArray<FString> Metadata, FString& Message);

		// Dependencies

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Dependencies", meta = (DisplayName = "Request 'Get Mod Dependencies'"))
		bool RequestGetModDependencies(int32 ModID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Dependencies", meta = (DisplayName = "Request 'Add Mod Dependencies'", AdvancedDisplay = "AccessToken"))
		bool RequestAddModDependencies(FString AccessToken, int32 ModID, TArray<int32> Dependencies, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Dependencies", meta = (DisplayName = "Request 'Delete Mod Dependencies'", AdvancedDisplay = "AccessToken"))
		bool RequestDeleteModDependencies(FString AccessToken, int32 ModID, TArray<int32> Metadata, FString& Message);

		// Teams

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Teams", meta = (DisplayName = "Request 'Get Mod Team Members'"))
		bool RequestGetModTeamMembers(int32 ModID, FString& Message);

		// Reports

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Reports", meta = (DisplayName = "Request 'Submit Report'", AdvancedDisplay = "AccessToken"))
		bool RequestSubmitReport(FString AccessToken, FModioAPI_SubmitReport Report, FString& Message);

		// Agreements

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Agreements", meta = (DisplayName = "Request 'Get Current Agreement'"))
		bool RequestGetCurrentAgreement(TEnumAsByte<EModioAPI_AgreementType> AgreementType, FString& Message);

		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Agreements", meta = (DisplayName = "Request 'Get Agreement Version'"))
		bool RequestGetAgreementVersion(TEnumAsByte<EModioAPI_AgreementType> AgreementVersion, FString& Message);

		// Users

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Users", meta = (DisplayName = "Request 'Mute a User'", AdvancedDisplay = "AccessToken"))
		bool RequestMuteUser(FString AccessToken, int32 UserID, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Users", meta = (DisplayName = "Request 'Unmute a User'", AdvancedDisplay = "AccessToken"))
		bool RequestUnmuteUser(FString AccessToken, int32 UserID, FString& Message);

		// Me

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get Authenticated User'", AdvancedDisplay = "AccessToken"))
		bool RequestGetAuthenticatedUser(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Events'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserEvents(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Modfiles'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserModfiles(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Games'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserGames(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Subscriptions'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserSubscriptions(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Mods'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserMods(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Purchases'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserPurchases(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get Users Muted'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUsersMuted(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Ratings'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserRatings(FString AccessToken, FString& Message);

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Me", meta = (DisplayName = "Request 'Get User Wallet'", AdvancedDisplay = "AccessToken"))
		bool RequestGetUserWallet(FString AccessToken, FString& Message);

		// General

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|General", meta = (DisplayName = "Request 'Get Ressource Owner'", AdvancedDisplay = "AccessToken"))
		bool RequestGetRessourceOwner(FString AccessToken, FString RessourceType, int32 RessourceID, FString& Message);

		// Checkout

		/*
		Send Request to mod.io API
		@param AccessToken The Access Token used as override. Only necessary if you disabled Automated Caching of Access Token when creating the mod.io API Connection!
		*/
		UFUNCTION(BlueprintCallable, Category = "mod.io API|Requests|Checkout", meta = (DisplayName = "Request 'Purchase An Item'", AdvancedDisplay = "AccessToken"))
		bool RequestPurchaseAnItem(FString AccessToken, int32 DisplayAmount, FString IdempotentKey, FString& Message);

		/*
		Responses
		*/
	private:
		// Authentication

		void TermsOfService_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void SteamAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void XboxLiveAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void PlayStationNetworkAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void NintendoSwitchAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void MetaQuestAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void EpicGamesAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GOGGalaxyAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GoogleAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DiscordAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void OpenIDAuthentication_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void EmailSecurityCode_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void ExchangeForAccessToken_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void Logout_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Games

		void GetGame_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Guides

		void GetGuides_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void EditGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteGuide_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetGuidesTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Mods

		void GetMods_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void EditMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Files

		void GetModfiles_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void EditModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteModfile_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void ManageModfilePlatformStatus_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Files (Multipart Uploads)

		void GetMultipartUploadParts_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddMultipartUploadPart_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void CreateMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetMultipartUploadSessions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void CompleteMultipartUploadSession_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Subscribe

		void SubscribeToMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void UnsubscribeFromMod_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Comments - Guides

		void GetGuideComments_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void UpdateGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteGuideComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddGuideCommentKarma_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Comments - Mods

		void GetModComments_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetModComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void UpdateModComments_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteModComment_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModCommentKarma_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Media

		void AddGameMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteModMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void ReorderModMedia_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Events

		void GetModsEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetModEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Tags

		void GetGameTagOptions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetGameTagOption_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteGameTagOption_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void RenameGameTag_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetModTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteModTags_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Ratings

		void AddModRating_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Stats

		void GetGameStats_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetModsStats_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetModStats_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Metadata

		void GetModMetadataKVP_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModMetadataKVP_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteModMetadataKVP_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Dependencies

		void GetModDependencies_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void AddModDependencies_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void DeleteModDependencies_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Teams

		void GetModTeamMembers_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Reports

		void SubmitReport_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Agreements

		void GetCurrentAgreement_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetAgreementVersion_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Users

		void MuteUser_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void UnmuteUser_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		// Me

		void GetAuthenticatedUser_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserEvents_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserModfiles_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserGames_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserSubscriptions_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserMods_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserPurchases_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUsersMuted_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserRatings_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);
		void GetUserWallet_ResponseReceived(FHttpRequestPtr Request, FHttpResponsePtr Response, bool bConnectedSuccessfully);

		/*
		Get Cached
		*/
	public:
		UFUNCTION(BlueprintPure, Category = "mod.io API|Temp Cache|Authentication", meta = (DisplayName = "Get cached Terms of Service"))
		bool GetCached_TermsOfService(FModioAPI_Terms& TermsOfService);

		UFUNCTION(BlueprintPure, Category = "mod.io API|Temp Cache|Users", meta = (DisplayName = "Get cached Users", AdvancedDisplay = "Filters, Sorting, Pagination"))
		bool GetCached_Users(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_User>& Users);

		UFUNCTION(BlueprintPure, Category = "mod.io API|Temp Cache|Users", meta = (DisplayName = "Get cached User by ID"))
		bool GetCached_User(int32 UserID, FModioAPI_User& User);

		UFUNCTION(BlueprintPure, Category = "mod.io API|Temp Cache|Events", meta = (DisplayName = "Get cached User Events", AdvancedDisplay = "Filters, Sorting, Pagination"))
		bool GetCached_UserEvents(FModioAPI_RequestFilters Filters, FModioAPI_RequestSorting Sorting, FModioAPI_RequestPagination Pagination, TArray<FModioAPI_UserEvent>& UserEvents);

		UFUNCTION(BlueprintPure, Category = "mod.io API|Temp Cache|Events", meta = (DisplayName = "Get cached User Event by ID"))
		bool GetCached_UserEvent(int32 UserEventID, FModioAPI_UserEvent& UserEvent);
};
