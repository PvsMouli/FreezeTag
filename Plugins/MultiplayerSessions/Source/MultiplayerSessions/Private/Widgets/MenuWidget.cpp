// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MenuWidget.h"
#include "Logging/MSLogger.h"
#include "Components/Button.h"
#include "MultiplayerSessionSubsystem.h"
#include "OnlineSessionSettings.h"
#include "Interfaces/OnlineSessionInterface.h"

void UMenuWidget::MenuSetup(int NumberOfPublicConnection, FString TypeOfMatch)
{
	MS_LOG("UMenuWidget::MenuSetup", true);
	AddToViewport();
	SetVisibility(ESlateVisibility::Visible);
	bIsFocusable = true;

	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeUIOnly InputModeData;
			InputModeData.SetWidgetToFocus(TakeWidget());
			InputModeData.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = true;
			//MS_LOG("UMenuWidget::MenuSetup: Input mode set to UI only and mouse cursor shown.", true);
		}
		/*else
		{
			MS_LOG("UMenuWidget::MenuSetup: PlayerController is null!", true);
		}*/
	}
	/*else
	{
		MS_LOG("UMenuWidget::MenuSetup: World is null!", true);
	}*/
	if (ButtonHost)
	{
		ButtonHost->OnReleased.Clear();
		ButtonHost->OnReleased.AddDynamic(this, &UMenuWidget::HostButtonClicked);
		//ButtonHost->OnReleased
	}
	else
	{
		MS_LOG("UMenuWidget::Initialize: ButtonHost is null!", false, true);
	}
	if (ButtonJoin)
	{
		ButtonJoin->OnReleased.Clear();
		ButtonJoin->OnReleased.AddDynamic(this, &UMenuWidget::JoinButtonClicked);
	}
	else
	{
		MS_LOG("UMenuWidget::Initialize: ButtonJoin is null!", false, true);
	}

	UGameInstance* GameInstance = GetGameInstance();
	if (GameInstance)
	{
		MultiplayerSessionSubsystem = GameInstance->GetSubsystem<UMultiplayerSessionSubsystem>();
		if (MultiplayerSessionSubsystem)
		{
			MS_LOG("UMenuWidget::MenuSetup: MultiplayerSessionSubsystem found.", true);
			MultiplayerSessionSubsystem->OnMultiplayerCreateSessionComplete.AddDynamic(this, &UMenuWidget::OnCreateSession);
			MultiplayerSessionSubsystem->OnMultiplayerFindSessionsComplete.AddUObject(this, &UMenuWidget::OnFindSessions);
			MultiplayerSessionSubsystem->OnMultiplayerJoinSessionComplete.AddUObject(this, &UMenuWidget::OnJoinSession);
			MultiplayerSessionSubsystem->OnMultiplayerDestroySessionComplete.AddDynamic(this, &UMenuWidget::OnDestroySession);
			MultiplayerSessionSubsystem->OnMultiplayerStartSessionComplete.AddDynamic(this, &UMenuWidget::OnStartSession);
		}
		else
		{
			MS_LOG("UMenuWidget::MenuSetup: MultiplayerSessionSubsystem is null!", true);
		}
	}
}

bool UMenuWidget::Initialize()
{
	Super::Initialize();
	MS_LOG("UMenuWidget::Initialize: ", true);
	return false; //Initialize is not getting called properly, so returning false to prevent crashes

	/*MS_LOG("UMenuWidget::Initialize: ",false, true);
	if (!Super::Initialize())
	{
		MS_LOG("UMenuWidget::Initialize: Super::Initialize() failed!",false, true);
		return false;
	}
	if (ButtonHost)
	{
		ButtonHost->OnClicked.AddDynamic(this, &UMenuWidget::HostButtonClicked);
	}
	else
	{
		MS_LOG("UMenuWidget::Initialize: ButtonHost is null!",false, true);
	}
	if (ButtonJoin)
	{
		ButtonJoin->OnClicked.AddDynamic(this, &UMenuWidget::JoinButtonClicked);
	}
	else
	{
		MS_LOG("UMenuWidget::Initialize: ButtonJoin is null!",false, true);
	}
	return false;*/
}

