/*
Copyright © 2023 Arvur GmbH / Robin Hasenbach.
Released under GNU AGPLv3 License.
*/

#include "ModioAPISubsystem.h"

bool UModioAPISubsystem::CreateModioAPIConnection(int32 GameID, FString ApiKey, bool AutomateAccessTokenCaching, bool StoreAccessTokenPersistingly, FString& Message)
{
	if (Connections.Num() > 0)
	{
		if (Connections.Find(GameID))
		{
			Message = "A Connection for this GameID already exists!";
			return false;
		}
	}

	UModioAPIObject* Connection = NewObject<UModioAPIObject>(UModioAPIObject::StaticClass());
	FString ConnectionCreationMessage;
	if (Connection->InitializeModioAPI(GameID, ApiKey, AutomateAccessTokenCaching, StoreAccessTokenPersistingly, ConnectionCreationMessage))
	{
		Connections.Add(GameID, Connection);
		Message = "Successfully created Connection for GameID " + FString::FromInt(GameID) + "!";
		return true;
	}

	Message = "Connection for GameID " + FString::FromInt(GameID) + " couldn't be created! /n";
	Message += "Connection Creation Message: " + ConnectionCreationMessage;
	return false;
}

UModioAPIObject* UModioAPISubsystem::GetFirstConnection()
{
	UModioAPIObject* ReturnValue;

	// No Connections...
	if (Connections.Num() < 1)
	{
		return nullptr;
	}

	// Get the first available Connection
	TArray<UModioAPIObject*> AvailableConnections;
	Connections.GenerateValueArray(AvailableConnections);
	ReturnValue = AvailableConnections[0];

	if (ReturnValue)
	{
		return ReturnValue;
	}

	return nullptr;
}

UModioAPIObject* UModioAPISubsystem::GetConnectionForGameID(int32 GameID)
{
	if (Connections.Find(GameID))
	{
		return Connections[GameID];
	}

	return nullptr;
}
