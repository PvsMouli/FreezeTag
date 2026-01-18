// Fill out your copyright notice in the Description page of Project Settings.

#include "System/FTGameInstance.h"
//#include "System/FTMacros.h"
#include "System/FTLogger.h"

void UFTGameInstance::Init()
{	
	Super::Init();
	//Just for testing purpose
	FT_LOG("UFTGameInstance::Init", true, false);
	//FTMP_LOG(" ");
	//IOnlineSubsystem* OnlineSub = IOnlineSubsystem::Get();
}