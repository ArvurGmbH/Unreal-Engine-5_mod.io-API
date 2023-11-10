/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "AsyncActions/DownloadGameIcon.h"


void UAsyncAction_DownloadGameIcon::DownloadStarted(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio)
{
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.ProgressInfo.BytesReceived = DownloadBytesReceived;
	DownloadMessage.ProgressInfo.BytesTotal = DownloadBytesTotal;
	DownloadMessage.ProgressInfo.Progress = DownloadProgressRatio;
	DownloadMessage.ErrorMessage = "Download started!";
	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Running;
	DownloadMessage.Icon = IconObject;
	Started.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadGameIcon::DownloadProgress(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio)
{
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.ProgressInfo.BytesReceived = DownloadBytesReceived;
	DownloadMessage.ProgressInfo.BytesTotal = DownloadBytesTotal;
	DownloadMessage.ProgressInfo.Progress = DownloadProgressRatio;
	DownloadMessage.ErrorMessage = "Downloading...";
	Progress.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadGameIcon::DownloadCompleted(EDownloadToStorageResult DownloadResult)
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

void UAsyncAction_DownloadGameIcon::Activate()
{
	DownloadMessage = FModioAPI_DownloadIconMessage();
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

	FModioAPI_Game Game;
	if (!ModioConnection->GetCached_Game(Game) || Game.ID < 0)
	{
		DownloadMessage.ErrorMessage = "Modio Connection hasn't cached its game yet!";
		Error.Broadcast(DownloadMessage);
		return;
	}

	IconObject = Game.Icon;
	FString FileDownloadURL = Game.Icon.Original;
	FString FileDownloadPath = ModioConnection->GetMediaDirectoryPathForGame() + "Icon" + FPaths::GetExtension(Game.Icon.Filename, true);

	// Add Progress & Completed Events
	OnStarted.BindUFunction(this, "DownloadStarted");
	OnProgress.BindUFunction(this, "DownloadProgress");
	OnCompleted.BindUFunction(this, "DownloadCompleted");

	DownloadStarted(-1, -1, 0);

	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Running;
	Downloader = UFileToStorageDownloader::DownloadFileToStorage(FileDownloadURL, FileDownloadPath, 0, "", false, OnProgress, OnCompleted);
}

UAsyncAction_DownloadGameIcon* UAsyncAction_DownloadGameIcon::AsyncActionDownloadGameIcon(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken)
{
	// Create Action Instance for Blueprint System
	UAsyncAction_DownloadGameIcon* Action = NewObject<UAsyncAction_DownloadGameIcon>();
	Action->ModioConnection = ModioConnection;
	Action->AccessToken = AccessToken;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}