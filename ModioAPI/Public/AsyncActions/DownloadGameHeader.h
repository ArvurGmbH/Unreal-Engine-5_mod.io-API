/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ModioAPIObject.h"
#include "FileToStorageDownloader.h"
#include "DownloadGameHeader.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_OnDownloadGameHeader, FModioAPI_DownloadHeaderMessage, Message);

/**
 * 
 */
UCLASS()
class MODIOAPI_API UAsyncAction_DownloadGameHeader : public UBlueprintAsyncActionBase
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Download Game Header to Cache",BlueprintInternalUseOnly = "true", Category = "mod.io API|Async Actions", WorldContext = "WorldContextObject", AdvancedDisplay = "AccessToken"))
	static UAsyncAction_DownloadGameHeader* AsyncActionDownloadGameHeader(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken);
	
	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameHeader Error;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameHeader Started;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameHeader Progress;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadGameHeader Completed;

	UModioAPIObject* ModioConnection;
	FString AccessToken;
	UFileToStorageDownloader* Downloader;
	FModioAPI_HeaderImage_Object HeaderObject;

	FModioAPI_DownloadHeaderMessage DownloadMessage;
};
