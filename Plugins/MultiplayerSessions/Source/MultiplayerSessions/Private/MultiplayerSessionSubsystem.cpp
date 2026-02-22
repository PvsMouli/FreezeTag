// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Logging/MSLogger.h"
#include "OnlineSessionSettings.h"
#include <Online/OnlineSessionNames.h>

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem():
	CreateSessionCompleteDelegate(FOnCreateSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnCreateSessionComplete)),
	FindSessionsCompleteDelegate(FOnFindSessionsCompleteDelegate::CreateUObject(this, &ThisClass::OnFindSessionsComplete)),
	JoinSessionCompleteDelegate(FOnJoinSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnJoinSessionComplete)),
	DestroySessionCompleteDelegate(FOnDestroySessionCompleteDelegate::CreateUObject(this, &ThisClass::OnDestroySessionComplete)),
	StartSessionCompleteDelegate(FOnStartSessionCompleteDelegate::CreateUObject(this, &ThisClass::OnStartSessionComplete))
{
	OnlineSessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	if (OnlineSessionInterface.IsValid())
	{
		MS_LOG("UMultiplayerSessionSubsystem() Online Session Interface is valid.", true);
	}
	else
	{
		MS_LOG("UMultiplayerSessionSubsystem() Online Session Interface is NOT valid.", true);
	}
}

void UMultiplayerSessionSubsystem::CreateSession(int32 NumPublicConnections, FString MatchType)
{
	MS_LOG("UMultiplayerSessionSubsystem::CreateSession called.", true);
	if (!OnlineSessionInterface.IsValid())
	{
		MS_LOG("UMultiplayerSessionSubsystem::CreateSession: Online Session Interface is NOT valid.", true);
		return;
	}

	auto ExisitingSession = OnlineSessionInterface->GetNamedSession(NAME_GameSession);
	if (ExisitingSession)
	{
		MS_LOG("UMultiplayerSessionSubsystem::CreateSession: Session already exists, destroying it first.", true);
		OnlineSessionInterface->DestroySession(NAME_GameSession);
	}
	CreateSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegate);

	LastSessionSettings = MakeShareable(new FOnlineSessionSettings());	
	LastSessionSettings->bIsLANMatch = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSettings->NumPublicConnections = NumPublicConnections;
	LastSessionSettings->bAllowJoinInProgress = true;
	LastSessionSettings->bAllowJoinViaPresence = true;
	LastSessionSettings->bShouldAdvertise = true;
	LastSessionSettings->bUsesPresence = true;
	LastSessionSettings->bUseLobbiesIfAvailable = true;
	LastSessionSettings->Set(FName("MATCH_TYPE"), MatchType, EOnlineDataAdvertisementType::ViaOnlineServiceAndPing);


	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	bool isSuccess = OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings); OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings);	OnlineSessionInterface->CreateSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, *LastSessionSettings);
	if (isSuccess)
	{
		MS_LOG("UMultiplayerSessionSubsystem::CreateSession: CreateSession call was successful.", true);
	}
	else
	{
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
		OnMultiplayerCreateSessionComplete.Broadcast(false);
		MS_LOG("UMultiplayerSessionSubsystem::CreateSession: CreateSession call failed.", true);
	}

}

void UMultiplayerSessionSubsystem::FindSessions(int32 MaxSearchResults)
{
	if (!OnlineSessionInterface.IsValid())
	{
		MS_LOG("UMultiplayerSessionSubsystem::CreateSession: Online Session Interface is NOT valid.", true);
		return;
	}

	FindSessionsCompleteDelegateHandle = OnlineSessionInterface->AddOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegate);
	LastSessionSearch = MakeShareable(new FOnlineSessionSearch());
	LastSessionSearch->MaxSearchResults = MaxSearchResults;
	LastSessionSearch->bIsLanQuery = IOnlineSubsystem::Get()->GetSubsystemName() == "NULL" ? true : false;
	LastSessionSearch->QuerySettings.Set(SEARCH_LOBBIES, true, EOnlineComparisonOp::Equals);

	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (OnlineSessionInterface->FindSessions(*LocalPlayer->GetPreferredUniqueNetId(), LastSessionSearch.ToSharedRef()))
	{
		MS_LOG("UMultiplayerSessionSubsystem::FindSessions: FindSessions call was successful.", true);		
	}
	else
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
		OnMultiplayerFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		MS_LOG("UMultiplayerSessionSubsystem::FindSessions: FindSessions call failed.", true);
	}
}

void UMultiplayerSessionSubsystem::JoinSession(const FOnlineSessionSearchResult& SessionResult)
{
	if (!OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		OnMultiplayerJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		MS_LOG("UMultiplayerSessionSubsystem::JoinSession: Online Session Interface is NOT valid.", true);
		return;
	}
	JoinSessionCompleteDelegateHandle = OnlineSessionInterface->AddOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegate);
	const ULocalPlayer* LocalPlayer = GetWorld()->GetFirstLocalPlayerFromController();
	if (OnlineSessionInterface->JoinSession(*LocalPlayer->GetPreferredUniqueNetId(), NAME_GameSession, SessionResult))
	{
		MS_LOG("UMultiplayerSessionSubsystem::JoinSession: JoinSession call was successful.", true);
	}
	else
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
		OnMultiplayerJoinSessionComplete.Broadcast(EOnJoinSessionCompleteResult::UnknownError);
		MS_LOG("UMultiplayerSessionSubsystem::JoinSession: JoinSession call failed.", true);
	}
}

void UMultiplayerSessionSubsystem::DestroySession()
{
}

void UMultiplayerSessionSubsystem::StartSession()
{
}

void UMultiplayerSessionSubsystem::OnCreateSessionComplete(FName SessionName, bool bWasSuccessful)
{
	MS_LOG("UMultiplayerSessionSubsystem::OnCreateSessionComplete called.", true);
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnCreateSessionCompleteDelegate_Handle(CreateSessionCompleteDelegateHandle);
	}
	OnMultiplayerCreateSessionComplete.Broadcast(bWasSuccessful);	

}

void UMultiplayerSessionSubsystem::OnFindSessionsComplete(bool bWasSuccessful)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnFindSessionsCompleteDelegate_Handle(FindSessionsCompleteDelegateHandle);
	}

	if (LastSessionSearch->SearchResults.Num() <= 0)
	{
		MS_LOG("UMultiplayerSessionSubsystem::OnFindSessionsComplete: No sessions found.", true);
		OnMultiplayerFindSessionsComplete.Broadcast(TArray<FOnlineSessionSearchResult>(), false);
		return;
	}

	OnMultiplayerFindSessionsComplete.Broadcast(LastSessionSearch->SearchResults, bWasSuccessful);
}

void UMultiplayerSessionSubsystem::OnJoinSessionComplete(FName SessionName, EOnJoinSessionCompleteResult::Type Result)
{
	if (OnlineSessionInterface.IsValid())
	{
		OnlineSessionInterface->ClearOnJoinSessionCompleteDelegate_Handle(JoinSessionCompleteDelegateHandle);
	}
	OnMultiplayerJoinSessionComplete.Broadcast(Result);
	
}

void UMultiplayerSessionSubsystem::OnDestroySessionComplete(FName SessionName, bool bWasSuccessful)
{
}

void UMultiplayerSessionSubsystem::OnStartSessionComplete(FName SessionName, bool bWasSuccessful)
{
}
