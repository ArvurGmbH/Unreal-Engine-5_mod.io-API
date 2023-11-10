/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ModioAPIObject.h"
#include "FileToStorageDownloader.h"
#include "DownloadActiveModfileForPlatform.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_OnDownloadActiveModfileForPlatform, FModioAPI_DownloadModfileMessage, Message);

/**
 * 
 */
UCLASS()
class MODIOAPI_API UAsyncAction_DownloadActiveModfileForPlatform : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()
protected:
	FOnDownloadProgressNative OnStarted;
	FOnDownloadProgressNative OnProgress;
	FOnFileToStorageDownloadCompleteNative OnCompleted;
	
	UFUNCTION()
	void DownloadStarted(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio);

	UFUNCTION()
	void DownloadProgress(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio);

	UFUNCTION()
	void DownloadCompleted(EDownloadToStorageResult DownloadResult);

public:

	/** Execute the actual Action */
	virtual void Activate() override;

	/** Used for the creation of the Async Action Blueprint Node */

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Download Active Modfile for Platform to Cache",BlueprintInternalUseOnly = "true", Category = "mod.io API|Async Actions", WorldContext = "WorldContextObject", AdvancedDisplay = "AccessToken"))
	static UAsyncAction_DownloadActiveModfileForPlatform* AsyncActionDownloadActiveModfileForPlatform(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken, FModioAPI_Mod Mod, TEnumAsByte<EModioAPI_Platforms> Platform);
	
	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadActiveModfileForPlatform Error;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadActiveModfileForPlatform Started;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadActiveModfileForPlatform Progress;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadActiveModfileForPlatform Completed;

	UModioAPIObject* ModioConnection;
	FString AccessToken;
	FModioAPI_Mod Mod;
	TEnumAsByte<EModioAPI_Platforms> Platform;
	UFileToStorageDownloader* Downloader;

	FModioAPI_DownloadModfileMessage DownloadMessage;
};
