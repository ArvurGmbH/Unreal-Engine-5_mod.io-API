/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "AsyncActions/DownloadModfile.h"


void UAsyncAction_DownloadModfile::DownloadStarted(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio)
{
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.ProgressInfo.BytesReceived = DownloadBytesReceived;
	DownloadMessage.ProgressInfo.BytesTotal = DownloadBytesTotal;
	DownloadMessage.ProgressInfo.Progress = DownloadProgressRatio;
	DownloadMessage.ErrorMessage = "Download started!";
	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Running;
	Started.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadModfile::DownloadProgress(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio)
{
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.ProgressInfo.BytesReceived = DownloadBytesReceived;
	DownloadMessage.ProgressInfo.BytesTotal = DownloadBytesTotal;
	DownloadMessage.ProgressInfo.Progress = DownloadProgressRatio;
	DownloadMessage.ErrorMessage = "Downloading...";
	Progress.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadModfile::DownloadCompleted(EDownloadToStorageResult DownloadResult)
{
	switch (DownloadResult)
	{
		case EDownloadToStorageResult::Success:
			DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_CompletedSuccessfully;
			DownloadMessage.ErrorMessage = "Download Successful!";
			break;
		case EDownloadToStorageResult::Cancelled:
			DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Cancelled;
			DownloadMessage.ErrorMessage = "Download Cancelled!";
			break;
		case EDownloadToStorageResult::DownloadFailed:
		default:
			DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_CompletedFailed;
			DownloadMessage.ErrorMessage = "Download Failed!";
			break;
	}

	Completed.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadModfile::Activate()
{
	DownloadMessage = FModioAPI_DownloadModfileMessage();
	DownloadMessage.Modfile = Modfile;
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_CompletedFailed;

	if (!ModioConnection)
	{
		DownloadMessage.ErrorMessage = "Modio Connection is invalid / missing!";
		Error.Broadcast(DownloadMessage);
		return;
	}

	if (!ModioConnection->IsInitialized())
	{
		DownloadMessage.ErrorMessage = "Modio Connection is not initialized!";
		Error.Broadcast(DownloadMessage);
		return;
	}

	if (Modfile.Mod_ID <= 0)
	{
		DownloadMessage.ErrorMessage = "ModID is invalid!";
		Error.Broadcast(DownloadMessage);
		return;
	}

	if (Modfile.ID <= 0)
	{
		DownloadMessage.ErrorMessage = "ModfileID is invalid!";
		Error.Broadcast(DownloadMessage);
		return;
	}

	FString FileDownloadURL = ModioConnection->GetApiPath() + ModioConnection->EndpointGames + "/" + FString::FromInt(ModioConnection->ModioGameID) + ModioConnection->EndpointMods + "/" + FString::FromInt(Modfile.Mod_ID) + ModioConnection->EndpointFiles + "/" + FString::FromInt(Modfile.ID) + ModioConnection->EndpointDownload;
	FString FileDownloadPath = ModioConnection->GetModfileDirectoryPathForModfile(Modfile.Mod_ID, Modfile.ID) + Modfile.Filename;

	// Add Progress & Completed Events
	OnStarted.BindUFunction(this, "DownloadStarted");
	OnProgress.BindUFunction(this, "DownloadProgress");
	OnCompleted.BindUFunction(this, "DownloadCompleted");

	DownloadStarted(-1, -1, 0);

	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Running;
	Downloader = UFileToStorageDownloader::DownloadFileToStorage(FileDownloadURL, FileDownloadPath, 0, "", false, OnProgress, OnCompleted);
}

UAsyncAction_DownloadModfile* UAsyncAction_DownloadModfile::AsyncActionDownloadModfile(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken, FModioAPI_Modfile Modfile)
{
	// Create Action Instance for Blueprint System
	UAsyncAction_DownloadModfile* Action = NewObject<UAsyncAction_DownloadModfile>();
	Action->ModioConnection = ModioConnection;
	Action->AccessToken = AccessToken;
	Action->Modfile = Modfile;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}