// Fill out your copyright notice in the Description page of Project Settings.


#include "Logging/MSLogger.h"

bool MSLogger::bIsLoggingEnabled = true;

MSLogger::MSLogger()
{
}

MSLogger::~MSLogger()
{
}

void MSLogger::LogToFile(const FString& Message, bool bUseUeLog, bool bUseCustomLog)
{
	if (!bIsLoggingEnabled)
	{
		return;
	}
	if (bUseUeLog)
	{
		UE_LOG(LogTemp, Log, TEXT("%s"), *Message);
	}
	if (bUseCustomLog)
	{
		FString FilePath = FPaths::ProjectLogDir() + TEXT("MSLogs.txt");
		FString Final = FDateTime::Now().ToString() + TEXT(" : ")  + Message + TEXT("\n");
		FFileHelper::SaveStringToFile(Final, *FilePath,
			FFileHelper::EEncodingOptions::AutoDetect,
			&IFileManager::Get(),
			FILEWRITE_Append);
	}
}
