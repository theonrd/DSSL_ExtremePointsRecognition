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
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Screenshot Tools")
	static void OverlayPointsToRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* RT,
		TArray<FVector2D> const& Vertices, float PointSize, UTexture* PointTexture, FLinearColor Color);

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject"), Category = "Screenshot Tools")
	static void TakeRenderTargetShot(UObject* WorldContextObject, UTextureRenderTarget2D* RT,
		const FString ProjectRelativeDirectory, const FString FileName);

	/**
	 * Delete selected directory inside project. Be careful.
	 * @param RelativeDirectory Project Relative path
	 */
	UFUNCTION(BlueprintCallable)
	static void DeleteDirectory(FString RelativeDirectory);
};
