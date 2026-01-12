// Fill out your copyright notice in the Description page of Project Settings.


#include "MultiplayerSessionSubsystem.h"
#include "OnlineSubsystem.h"
#include "Logging/MSLogger.h"

UMultiplayerSessionSubsystem::UMultiplayerSessionSubsystem()
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
