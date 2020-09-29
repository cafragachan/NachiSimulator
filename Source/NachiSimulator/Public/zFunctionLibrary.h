// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "zFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class NACHISIMULATOR_API UzFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	UFUNCTION(BlueprintCallable, Category = "zSpace")
	static bool SaveTextFile(FString SaveDirectory, TArray<FString> SaveText, bool AllowOverwrite);
	
};
