// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/MenuWidget.h"
#include "Logging/MSLogger.h"
#include "Components/Button.h"

void UMenuWidget::MenuSetup()
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
			MS_LOG("UMenuWidget::MenuSetup: Input mode set to UI only and mouse cursor shown.", true);
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
}

//bool UMenuWidget::Initialize()
//{
//	return false; //Initialize is not getting called properly, so returning false to prevent crashes
//
//	MS_LOG("UMenuWidget::Initialize: ",false, true);
//	if (!Super::Initialize())
//	{
//		MS_LOG("UMenuWidget::Initialize: Super::Initialize() failed!",false, true);
//		return false;
//	}
//	if (ButtonHost)
//	{
//		ButtonHost->OnClicked.AddDynamic(this, &UMenuWidget::HostButtonClicked);
//	}
//	else
//	{
//		MS_LOG("UMenuWidget::Initialize: ButtonHost is null!",false, true);
//	}
//	if (ButtonJoin)
//	{
//		ButtonJoin->OnClicked.AddDynamic(this, &UMenuWidget::JoinButtonClicked);
//	}
//	else
//	{
//		MS_LOG("UMenuWidget::Initialize: ButtonJoin is null!",false, true);
//	}
//	return false;
//}

void UMenuWidget::HostButtonClicked()
{
	MS_LOG("UMenuWidget::HostButtonClicked", true);
}

void UMenuWidget::JoinButtonClicked()
{
	MS_LOG("UMenuWidget::JoinButtonClicked", true);
}
