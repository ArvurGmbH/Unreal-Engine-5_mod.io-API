/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ModioAPIObject.h"
#include "FileToStorageDownloader.h"
#include "DownloadGameIcon.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_OnDownloadGameIcon, FModioAPI_DownloadIconMessage, Message);

/**
 * 
 */
UCLASS()
class MODIOAPI_API UAsyncAction_DownloadGameIcon : public UBlueprintAsyncActionBase
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Download Game Icon to Cache",BlueprintInternalUseOnly = "true", Category = "mod.io API|Async Actions", WorldContext = "WorldContextObject", AdvancedDisplay = "AccessToken"))
	static UAsyncAction_DownloadGameIcon* AsyncActionDownloadGameIcon(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken);
	
	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameIcon Error;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameIcon Started;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameIcon Progress;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameIcon Completed;

	UModioAPIObject* ModioConnection;
	FString AccessToken;
	UFileToStorageDownloader* Downloader;
	FModioAPI_Icon_Object IconObject;

	FModioAPI_DownloadIconMessage DownloadMessage;
};
