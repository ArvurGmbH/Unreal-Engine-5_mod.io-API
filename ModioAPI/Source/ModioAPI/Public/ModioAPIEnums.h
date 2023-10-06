/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

UENUM(BlueprintType, DisplayName = "mod.io Meta Quest - Device Type", Category = "mod.io API", meta = (Tooltip = "Meta Quest Device Type"))
enum EModioAPI_MetaQuest_DeviceType
{
	MetaQuest_DeviceType_Rift		UMETA(DisplayName = "Rift"),
	MetaQuest_DeviceType_Quest		UMETA(DisplayName = "Quest"),
};

UENUM(BlueprintType, DisplayName = "mod.io Game Status", Category = "mod.io API", meta = (Tooltip = "Game Status"))
enum EModioAPI_GameGuideModStatus
{
	GameStatus_NotAccepted		UMETA(DisplayName = "0 = Not Accepted"),
	GameStatus_Accepted			UMETA(DisplayName = "1 = Accepted"),
	GameStatus_Deleted			UMETA(DisplayName = "3 = Deleted"),
};

UENUM(BlueprintType, DisplayName = "mod.io Game Presentation", Category = "mod.io API", meta = (Tooltip = "Game Presentation"))
enum EModioAPI_GamePresentation
{
	GamePresentation_GridView		UMETA(DisplayName = "0 = Grid View"),
	GamePresentation_TableView		UMETA(DisplayName = "1 = Table View"),
};

UENUM(BlueprintType, DisplayName = "mod.io Game Mods Submission", Category = "mod.io API", meta = (Tooltip = "From where can Mods be submitted?"))
enum EModioAPI_GameModsSubmission
{
	GameModsSubmission_ToolsAPIOnly		UMETA(DisplayName = "0 = Tools & API Only"),
	GameModsSubmission_Anywhere			UMETA(DisplayName = "1 = Anywhere"),
};

UENUM(BlueprintType, DisplayName = "mod.io Game Mods Curation", Category = "mod.io API", meta = (Tooltip = "What about Mods is curated / needs approval?"))
enum EModioAPI_GameModsCuration
{
	GameModsCuration_NoCuration			UMETA(DisplayName = "0 = No Curation"),
	GameModsCuration_PriceChanges		UMETA(DisplayName = "1 = Price Change Approval"),
	GameModsCuration_FullCuration		UMETA(DisplayName = "2 = Full Curation"),
};

UENUM(BlueprintType, DisplayName = "mod.io Game API Access Option", Category = "mod.io API")
enum EModioAPI_API_AccessOption
{
	API_AccessOption_Disabled					UMETA(DisplayName = "0 = Disabled"),
	API_AccessOption_AllowThirdParties			UMETA(DisplayName = "1 = Allow 3rd Parties access"),
	API_AccessOption_AllowDirectDownloads		UMETA(DisplayName = "2 = Allow direct Downloads"),
};

UENUM(BlueprintType, DisplayName = "mod.io Game Maturity Option", Category = "mod.io API")
enum EModioAPI_MaturityOption
{
	MaturityOption_DontAllowMatureContent		UMETA(DisplayName = "0 = No mature Content"),
	MaturityOption_AllowMatureContent			UMETA(DisplayName = "1 = Allow mature Content"),
	MaturityOption_MatureAudiencesOnly			UMETA(DisplayName = "2 = Mature audiences only"),
};

UENUM(BlueprintType, DisplayName = "mod.io Mod Visibility", Category = "mod.io API", meta = (Tooltip = "Mod Visibility"))
enum EModioAPI_ModVisibility
{
	ModVisibility_Hidden			UMETA(DisplayName = "0 = Hidden"),
	ModVisibility_Public			UMETA(DisplayName = "1 = Public"),
};

UENUM(BlueprintType, DisplayName = "mod.io Modfile Virus Status", Category = "mod.io API", meta = (Tooltip = "Modfile Virus Status"))
enum EModioAPI_ModfileVirusStatus
{
	ModfileVirusStatus_NotScanned		UMETA(DisplayName = "0 = Not scanned"),
	ModfileVirusStatus_ScanComplete		UMETA(DisplayName = "1 = Scan complete"),
	ModfileVirusStatus_InProgress		UMETA(DisplayName = "2 = In progress"),
	ModfileVirusStatus_TooLargeToScan	UMETA(DisplayName = "3 = Too large to scan"),
	ModfileVirusStatus_FileNotFound		UMETA(DisplayName = "4 = File not found"),
	ModfileVirusStatus_ErrorScanning	UMETA(DisplayName = "5 = Error Scanning"),
};

UENUM(BlueprintType, DisplayName = "mod.io Modfile Virus Positivity", Category = "mod.io API", meta = (Tooltip = "Modfile Virus Positive"))
enum EModioAPI_ModfileVirusPositivity
{
	ModfileVirusPositive_NoThreats					UMETA(DisplayName = "0 = No threats detected"),
	ModfileVirusPositive_Malicious					UMETA(DisplayName = "1 = Flagged as malicious"),
	ModfileVirusPositive_ContainingHarmfulFiles		UMETA(DisplayName = "2 = Flagged as containing harmful files"),
};

