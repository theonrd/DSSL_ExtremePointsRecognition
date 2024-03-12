// Fill out your copyright notice in the Description page of Project Settings.

#include "ExtremePointsRecognition.h"

#include "SkeletalRenderPublic.h"
#include "Rendering/SkeletalMeshRenderData.h"

bool UExtremePointsRecognition::GetSkeletalMeshVertices(TArray<FVector3f>& OutVertices, USkeletalMeshComponent* MeshComponent, const int32 LODIndex)
{
	OutVertices.Empty();

	if (!IsValid(MeshComponent))
	{
		UE_LOG(LogTemp, Error, TEXT("Can't get mesh vertices because mesh is invalid!"))
		return false;
	}
	
	if (MeshComponent->GetCPUSkinningEnabled())
	{
		GetLocalVerticesWithCpuSkinning(OutVertices, MeshComponent, LODIndex);
	}
	else
	{
		GetLocalVerticesWithGpuSkinning(OutVertices, MeshComponent, LODIndex);
	}

	TransformLocalToWorld(OutVertices, MeshComponent);

	return true;
}

TArray<FVector2D> UExtremePointsRecognition::ProjectExtremeVerticesToScreen(APlayerController* LocalPlayerController, TArray<FVector> const& Vertices)
{
	if (Vertices.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("Can't find extreme vertices with empty vertices array!"))
		return {};
	}
	
	int32 ViewportSizeX, ViewportSizeY;
	LocalPlayerController->GetViewportSize(ViewportSizeX, ViewportSizeY);

	const FVector2D HalfViewportSize(ViewportSizeX / 2, ViewportSizeY / 2);
	
	// Initialize vertices with "worst" values.
	constexpr double DoubleNegativeMax = DBL_MAX * -1;
	FVector2D Left(DBL_MAX);
	FVector2D Right(DoubleNegativeMax);
	FVector2D Top(DBL_MAX);
	FVector2D Bottom(DoubleNegativeMax);
	
	for (auto const& Vert : Vertices)
	{
		FVector2D ScreenLocation;
		LocalPlayerController->ProjectWorldLocationToScreen(Vert, ScreenLocation, true);

		// Make coordinates center-relative for convenient resolution scaling.
		ScreenLocation -= HalfViewportSize;

		if (ScreenLocation.X < Left.X)
		{
			Left = ScreenLocation;
		}

		if (ScreenLocation.X > Right.X)
		{
			Right = ScreenLocation;
		}

		if (ScreenLocation.Y < Top.Y)
		{
			Top = ScreenLocation;
		}

		if (ScreenLocation.Y > Bottom.Y)
		{
			Bottom = ScreenLocation;
		}
	}
	
	return {std::move(Left), std::move(Right), std::move(Top), std::move(Bottom)};
}

void UExtremePointsRecognition::GetLocalVerticesWithCpuSkinning(TArray<FVector3f>& OutVertices,
	const USkeletalMeshComponent* MeshComponent, const int32 LODIndex)
{
	TArray<FFinalSkinVertex> SkinnedVertices;
	MeshComponent->GetCPUSkinnedVertices(SkinnedVertices, LODIndex);
	
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
