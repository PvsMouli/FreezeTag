// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

#define FT_LOG(Message, ...) FTLogger::LogToFile(Message, ##__VA_ARGS__)

/**
 * 
 */
class FREEZETAG_API FTLogger
{
public:
	FTLogger();
	~FTLogger();
	//static void LogToFile(const FString& Message);
	static void LogToFile(const FString& Message, bool bUseUeLog=false, bool bUseCustomLog=true);
	static bool bIsLoggingEnabled;
};
