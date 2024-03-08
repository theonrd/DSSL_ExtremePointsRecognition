// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "ExtremePointsRecognition.generated.h"

/**
 * Library for recognition screen-relative extreme points of mesh.
 */
UCLASS()
class DSSL_TESTPROJECT_API UExtremePointsRecognition : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	/**
	 * Get world coordinates with all vertices.
	 * @param MeshComponent Mesh to use
	 * @param LODIndex
	 * @return Array of world coordinates of each vert.
	 */
	UFUNCTION(BlueprintCallable, Category = "Extreme Points Recognition")
	static TArray<FVector3f> GetMeshVertices(USkeletalMeshComponent* MeshComponent, int32 LODIndex);

	/**
	 * Find and return extreme points and project it to screen.
	 * @param LocalPlayerController 
	 * @param Vertices All vertices to check
	 * @return Array of four screen-relative 2D points with extreme points (left, right, top, bottom)
	 */
	UFUNCTION(BlueprintCallable, Category = "Extreme Points Recognition")
	static TArray<FVector2D> FindExtremeVertices(APlayerController* LocalPlayerController, TArray<FVector> Vertices);

private:
	static void GetLocalVerticesWithCpuSkinning(TArray<FVector3f>& OutVertices, const USkeletalMeshComponent* MeshComponent, int32 LODIndex);

	static void GetLocalVerticesWithGpuSkinning(TArray<FVector3f>& OutVertices, USkeletalMeshComponent* MeshComponent, int32 LODIndex);

	static void TransformLocalToWorld(TArray<FVector3f>& InOutVertices, const USkeletalMeshComponent* MeshComponent);
};
