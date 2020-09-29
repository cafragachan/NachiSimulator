// Fill out your copyright notice in the Description page of Project Settings.


#include "zFunctionLibrary.h"

#include "Misc/FileHelper.h"
#include "HAL/PlatformFilemanager.h"

bool UzFunctionLibrary::SaveTextFile(FString SaveDirectory, TArray<FString> SaveText, bool AllowOverwrite)
{
	if (!AllowOverwrite)
	{
		if (FPlatformFileManager::Get().GetPlatformFile().FileExists(*SaveDirectory))
		{
			return false;
		}
	}

	FString FinalString = "";
	for (auto& Line : SaveText)
	{
		FinalString += Line;
		FinalString += LINE_TERMINATOR;
	}

	return FFileHelper::SaveStringToFile(FinalString, *SaveDirectory);
}

