/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "AsyncActions/UploadZipAsModfile.h"

void UAsyncAction_UploadZipAsModfile::Activate()
{
	UploadMessage = FModioAPI_UploadModfileMultipartMessage();

	if (!ModioConnection)
	{
		UploadMessage.ErrorMessage = "Modio Connection is invalid / missing!";
		Error.Broadcast(UploadMessage);
		return;
	}

	if (!ModioConnection->IsInitialized())
	{
		UploadMessage.ErrorMessage = "Modio Connection is not initialized!";
		Error.Broadcast(UploadMessage);
		return;
	}

	if (ModID <= 0)
	{
		UploadMessage.ErrorMessage = "ModID is invalid!";
		Error.Broadcast(UploadMessage);
		return;
	}

	if (ZipArchiveFilePath.IsEmpty())
	{
		UploadMessage.ErrorMessage = "Zip Archive File Path is empty!";
		Error.Broadcast(UploadMessage);
		return;
	}

	if (!IFileManager::Get().FileExists(*ZipArchiveFilePath))
	{
		UploadMessage.ErrorMessage = "File Path not pointing to a valid File!";
		Error.Broadcast(UploadMessage);
		return;
	}

	if (FPaths::GetExtension(*ZipArchiveFilePath, false).ToLower() != "zip")
	{
		UploadMessage.ErrorMessage = "File Path not pointing to a Zip-File!";
		Error.Broadcast(UploadMessage);
		return;
	}

	// Max File Size = 20 GB
	TotalFileSize = IFileManager::Get().FileSize(*ZipArchiveFilePath);
	if (TotalFileSize > 21474836480)
	{
		UploadMessage.ErrorMessage = "Zip File is too large! Max. File Size is 20 GB / 21.474.836.480 Bytes";
		Error.Broadcast(UploadMessage);
		return;
	}

	// Create Multipart Upload Session
	if (AccessToken.IsEmpty())
	{
		AccessToken = ModioConnection->GetPersistingCacheAccessToken().AccessToken;

		if (AccessToken.IsEmpty())
		{
			UploadMessage.ErrorMessage = "No Access Token cached or directly provided!";
			Error.Broadcast(UploadMessage);
			return;
		}
	}

	ModioConnection->OnResponseReceived_CreateMultipartUploadSession.AddDynamic(this, &UAsyncAction_UploadZipAsModfile::SessionCreated);

	FString FileNameString = FileName.ToString();
	if (!FileNameString.ToLower().EndsWith(".zip"))
	{
		FileNameString.Append(".zip");
	}

	ModioConnection->RequestCreateMultipartUploadSession(AccessToken, ModID, FileNameString, "", Message);
	return;
}

void UAsyncAction_UploadZipAsModfile::UploadPart(int32 PartNumber)
{
	if (PartNumber < 1)
	{
		UploadMessage.ErrorMessage = "Invalid Upload Part Number! Part Numbers start at 1, not at 0! This is not an Index!";
		Error.Broadcast(UploadMessage);
		return;
	}

	int64 ContentRangeStart = UModioAPIFunctionLibrary::GetContentRangeStartForUploadPartNumber(PartNumber);
	int64 ContentRangeEnd = UModioAPIFunctionLibrary::GetContentRangeEndForUploadSessionOnPartNumber(UploadPartsFilePaths, PartNumber);
	FString FileToUpload = UModioAPIFunctionLibrary::FindUploadPartFileByPartNumber(UploadPartsFilePaths, PartNumber);

	if (!FileToUpload.IsEmpty())
	{
		TArray<uint8> FileData;
		FFileHelper::LoadFileToArray(FileData, *FileToUpload);

		ModioConnection->OnResponseReceived_AddMultipartUploadPart.AddDynamic(this, &UAsyncAction_UploadZipAsModfile::PartUploaded);
		ModioConnection->RequestAddMultipartUploadPart(AccessToken, ModID, UploadSessionID, ContentRangeStart, ContentRangeEnd, TotalFileSize, FileData, Message);
	}
	else
	{
		UploadMessage.ErrorMessage = "Error finding the Upload Part File!";
		Error.Broadcast(UploadMessage);
	}
}

