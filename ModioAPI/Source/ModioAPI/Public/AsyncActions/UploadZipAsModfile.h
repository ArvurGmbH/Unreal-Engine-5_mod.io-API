/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintAsyncActionBase.h"
#include "ModioAPIObject.h"
#include "UploadZipAsModfile.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FModioAPI_OnUploadZipAsModfile, FModioAPI_UploadModfileMultipartMessage, Message);

/**
 * 
 */
UCLASS()
class MODIOAPI_API UAsyncAction_UploadZipAsModfile : public UBlueprintAsyncActionBase
{
	GENERATED_BODY()

protected:
	UFUNCTION()
	void SessionCreated(FModioAPI_MultipartUpload CreatedMultipartUpload, FModioAPI_Error_Object ErrorObject);

	UFUNCTION()
	void UploadPart(int32 PartNumber);

	UFUNCTION()
	void PartUploaded(FModioAPI_MultipartUploadPart UploadedPart, FModioAPI_Error_Object ErrorObject);

	UFUNCTION()
	void SessionCompleted(FModioAPI_MultipartUpload CompletedMultipartUpload, FModioAPI_Error_Object ErrorObject);

	UFUNCTION()
	void ModfileAddedFromUploadSession(FModioAPI_Modfile AddedModfile, FModioAPI_Error_Object ErrorObject);

public:

	/** Execute the actual Action */
	virtual void Activate() override;

	/** Used for the creation of the Async Action Blueprint Node */

	UFUNCTION(BlueprintCallable, meta = (DisplayName = "Upload Zip as Modfile",BlueprintInternalUseOnly = "true", Category = "mod.io API|Async Actions", WorldContext = "WorldContextObject", AdvancedDisplay = "AccessToken"))
	static UAsyncAction_UploadZipAsModfile* AsyncActionUploadZipAsModfile(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken, int32 ModID, FString ZipArchiveFilePath, FName FileName, FString Version, FString Changelog, bool Active, FString Metadata_Blob, TArray<TEnumAsByte<EModioAPI_Platforms>> Platforms);

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnUploadZipAsModfile Error;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnUploadZipAsModfile UploadSessionCreated;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnUploadZipAsModfile UploadProgress;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnUploadZipAsModfile UploadSessionCompleted;

	UPROPERTY(BlueprintAssignable)
	FModioAPI_OnUploadZipAsModfile ModfileAdded;

	UModioAPIObject* ModioConnection;
	FString AccessToken;
	int32 ModID;
	FString ZipArchiveFilePath;
	FName FileName;
	FModioAPI_EditModfile ModfileInfo;
	FModioAPI_ManageModfilePlatformStatus PlatformsStatus;
	FString Version;
	FString Changelog;
	bool Active;
	FString Metadata_Blob;
	TArray<TEnumAsByte<EModioAPI_Platforms>> Platforms;

	FModioAPI_UploadModfileMultipartMessage UploadMessage;
	bool Success;
	FString Message;
	int64 TotalFileSize;
	FString UploadSessionID;
	TArray<FString> UploadPartsFilePaths;
	int32 TotalUploadParts;
	int32 CurrentUploadPart = 1;
	bool HasCompletedSession = false;
	int32 ModfileID;
};
