// Fill out your copyright notice in the Description page of Project Settings.


#include "System/FTLogger.h"

FTLogger::FTLogger()
{
}

FTLogger::~FTLogger()
{
}

void FTLogger::LogToFile(const FString& Message)
{
    FString FilePath = FPaths::ProjectLogDir() + TEXT("MyCustomLog.txt");

    FString Final = FDateTime::Now().ToString() + TEXT(" : ") + Message + TEXT("\n");

    FFileHelper::SaveStringToFile(Final, *FilePath,
        FFileHelper::EEncodingOptions::AutoDetect,
        &IFileManager::Get(),
        FILEWRITE_Append);
}