void UMenuWidget::NativeDestruct()
{
	Super::NativeDestruct();
	MenuTearDown();
}

void UMenuWidget::OnCreateSession(bool bWasSuccessful)
{
	if (bWasSuccessful)
	{
		UWorld* World = GetWorld();
		if (World)
		{
			World->ServerTravel("/Game/FreezeTag/Levels/Lobby?listen");
			MS_LOG("UMenuWidget::OnCreateSession: Server traveling to Lobby.", true);
		}
	}	
	else
	{
		MS_LOG("UMenuWidget::OnCreateSession: Failed to create session!", true);
	}
}

void UMenuWidget::OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful)
{
	MS_LOG("UMenuWidget::OnFindSessions:", true);
	if (MultiplayerSessionSubsystem == nullptr)
	{
		MS_LOG("UMenuWidget::OnFindSessions: MultiplayerSessionSubsystem is null!", true);
		return;
	}

	for (auto Result : SessionResults)
	{
		FString ServerName;
		Result.Session.SessionSettings.Get(FName("MATCH_TYPE"), ServerName);
		if (ServerName == FString("FreeForAll"))
		{
			MultiplayerSessionSubsystem->JoinSession(Result);
			return;
		}
	}
}

void UMenuWidget::OnJoinSession(EOnJoinSessionCompleteResult::Type Result/*, const FString& address*/)
{
	IOnlineSessionPtr OnlineSessionInterface = IOnlineSubsystem::Get()->GetSessionInterface();
	if (OnlineSessionInterface.IsValid())
	{
		FString Address;
		OnlineSessionInterface->GetResolvedConnectString(NAME_GameSession, Address);

		APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
		if (PlayerController)
		{
			PlayerController->ClientTravel(Address, ETravelType::TRAVEL_Absolute);
			MS_LOG("UMenuWidget::OnJoinSession: Client traveling to session at address: " + Address, true);
		}

	}
	else
	{
		MS_LOG("UMenuWidget::OnJoinSession: Online Session Interface is invalid!", true);
	}
}

void UMenuWidget::OnDestroySession(bool bWasSuccessful)
{
}

void UMenuWidget::OnStartSession(bool bWasSuccessful)
{
}

void UMenuWidget::HostButtonClicked()
{
	MS_LOG("UMenuWidget::HostButtonClicked", true);
	if (MultiplayerSessionSubsystem)
	{
		//TEXT("FreeForAll")
		MultiplayerSessionSubsystem->CreateSession(4, FString("FreeForAll"));		
	}
	else
	{
		MS_LOG("UMenuWidget::HostButtonClicked: MultiplayerSessionSubsystem is null!", true);
	}
}

void UMenuWidget::JoinButtonClicked()
{
	MS_LOG("UMenuWidget::JoinButtonClicked", true);
	if (MultiplayerSessionSubsystem)
	{
		MultiplayerSessionSubsystem->FindSessions(10000);
	}
}

void UMenuWidget::MenuTearDown()
{
	MS_LOG("UMenuWidget::MenuTearDown", true);
	RemoveFromViewport();
	SetVisibility(ESlateVisibility::Hidden);
	bIsFocusable = false;
	UWorld* World = GetWorld();
	if (World)
	{
		APlayerController* PlayerController = World->GetFirstPlayerController();
		if (PlayerController)
		{
			FInputModeGameOnly InputModeData;
			PlayerController->SetInputMode(InputModeData);
			PlayerController->bShowMouseCursor = false;
			//MS_LOG("UMenuWidget::MenuTearDown: Input mode set to Game only and mouse cursor hidden.", true);
		}
	}
}
