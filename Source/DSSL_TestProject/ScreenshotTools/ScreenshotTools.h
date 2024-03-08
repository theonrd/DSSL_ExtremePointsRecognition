// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ScreenshotTools.generated.h"

/**
 * Common features to make screenshots.
 */
UCLASS()
class DSSL_TESTPROJECT_API UScreenshotTools : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category = "Screenshot Tools")
	static void MakeScreenshot(FString ProjectRelativeDirectory, FString FileName, int32 ResolutionX = 1920, int32 ResolutionY = 1080, float ResolutionScale = 1);
};
