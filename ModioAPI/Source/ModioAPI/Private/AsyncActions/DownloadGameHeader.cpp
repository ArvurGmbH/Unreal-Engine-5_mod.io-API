/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "AsyncActions/DownloadGameHeader.h"


void UAsyncAction_DownloadGameHeader::DownloadStarted(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio)
{
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.ProgressInfo.BytesReceived = DownloadBytesReceived;
	DownloadMessage.ProgressInfo.BytesTotal = DownloadBytesTotal;
	DownloadMessage.ProgressInfo.Progress = DownloadProgressRatio;
	DownloadMessage.ErrorMessage = "Download started!";
	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Running;
	DownloadMessage.Header = HeaderObject;
	Started.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadGameHeader::DownloadProgress(int64 DownloadBytesReceived, int64 DownloadBytesTotal, float DownloadProgressRatio)
{
	DownloadMessage.ProgressInfo = FModioAPI_DownloadFileProgress();
	DownloadMessage.ProgressInfo.BytesReceived = DownloadBytesReceived;
	DownloadMessage.ProgressInfo.BytesTotal = DownloadBytesTotal;
	DownloadMessage.ProgressInfo.Progress = DownloadProgressRatio;
	DownloadMessage.ErrorMessage = "Downloading...";
	Progress.Broadcast(DownloadMessage);
}

void UAsyncAction_DownloadGameHeader::DownloadCompleted(EDownloadToStorageResult DownloadResult)
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

void UAsyncAction_DownloadGameHeader::Activate()
{
	DownloadMessage = FModioAPI_DownloadHeaderMessage();
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

	HeaderObject = Game.Header;
	FString FileDownloadURL = Game.Header.Original;
	FString FileDownloadPath = ModioConnection->GetMediaDirectoryPathForGame() + "Header" + FPaths::GetExtension(Game.Header.Filename, true);

	// Add Progress & Completed Events
	OnStarted.BindUFunction(this, "DownloadStarted");
	OnProgress.BindUFunction(this, "DownloadProgress");
	OnCompleted.BindUFunction(this, "DownloadCompleted");

	DownloadStarted(-1, -1, 0);

	DownloadMessage.Result = EModioAPI_DownloadResult::DownloadResult_Running;
	Downloader = UFileToStorageDownloader::DownloadFileToStorage(FileDownloadURL, FileDownloadPath, 0, "", false, OnProgress, OnCompleted);
}

UAsyncAction_DownloadGameHeader* UAsyncAction_DownloadGameHeader::AsyncActionDownloadGameHeader(UObject* WorldContextObject, UModioAPIObject* ModioConnection, FString AccessToken)
{
	// Create Action Instance for Blueprint System
	UAsyncAction_DownloadGameHeader* Action = NewObject<UAsyncAction_DownloadGameHeader>();
	Action->ModioConnection = ModioConnection;
	Action->AccessToken = AccessToken;
	Action->RegisterWithGameInstance(WorldContextObject);

	return Action;
}