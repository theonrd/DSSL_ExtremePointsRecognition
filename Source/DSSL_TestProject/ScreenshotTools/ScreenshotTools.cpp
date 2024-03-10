// Fill out your copyright notice in the Description page of Project Settings.

#include "ScreenshotTools.h"

#include "Blueprint/WidgetLayoutLibrary.h"
#include "Engine/Canvas.h"
#include "Kismet/KismetRenderingLibrary.h"

void UScreenshotTools::OverlayPointsToRenderTarget(UObject* WorldContextObject, UTextureRenderTarget2D* RT,
	TArray<FVector2D> const& Vertices, const float PointSize, UTexture* PointTexture, const FLinearColor Color)
{
	if (RT == nullptr || PointTexture == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't overlay points on render target without render target and texture!"))
		return;
	}
	
	// Initialize RT Canvas.
	UCanvas* RenderCanvas;
	FVector2D ScreenSize;
	FDrawToRenderTargetContext Context;
	UKismetRenderingLibrary::BeginDrawCanvasToRenderTarget(WorldContextObject, RT, RenderCanvas, ScreenSize, Context);

	const float ViewportScale = UWidgetLayoutLibrary::GetViewportScale(WorldContextObject);
	const FVector2D HalfScreenSize = ScreenSize / 2;

	for (FVector2D const& Vertex : Vertices)
	{
		const FVector2D RtScaledVertex = (Vertex / ViewportScale) + HalfScreenSize;

		const FVector2D ImageAdjustedRtVertex = RtScaledVertex - (PointSize / 2);

		RenderCanvas->K2_DrawTexture(PointTexture, ImageAdjustedRtVertex, FVector2D(PointSize),
			FVector2D::ZeroVector, FVector2D::UnitVector, Color);
	}

	UKismetRenderingLibrary::EndDrawCanvasToRenderTarget(WorldContextObject, Context);
}

void UScreenshotTools::TakeRenderTargetShot(UObject* WorldContextObject, UTextureRenderTarget2D* RT, const FString ProjectRelativeDirectory,const FString FileName)
{
	const FString AbsoluteFileDirectory =
		FString::Printf(TEXT("%s/%s/"), *FPaths::ProjectDir(), *ProjectRelativeDirectory);
	
	const FString FormattedFileName = FString::Printf(TEXT("%s.png"), *FileName);
	
	UKismetRenderingLibrary::ExportRenderTarget(WorldContextObject, RT, AbsoluteFileDirectory, FormattedFileName);
}

void UScreenshotTools::DeleteDirectory(FString RelativeDirectory)
{
	// Sure you don't want to delete project
	if (RelativeDirectory.IsEmpty())
	{
		return;
	}
	
	IPlatformFile& FileManager = FPlatformFileManager::Get().GetPlatformFile();

	const FString AbsoluteFileDirectory =
		FString::Printf(TEXT("%s/%s/"), *FPaths::ProjectDir(), *RelativeDirectory);
	
	FileManager.DeleteDirectoryRecursively(*AbsoluteFileDirectory);
}
