/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "ModioAPIObject.h"
#include "ModioAPISubsystem.generated.h"

/**
 * 
 */
UCLASS(meta = (DisplayName = "mod.io API"))
class MODIOAPI_API UModioAPISubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
	public:
		UPROPERTY()
		TMap<int32, UModioAPIObject*> Connections;

	public:
		UFUNCTION(BlueprintCallable, Category="mod.io API|Connections", meta = (DisplayName = "Create mod.io API Connection"))
		bool CreateModioAPIConnection(int32 GameID, FString ApiKey, bool AutomateAccessTokenCaching, bool StoreAccessTokenPersistingly, FString& Message);

		UFUNCTION(BlueprintPure, Category = "mod.io API|Connections", meta = (DisplayName = "Get mod.io API Connection"))
		UModioAPIObject* GetFirstConnection();

		UFUNCTION(BlueprintPure, Category = "mod.io API|Connections", meta = (DisplayName = "Get mod.io API Connection for Game"))
		UModioAPIObject* GetConnectionForGameID(int32 GameID);
};
