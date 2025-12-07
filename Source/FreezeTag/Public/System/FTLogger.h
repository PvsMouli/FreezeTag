// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

/**
 * 
 */
class FREEZETAG_API FTLogger
{
public:
	FTLogger();
	~FTLogger();
	static void LogToFile(const FString& Message);
};
