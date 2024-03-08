// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtremePointsRecognition.h"

#include "SkeletalRenderPublic.h"
#include "Rendering/SkeletalMeshRenderData.h"

TArray<FVector3f> UExtremePointsRecognition::GetSkeletalMeshVertices(USkeletalMeshComponent* MeshComponent, int32 LODIndex)
{
	TArray<FVector3f> Result;

	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get mesh vertices because mesh is invalid!"))
		return Result;
	}
	
	if (MeshComponent->GetCPUSkinningEnabled())
	{
		GetLocalVerticesWithCpuSkinning(Result, MeshComponent, LODIndex);
	}
	else
	{
		GetLocalVerticesWithGpuSkinning(Result, MeshComponent, LODIndex);
	}

	TransformLocalToWorld(Result, MeshComponent);

	return Result;
}

TArray<FVector2D> UExtremePointsRecognition::ProjectExtremeVerticesToScreen(APlayerController* LocalPlayerController, TArray<FVector> Vertices)
{
	TOptional<FVector2D> Left, Right, Top, Bottom;

	if (Vertices.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find extreme vertices with empty vertices array!"))
		return {};
	}
	
	int32 ViewportSizeX, ViewportSizeY;
	LocalPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	for (const auto& Vert : Vertices)
	{
		FVector2D ScreenLocation;
		LocalPlayerController->ProjectWorldLocationToScreen(Vert, ScreenLocation, true);

		// Make coordinates center-relative for convenient resolution scaling.
		ScreenLocation -= FVector2D(ViewportSizeX / 2, ViewportSizeY / 2);

		if (!Left.IsSet() || ScreenLocation.X < Left.GetValue().X)
		{
			Left = ScreenLocation;
		}

		if (!Right.IsSet() || ScreenLocation.X > Right.GetValue().X)
		{
			Right = ScreenLocation;
		}

		if (!Top.IsSet() || ScreenLocation.Y < Top.GetValue().Y)
		{
			Top = ScreenLocation;
		}

		if (!Bottom.IsSet() || ScreenLocation.Y > Bottom.GetValue().Y)
		{
			Bottom = ScreenLocation;
		}
	}
	
	return {Left.GetValue(), Right.GetValue(), Top.GetValue(), Bottom.GetValue()};
}

void UExtremePointsRecognition::GetLocalVerticesWithCpuSkinning(TArray<FVector3f>& OutVertices,
	const USkeletalMeshComponent* MeshComponent, const int32 LODIndex)
{
	TArray<FFinalSkinVertex> SkinnedVertices;
	MeshComponent->GetCPUSkinnedVertices(SkinnedVertices, LODIndex);

	OutVertices.Empty();
	OutVertices.AddUninitialized(SkinnedVertices.Num());
	for (int32 i = 0; i < SkinnedVertices.Num(); ++i)
	{
		OutVertices[i] = SkinnedVertices[i].Position;
	}
}

void UExtremePointsRecognition::GetLocalVerticesWithGpuSkinning(TArray<FVector3f>& OutVertices,
	USkeletalMeshComponent* MeshComponent, const int32 LODIndex)
{
	const FSkeletalMeshRenderData* RenderData = MeshComponent->GetSkeletalMeshRenderData();

	if (!RenderData)
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get mesh vertices because render data is invalid!"))
		return;
	}

	const FSkeletalMeshLODRenderData& LOD = RenderData->LODRenderData[LODIndex];
	const FSkinWeightVertexBuffer& Buffer = LOD.SkinWeightVertexBuffer;

	TArray<FMatrix44f> CacheToLocals;
	MeshComponent->GetCurrentRefToLocalMatrices(CacheToLocals, LODIndex);

	MeshComponent->ComputeSkinnedPositions(MeshComponent, OutVertices, CacheToLocals, LOD, Buffer);
}

void UExtremePointsRecognition::TransformLocalToWorld(TArray<FVector3f>& InOutVertices, const USkeletalMeshComponent* MeshComponent)
{
	const FTransform ToWorld = MeshComponent->GetComponentTransform();

	for (FVector3f& EachVertex : InOutVertices)
	{
		EachVertex = static_cast<FVector3f>(ToWorld.TransformPosition(static_cast<FVector>(EachVertex)));
	}
}
