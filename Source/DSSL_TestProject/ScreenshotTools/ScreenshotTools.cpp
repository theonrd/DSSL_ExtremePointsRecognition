// Fill out your copyright notice in the Description page of Project Settings.

#include "ScreenshotTools.h"

#include "HighResScreenshot.h"

void UScreenshotTools::MakeScreenshot(const FString ProjectRelativeDirectory, const FString FileName, const int32 ResolutionX, const int32 ResolutionY, float ResolutionScale)
{
	GIsHighResScreenshot = true;

	const FString AbsoluteFileName = FString::Printf(
		TEXT("%s/%s/Screenshot_%s.png"), *FPaths::ProjectDir(), *ProjectRelativeDirectory, *FileName);

	GetHighResScreenshotConfig().SetResolution(ResolutionX, ResolutionY, ResolutionScale);
	GetHighResScreenshotConfig().SetFilename(AbsoluteFileName);

	FScreenshotRequest::RequestScreenshot(AbsoluteFileName, false, false);
}