UENUM(BlueprintType, DisplayName = "mod.io Modfile Platform Status", Category = "mod.io API", meta = (Tooltip = "Modfile Platform Status"))
enum EModioAPI_ModfilePlatformStatus
{
	ModfilePlatformStatus_Pending		UMETA(DisplayName = "0 = Pending"),
	ModfilePlatformStatus_Approved		UMETA(DisplayName = "1 = Approved"),
	ModfilePlatformStatus_Denied		UMETA(DisplayName = "2 = Denied"),
	ModfilePlatformStatus_Targetted		UMETA(DisplayName = "3 = Targetted"),
};

UENUM(BlueprintType, DisplayName = "mod.io Mod Rating", Category = "mod.io API", meta = (Tooltip = "Mod Rating"))
enum EModioAPI_ModRating
{
	ModRating_Positive			UMETA(DisplayName = "1 = Positive (Thumbs up)"),
	ModRating_Neutral			UMETA(DisplayName = "0 = Remove previous Rating"),
	ModRating_Negative			UMETA(DisplayName = "-1 = Negative (Thumbs down)"),
};

UENUM(BlueprintType, DisplayName = "mod.io Mod Rating", Category = "mod.io API", meta = (Tooltip = "Mod Rating"))
enum EModioAPI_AgreementType
{
	AgreementType_TermsOfUse				UMETA(DisplayName = "1 = Terms of Use"),
	AgreementType_PrivacyPolicy				UMETA(DisplayName = "2 = Privacy Policy"),
	AgreementType_GameTerms					UMETA(DisplayName = "3 = Game Terms"),
	AgreementType_ApiAccessTerms			UMETA(DisplayName = "4 = API Access Terms"),
	AgreementType_MonetizationTerms			UMETA(DisplayName = "5 = Monetization Terms"),
	AgreementType_AcceptableUsePolicy		UMETA(DisplayName = "6 = Acceptable Use Policy"),
	AgreementType_CookiesPolicy				UMETA(DisplayName = "7 = Cookies Policy"),
};

UENUM(BlueprintType, DisplayName = "mod.io Mod Event Type", Category = "mod.io API", meta = (Tooltip = "Mod Event Type"))
enum EModioAPI_ModEventType
{
	ModEventType_ModfileChanged		UMETA(DisplayName = "Modfile Changed"),
	ModEventType_ModAvailable		UMETA(DisplayName = "Mod Available"),
	ModEventType_ModUnavailable		UMETA(DisplayName = "Mod Unavailable"),
	ModEventType_ModEdited			UMETA(DisplayName = "Mod Edited"),
	ModEventType_ModDeleted			UMETA(DisplayName = "Mod Deleted"),
	ModEventType_ModTeamChanged		UMETA(DisplayName = "Mod Team Changed"),
};

UENUM(BlueprintType, DisplayName = "mod.io Multipart Upload Status", Category = "mod.io API", meta = (Tooltip = "Multipart Upload Status"))
enum EModioAPI_MultipartUploadStatus
{
	MultipartUploadStatus_Incomplete	UMETA(DisplayName = "0 = Incomplete"),
	MultipartUploadStatus_Pending		UMETA(DisplayName = "1 = Pending"),
	MultipartUploadStatus_Processing	UMETA(DisplayName = "2 = Processing"),
	MultipartUploadStatus_Complete		UMETA(DisplayName = "3 = Complete"),
	MultipartUploadStatus_Cancelled		UMETA(DisplayName = "4 = Cancelled"),
};

UENUM(BlueprintType, DisplayName = "mod.io User Event Type", Category = "mod.io API", meta = (Tooltip = "User Event Type"))
enum EModioAPI_UserEventType
{
	UserEventType_TeamJoin			UMETA(DisplayName = "Team Join"),
	UserEventType_TeamLeave			UMETA(DisplayName = "Team Leave"),
	UserEventType_Subscribe			UMETA(DisplayName = "Subscribe"),
	UserEventType_Unsubscribe		UMETA(DisplayName = "Unsubscribe"),
};

UENUM(BlueprintType, DisplayName = "mod.io Rating Type", Category = "mod.io API", meta = (Tooltip = "Rating Type"))
enum EModioAPI_RatingType
{
	RatingType_Positive			UMETA(DisplayName = "Positive Rating"),
	RatingType_Negative			UMETA(DisplayName = "Negative Negative"),
};

UENUM(BlueprintType, DisplayName = "mod.io Filter Comparison", Category = "mod.io API|Filtering", meta = (Tooltip = "Filter Type"))
enum EModioAPI_FilterComparison
{
	FilterComparison_FullTextSearch		UMETA(DisplayName = "Full Text Search"),
	FilterComparison_Equals				UMETA(DisplayName = "Equals"),
	FilterComparison_NotEqualTo			UMETA(DisplayName = "Not Equal To"),
	FilterComparison_Like				UMETA(DisplayName = "Like"),
	FilterComparison_NotLike			UMETA(DisplayName = "Not Like"),
	FilterComparison_In					UMETA(DisplayName = "In"),
	FilterComparison_Max				UMETA(DisplayName = "Max"),
	FilterComparison_Min				UMETA(DisplayName = "Min"),
};

UENUM(BlueprintType, DisplayName = "mod.io Sorting Order", Category = "mod.io API|Sorting", meta = (Tooltip = "Order for Sorting"))
enum EModioAPI_SortingOrder
{
	SortingOrder_Ascending				UMETA(DisplayName = "Ascending"),
	SortingOrder_Descending				UMETA(DisplayName = "Descending"),
};