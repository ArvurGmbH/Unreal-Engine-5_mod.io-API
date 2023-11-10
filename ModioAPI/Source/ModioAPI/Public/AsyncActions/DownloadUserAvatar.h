/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ModioAPIObject.h"
#include "FileToStorageDownloader.h"
#include "DownloadUserAvatar.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_OnDownloadUserAvatar, FModioAPI_DownloadAvatarMessage, Message);

/**
 * 
 */
UCLASS()
class MODIOAPI_API UAsyncAction_DownloadUserAvatar : public UBlueprintAsyncActionBase
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

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Download User Avatar to Cache",BlueprintInternalUseOnly = "true", Category = "mod.io API|Async Actions", WorldContext = "WorldContextObject", AdvancedDisplay = "AccessToken"))
	static UAsyncAction_DownloadUserAvatar* AsyncActionDownloadUserAvatar(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken, FModioAPI_User User);
	
	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadUserAvatar Error;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadUserAvatar Started;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadUserAvatar Progress;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnDownloadUserAvatar Completed;

	UModioAPIObject* ModioConnection;
	FString AccessToken;
	UFileToStorageDownloader* Downloader;
	FModioAPI_User User;
	FModioAPI_Avatar_Object AvatarObject;

	FModioAPI_DownloadAvatarMessage DownloadMessage;
};