void UAsyncAction_UploadZipAsModfile::PartUploaded(FModioAPI_MultipartUploadPart UploadedPart, FModioAPI_Error_Object ErrorObject)
{
	// Catch all the remaining Bound Delegates...
	if (UploadedPart.Part_Number != CurrentUploadPart)
	{
		return;
	}

	// Check for Error
	if (ErrorObject.Error.Code != 0)
	{
		UploadMessage.ErrorMessage = "Error when uploading a Part to the Upload Session!";
		Error.Broadcast(UploadMessage);
		return;
	}

	if (!UploadedPart.Upload_ID.IsEmpty())
	{
		// Upload Progress
		UploadMessage.UploadProgress.PartsUploaded = CurrentUploadPart;
		UploadMessage.UploadProgress.PartsTotal = UploadPartsFilePaths.Num();

		if (UploadMessage.UploadProgress.PartsUploaded >= UploadMessage.UploadProgress.PartsTotal)
		{
			UploadMessage.UploadProgress.BytesUploaded = TotalFileSize;
		}
		else
		{
			UploadMessage.UploadProgress.BytesUploaded = CurrentUploadPart * 52428800;
		}

		UploadMessage.UploadProgress.BytesTotal = TotalFileSize;
		UploadMessage.UploadProgress.Progress = ((float)UploadMessage.UploadProgress.BytesUploaded) / ((float)TotalFileSize);

		UploadProgress.Broadcast(UploadMessage);

		// Start the next Upload Part, if this wasn't the last one!
		if (CurrentUploadPart < TotalUploadParts)
		{
			CurrentUploadPart++;
			UploadPart(CurrentUploadPart);
		}
		else
		{
			// If that was the last Part, complete the Multipart Upload Session!
			// Complete Multipart Upload Session
			ModioConnection->OnResponseReceived_CompleteMultipartUploadSession.AddDynamic(this, &UAsyncAction_UploadZipAsModfile::SessionCompleted);
			ModioConnection->RequestCompleteMultipartUploadSession(AccessToken, ModID, UploadSessionID, Message);
		}

		return;
	}
}

void UAsyncAction_UploadZipAsModfile::SessionCreated(FModioAPI_MultipartUpload CreatedMultipartUpload, FModioAPI_Error_Object ErrorObject)
{
	// Check for Error
	if (ErrorObject.Error.Code != 0)
	{
		UploadMessage.ErrorMessage = "Error when creating a Multipart Upload Session!";
		Error.Broadcast(UploadMessage);
		return;
	}

	UploadSessionID = CreatedMultipartUpload.Upload_ID;
	UploadMessage.ErrorMessage.Empty();
	UploadMessage.UploadSessionID = UploadSessionID;
	UploadSessionCreated.Broadcast(UploadMessage);

	// Split Zip-File to 50 MB Parts ready for Upload
	ModioConnection->PrepareZipArchiveForMultipartUploadSession(ZipArchiveFilePath, UploadSessionID, Success, Message, TotalFileSize);

	if (!Success)
	{
		UploadMessage.ErrorMessage = "Error when preparing the Zip File for Upload by spliting the File into 50MB Upload Parts!";
		Error.Broadcast(UploadMessage);
		return;
	}

	FString UploadSessionDirectoryPath = ModioConnection->GetDirectoryPathForUploadSession(UploadSessionID);
	if (UploadSessionDirectoryPath.IsEmpty())
	{
		UploadMessage.ErrorMessage = "Error finding the Upload Session Directory containing the Upload Parts!";
		Error.Broadcast(UploadMessage);
		return;
	}

	// Find the Files for Uploading
	TArray<FString> UploadPartsRelativeFilePaths;
	IFileManager::Get().FindFiles(UploadPartsRelativeFilePaths, *UploadSessionDirectoryPath);
	
	UploadPartsFilePaths.Empty();
	for (FString RelativePath : UploadPartsRelativeFilePaths)
	{
		UploadPartsFilePaths.AddUnique(ModioConnection->GetDirectoryPathForUploadSession(UploadSessionID) + RelativePath);
	}
	TotalUploadParts = UploadPartsFilePaths.Num();

	if (UploadPartsFilePaths.IsEmpty())
	{
		UploadMessage.ErrorMessage = "Error finding any Upload Parts in the Upload Session Directory!";
		Error.Broadcast(UploadMessage);
		return;
	}

	// Upload First Part meaning in this case the first one (Part Number 1, not starting at 0!)...
	CurrentUploadPart = 1;
	UploadPart(CurrentUploadPart);
	return;
}

