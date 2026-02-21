// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Interfaces/OnlineSessionInterface.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MenuWidget.generated.h"
class UMultiplayerSessionSubsystem;

/**
 * 
 */
UCLASS(Blueprintable)
class MULTIPLAYERSESSIONS_API UMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	//Called from character blueprint event begin play
	UFUNCTION(BlueprintCallable)
	void MenuSetup(int NumberOfPublicConnection = 4, FString TypeOfMatch = "FreeForAll");
protected:
	/*
	* Removed this becuase sometimes intialize is not getting called so I was returning false without
	* calling super which in turn caused another crash in MenuSetup() SetVisibility() function becuase
	* SuperIntialize() is not called it didn't get constructed casuing access violation error
	*/
	virtual bool Initialize() override;
	virtual void NativeDestruct() override;
private:
	UFUNCTION()	
	void OnCreateSession(bool bWasSuccessful);
	void OnFindSessions(const TArray<FOnlineSessionSearchResult>& SessionResults, bool bWasSuccessful);
	void OnJoinSession(EOnJoinSessionCompleteResult::Type Result, const FString& address);
	UFUNCTION()
	void OnDestroySession(bool bWasSuccessful);
	UFUNCTION()
	void OnStartSession(bool bWasSuccessful);

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonHost;

	UPROPERTY(meta = (BindWidget))
	class UButton* ButtonJoin;

	UFUNCTION()
	void HostButtonClicked();
	UFUNCTION()
	void JoinButtonClicked();

	UFUNCTION()
	void MenuTearDown();

	UPROPERTY()
	TObjectPtr<UMultiplayerSessionSubsystem> MultiplayerSessionSubsystem;
};
