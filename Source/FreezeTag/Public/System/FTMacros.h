// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"

DECLARE_LOG_CATEGORY_EXTERN(FTMPLogs, Log, All);

/* ------------------------------------------------------------
 *  INTERNAL BASE MACRO
 * ------------------------------------------------------------ */
#define __LOG_BASE(CategoryName, Verbosity, Format, ...) \
    UE_LOG(CategoryName, Verbosity, TEXT("[%s][%s:%d] " Format), \
        TEXT(__FUNCTION__), TEXT(__FILE__), __LINE__, ##__VA_ARGS__)

/* ------------------------------------------------------------
 *  NETWORK-SPECIFIC LOGGING
 * ------------------------------------------------------------ */
#define FTMP_LOG(Format, ...)       __LOG_BASE(FTMPLogs, Log, Format, ##__VA_ARGS__)
#define FTLogToFile(Message)    FTLogger::LogToFile(Message)

/**
 * 
 */
class FREEZETAG_API FTMacros
{
public:
	FTMacros();
	~FTMacros();
};
