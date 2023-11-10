/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ModioAPIObject.h"
#include "FileToStorageDownloader.h"
#include "DownloadModfile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_OnDownloadModfile, FModioAPI_DownloadModfileMessage, Message);

/**
 * 
 */
UCLASS()
class MODIOAPI_API UAsyncAction_DownloadModfile : public UBlueprintAsyncActionBase
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Download Modfile to Cache",BlueprintInternalUseOnly = "true", Category = "mod.io API|Async Actions", WorldContext = "WorldContextObject", AdvancedDisplay = "AccessToken"))
	static UAsyncAction_DownloadModfile* AsyncActionDownloadModfile(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken, FModioAPI_Modfile Modfile);
	
	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadModfile Error;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadModfile Started;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadModfile Progress;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadModfile Completed;

	UModioAPIObject* ModioConnection;
	FString AccessToken;
	FModioAPI_Modfile Modfile;
	UFileToStorageDownloader* Downloader;

	FModioAPI_DownloadModfileMessage DownloadMessage;
};
