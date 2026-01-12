// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

//By default custom logging(second parameter) is enabled, enable UE logging as well by setting the first parameter to true
#define MS_LOG(Message, ...) MSLogger::LogToFile(Message, ##__VA_ARGS__)
/**
 * 
 */
class MULTIPLAYERSESSIONS_API MSLogger
{
public:
	MSLogger();
	~MSLogger();
	static void LogToFile(const FString& Message, bool bUseUeLog = false, bool bUseCustomLog = true);
	static bool bIsLoggingEnabled;
};