void UAsyncAction_UploadZipAsModfile::SessionCompleted(FModioAPI_MultipartUpload CompletedMultipartUpload, FModioAPI_Error_Object ErrorObject)
{
	if (HasCompletedSession)
	{
		return;
	}

	// Check for Error
	if (ErrorObject.Error.Code != 0)
	{
		// Filter out this special case... Gives me headaches...
		if (ErrorObject.Error.Code != 400 && ErrorObject.Error.Error_Ref != 29003)
		{
			UploadMessage.ErrorMessage = "Error when creating a Multipart Upload Session!";
			Error.Broadcast(UploadMessage);
			return;
		}
	}

	if (!CompletedMultipartUpload.Upload_ID.IsEmpty())
	{
		HasCompletedSession = true;
		UploadSessionCompleted.Broadcast(UploadMessage);

		// Request Adding the Upload Session ID as Modfile
		ModioConnection->OnResponseReceived_AddModfile.AddDynamic(this, &UAsyncAction_UploadZipAsModfile::ModfileAddedFromUploadSession);

		FModioAPI_AddModfile AddModfile;
		AddModfile.ModfileName = FileName;
		AddModfile.UploadID = UploadSessionID;
		AddModfile.Version = Version;
		AddModfile.Changelog = Changelog;
		AddModfile.Active = Active;
		AddModfile.MetadataBlob = Metadata_Blob;
		AddModfile.Platforms = Platforms;
		ModioConnection->RequestAddModfile(AccessToken, ModID, AddModfile, Message);
	}
}

void UAsyncAction_UploadZipAsModfile::ModfileAddedFromUploadSession(FModioAPI_Modfile AddedModfile, FModioAPI_Error_Object ErrorObject)
{
	if (ModfileID > 0)
	{
		return;
	}

	// Check for Errors
	if (ErrorObject.Error.Code != 0)
	{
		UploadMessage.ErrorMessage = "Error when adding the Multipart Upload Session as a Modfile!";
		Error.Broadcast(UploadMessage);
		return;
	}

	ModfileID = AddedModfile.ID;
	UploadMessage.AddedModfile = AddedModfile;
	ModfileAdded.Broadcast(UploadMessage);

	// Cleanup File Storage
	if (!ModioConnection->CleanupUploadSession(UploadSessionID, Message))
	{
		UploadMessage.ErrorMessage = "Error when cleaning up the Multipart Upload Session in Cache!";
		Error.Broadcast(UploadMessage);
		return;
	}
}

UAsyncAction_UploadZipAsModfile* UAsyncAction_UploadZipAsModfile::AsyncActionUploadZipAsModfile(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken, int32 ModID, FString ZipArchiveFilePath, FName FileName, FString Version, FString Changelog, bool Active, FString Metadata_Blob, TArray<TEnumAsByte<EModioAPI_Platforms>> Platforms)
{
	// Create Action Instance for Blueprint System
	UAsyncAction_UploadZipAsModfile* Action = NewObject<UAsyncAction_UploadZipAsModfile>();
	Action->ModioConnection = ModioConnection;
	Action->AccessToken = AccessToken;
	Action->ModID = ModID;
	Action->ZipArchiveFilePath = ZipArchiveFilePath;
	Action->FileName = FileName;
	Action->Version = Version;
	Action->Changelog = Changelog;
	Action->Active = Active;
	Action->Metadata_Blob = Metadata_Blob;
	Action->Platforms = Platforms;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}